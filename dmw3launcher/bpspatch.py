"""A pure-Python BPS ("Beat Patching System") applier.

BPS is the patch format the project uses to turn a player's own retail disc image into the finished
modded image. A .bps file holds the steps needed to rebuild one file (the *target*) from another
(the *source*), plus the CRC32 of each end and a CRC32 of the patch itself. This module implements
the APPLY half only - the launcher never creates patches - from the published format, so it needs
no third-party code and carries no licence question. (The project's Flips-based creator lives
outside this tree; this is the reader that matches it.)

Format, byte for byte
---------------------
    "BPS1"                 magic
    sourceSize             varint   the expected size of the source image
    targetSize             varint   the size of the image this patch produces
    metadataSize           varint
    metadata               metadataSize bytes (often empty; some patches carry a manifest)
    ...actions...          the instruction stream that rebuilds source -> target
    sourceCrc32            uint32 LE  CRC32 of the whole source image
    targetCrc32            uint32 LE  CRC32 of the whole patched image
    patchCrc32             uint32 LE  CRC32 of every byte above this field

A varint is BPS's own "number": seven data bits per byte, low group first, the top bit marking the
last byte. An action is one varint whose low two bits pick the kind and whose rest is a length
minus one:

    0 SourceRead   length bytes copied from the source at the SAME offset the output is at
    1 TargetRead   length bytes taken from the patch stream itself
    2 SourceCopy   a signed offset added to a source-relative pointer, then length bytes copied
    3 TargetCopy   a signed offset added to a target-relative pointer, then length bytes copied

Memory behaviour (deliberate, and stated in the UI)
---------------------------------------------------
A Digimon World 3 disc image is ~647 MB, so the source is never loaded into RAM: it is opened
read-only and read in bounded (8 MiB) chunks, both for its CRC32 and for every SourceRead /
SourceCopy. The output is created at its final size and memory-mapped for writing, because TargetCopy
has to read back bytes already written; the patch itself is memory-mapped too (a .bps is small). The
only buffers that scale are bounded by the 8 MiB copy chunk, so the Python heap stays flat.

Honest caveat on the OUTPUT: mapping the output file means the OS charges its pages to this process
as file-backed pages while it is dirty, so the process's peak working set can approach the size of
the patched image (~650 MB for a 647 MB disc), even though that memory is reclaimable file-backed
pages rather than a Python copy of the image. That is reported in the card's log line too, so the
figure is never a surprise. The source is opened read-only and is never written to, and the output is
refused if it would be the source file itself, so a player's original disc image cannot be damaged
by this code.

The single 597 MB SourceRead in this project's own patch is exactly why a pure-Python decoder
matters: a JavaScript patcher computes `((length - 1) << 2)` in 32-bit arithmetic and overflows on
an action that large. Python integers are arbitrary precision, so this decoder does not care how
big an action is - only the chunked copies keep the working set bounded.
"""
from __future__ import annotations

import mmap
import os
import pathlib
import re
import time
import zlib
from dataclasses import dataclass

MAGIC = b"BPS1"
SOURCE_READ, TARGET_READ, SOURCE_COPY, TARGET_COPY = 0, 1, 2, 3
TRAILER_SIZE = 12                       # sourceCrc32 + targetCrc32 + patchCrc32

# Every bulk copy moves at most this many bytes at once. It bounds the transient working set no
# matter how long a single action is (this project's patch has one 597 MB action), and it keeps the
# progress callback responsive.
COPY_CHUNK = 8 * 1024 * 1024
# How often a long copy refreshes the caller's progress, in bytes.
PROGRESS_STEP = 32 * 1024 * 1024

_CUE_FILE = re.compile(r'FILE\s+"([^"]+)"\s+BINARY', re.I)


class BpsError(Exception):
    """A patch that cannot be read, does not match the image, or cannot be applied.

    The message is player-facing: it says what is wrong and, where there is one, what to do.
    """


@dataclass(frozen=True)
class BpsHeader:
    """What a patch says about itself, read without applying it."""

    source_size: int
    target_size: int
    metadata: bytes = b""
    source_crc: int = 0
    target_crc: int = 0
    patch_crc: int = 0
    patch_size: int = 0
    action_offset: int = 0              # where the action stream starts (internal)

    @property
    def metadata_text(self) -> str:
        """The metadata as text, when a patch carries a readable manifest. '' when there is none."""
        return self.metadata.decode("utf-8", errors="replace").strip()


@dataclass
class BpsResult:
    """The outcome of an apply, as the UI reports it."""

    ok: bool
    message: str = ""
    source: pathlib.Path | None = None
    output: pathlib.Path | None = None
    source_crc: int = 0
    target_crc: int = 0
    bytes_written: int = 0
    elapsed: float = 0.0

    def summary(self) -> str:
        if not self.ok:
            return self.message
        where = self.output if self.output is not None else "?"
        return (f"{where} - patched and verified "
                f"(target CRC32 {self.target_crc:08X}, {self.bytes_written:,} bytes, "
                f"{self.elapsed:.1f}s).")


# ---------------------------------------------------------------- numbers and CRC32

class _Reader:
    """A cursor over the mapped patch, decoding BPS numbers and signed offsets."""

    __slots__ = ("data", "pos", "end")

    def __init__(self, data, start: int = 0, end: int | None = None) -> None:
        self.data = data
        self.pos = start
        self.end = len(data) if end is None else end

    def byte(self) -> int:
        if self.pos >= self.end:
            raise BpsError("The patch is truncated: it ends in the middle of an instruction.")
        b = self.data[self.pos]
        self.pos += 1
        return b

    def number(self) -> int:
        value = 0
        shift = 1
        while True:
            x = self.byte()
            value += (x & 0x7F) * shift
            if x & 0x80:
                return value
            shift <<= 7
            value += shift

    def signed(self) -> int:
        v = self.number()
        return -(v >> 1) if v & 1 else v >> 1


def _crc32(data, total: int, *, progress=None) -> int:
    """CRC32 (zlib) over the first `total` bytes of a bytes-like, in bounded chunks."""
    view = data if isinstance(data, memoryview) else memoryview(data)
    crc = 0
    off = 0
    while off < total:
        n = min(COPY_CHUNK, total - off)
        crc = zlib.crc32(view[off:off + n], crc)
        off += n
        if progress is not None:
            progress(off, total)
    return crc & 0xFFFFFFFF


def _crc32_stream(fh, total: int, *, progress=None) -> int:
    """CRC32 of a file read from position 0 in bounded chunks, without mapping it into memory."""
    fh.seek(0)
    crc = 0
    off = 0
    while off < total:
        data = fh.read(min(COPY_CHUNK, total - off))
        if not data:
            break
        crc = zlib.crc32(data, crc)
        off += len(data)
        if progress is not None:
            progress(off, total)
    return crc & 0xFFFFFFFF


def _read_view(fh, size: int):
    """A read-only view of an open file: a memory map when it has bytes, else b''."""
    if size <= 0:
        return b""
    return mmap.mmap(fh.fileno(), 0, access=mmap.ACCESS_READ)


def _close(view) -> None:
    try:
        if isinstance(view, mmap.mmap):
            view.close()
    except (ValueError, OSError):
        pass


# ---------------------------------------------------------------- the source side of a .cue

def resolve_base(image: pathlib.Path) -> pathlib.Path:
    """The raw image behind a disc file: the .bin itself, or the file a .cue points at.

    A patch attaches to the raw .bin; a playable disc is usually a .cue + .bin pair, so a player
    who picks the .cue must still hit the right bytes. A .cue that names no .bin, or names one that
    is not beside it, is reported rather than guessed at.
    """
    p = pathlib.Path(image)
    if p.suffix.lower() != ".cue":
        return p
    try:
        text = p.read_text(encoding="utf-8", errors="replace")
    except OSError as exc:
        raise BpsError(f"Could not read {p.name}: {exc}") from None
    m = _CUE_FILE.search(text)
    if not m:
        raise BpsError(f"{p.name} is a .cue that does not point at a .bin. Give the .bin itself, "
                       f"or a .cue that names the image next to it.")
    target = p.parent / m.group(1)
    if not target.is_file():
        raise BpsError(f'{p.name} points at "{m.group(1)}", but no such file is next to it.')
    return target


# ---------------------------------------------------------------- reading the header

def _parse_header(blob, patch_size: int) -> BpsHeader:
    if patch_size < 4 + 3 + TRAILER_SIZE or bytes(blob[:4]) != MAGIC:
        head = bytes(blob[:4]) if patch_size >= 4 else b""
        if patch_size < 12:
            raise BpsError(f"The patch is only {patch_size} bytes - far too small to be a BPS "
                           f"patch. It is not a .bps file, or it is truncated.")
        raise BpsError(f"This is not a BPS patch: it starts with {head!r}, not b'BPS1'.")
    r = _Reader(blob, 4)
    source_size = r.number()
    target_size = r.number()
    meta_size = r.number()
    if r.pos + meta_size + TRAILER_SIZE > patch_size:
        raise BpsError("The patch is truncated: its header says it carries "
                       f"{meta_size} bytes of metadata, but they do not fit in the file.")
    metadata = bytes(blob[r.pos:r.pos + meta_size])
    action_offset = r.pos + meta_size
    source_crc = int.from_bytes(blob[-12:-8], "little")
    target_crc = int.from_bytes(blob[-8:-4], "little")
    patch_crc = int.from_bytes(blob[-4:], "little")
    return BpsHeader(source_size=source_size, target_size=target_size, metadata=metadata,
                     source_crc=source_crc, target_crc=target_crc, patch_crc=patch_crc,
                     patch_size=patch_size, action_offset=action_offset)


def read_header(patch_path: pathlib.Path) -> BpsHeader:
    """Read a patch's header (sizes, metadata, checksums) without applying it."""
    path = pathlib.Path(patch_path)
    if not path.is_file():
        raise BpsError(f"No such patch file: {path}")
    size = path.stat().st_size
    if size == 0:
        raise BpsError(f"{path.name} is empty. It is not a BPS patch.")
    fh = open(path, "rb")
    try:
        blob = _read_view(fh, size)
        try:
            return _parse_header(blob, size)
        finally:
            _close(blob)
    finally:
        fh.close()


# ---------------------------------------------------------------- the output path

def default_output(base_path: pathlib.Path) -> pathlib.Path:
    """A free output path beside the base image that never collides with an existing file."""
    base = resolve_base(base_path)
    for n in range(1, 1000):
        suffix = " (patched)" if n == 1 else f" (patched-{n})"
        cand = base.with_name(base.stem + suffix + ".bin")
        if not cand.exists():
            return cand
    return base.with_name(base.stem + " (patched).bin")


# ---------------------------------------------------------------- applying

def _copy(src_view, src_start: int, out_view, out_start: int, length: int,
          *, tick=None, chunk_limit: int | None = None) -> None:
    """Copy `length` bytes in bounded chunks, honouring overlapping target copies.

    `chunk_limit` caps the chunk so a TargetCopy never reads bytes it has not written yet: with the
    copy reading forward from an earlier offset, a chunk no larger than the distance between the
    source and destination offsets guarantees every byte read is already final.
    """
    chunk = COPY_CHUNK
    if chunk_limit is not None:
        chunk = max(1, min(chunk, chunk_limit))
    off = 0
    while off < length:
        step = min(chunk, length - off)
        out_view[out_start + off:out_start + off + step] = \
            src_view[src_start + off:src_start + off + step]
        off += step
        if tick is not None:
            tick(step)


def _copy_from_file(src_fh, out_view, src_start: int, out_start: int, length: int,
                    *, tick=None) -> None:
    """Copy `length` bytes from a source FILE into the output view, in bounded chunks.

    The source is streamed rather than mapped: only an 8 MiB read buffer is ever in the Python
    heap, whatever the image size.
    """
    src_fh.seek(src_start)
    off = 0
    while off < length:
        step = min(COPY_CHUNK, length - off)
        data = src_fh.read(step)
        if len(data) != step:
            raise BpsError("The source image ended early while the patch was reading it.")
        out_view[out_start + off:out_start + off + step] = data
        off += step
        if tick is not None:
            tick(step)


def apply(patch_path: pathlib.Path, base_path: pathlib.Path, out_path: pathlib.Path,
          *, log=None, progress=None, overwrite: bool = False) -> BpsResult:
    """Apply a BPS patch to a copy of `base_path`, writing `out_path`.

    Order of operations, which is the point of the safety:

      1. the patch's own CRC32 is checked, so a damaged patch is rejected before anything else;
      2. the base image's size and CRC32 are checked against the header BEFORE any output exists -
         a mismatch is Flips' "This patch is not intended for this ROM", and nothing is written;
      3. the target image is written by memory-mapping the output file;
      4. the target's CRC32 is checked, and on mismatch the output is DELETED so no bad image is
         left looking like the finished one.

    `base_path` may be a .cue; the .bin it names is used. The base file is opened read-only and is
    never modified; `out_path` is refused when it would be the base file itself. `log` receives one
    line at a time; `progress(done, total)` is called during the long copies. Both are optional.
    """
    started = time.monotonic()

    def say(text: str) -> None:
        if log is not None:
            log(text)

    patch_path = pathlib.Path(patch_path)
    out_path = pathlib.Path(out_path)
    base = resolve_base(base_path)

    if not patch_path.is_file():
        raise BpsError(f"No such patch file: {patch_path}")
    if not base.is_file():
        raise BpsError(f"No such disc image: {base}")

    # 1. parse and verify the patch itself (its own CRC32 covers every preceding byte).
    patch_size = patch_path.stat().st_size
    patch_fh = open(patch_path, "rb")
    try:
        patch_view = _read_view(patch_fh, patch_size)
        header = _parse_header(patch_view, patch_size)
        patch_crc = _crc32(patch_view, patch_size - 4)
        if patch_crc != header.patch_crc:
            raise BpsError(
                f"{patch_path.name} is damaged: its own checksum does not match "
                f"(the header says {header.patch_crc:08X}, the file is {patch_crc:08X}). "
                f"Re-copy the patch file.")
        detail = f", metadata {len(header.metadata)} B" if header.metadata else ""
        say(f"patch {patch_path.name}: source {header.source_size:,} B, "
            f"target {header.target_size:,} B{detail}")
        if header.metadata_text:
            say(f"   metadata: {header.metadata_text[:120]}")

        # 2. the base image, checked BEFORE anything is written.
        base_size = base.stat().st_size
        if base_size != header.source_size:
            raise BpsError(
                f"This patch is not intended for this ROM: it expects a source image of "
                f"{header.source_size:,} bytes, but {base.name} is {base_size:,} bytes.")
        src_fh = open(base, "rb")
        try:
            say(f"checking {base.name}'s CRC32 ({base_size:,} bytes) before writing anything...")
            got = _crc32_stream(src_fh, base_size)
            if got != header.source_crc:
                raise BpsError(
                    f"This patch is not intended for this ROM: {base.name} does not match the "
                    f"patch's source checksum (expected {header.source_crc:08X}, found "
                    f"{got:08X}). Use your own retail image, unmodified.")
            say(f"   source CRC32 {got:08X} matches; writing the patched image...")
            say(f"   (the source is streamed; the output is written through a memory map, so the "
                f"process's peak RAM tracks the image size, about {header.target_size:,} bytes, "
                f"and is reclaimable)")

            # 3. output. Refuse to touch the base image or an existing file (unless overwrite).
            if _same_file(out_path, base):
                raise BpsError(f"The output would overwrite your disc image ({base.name}). "
                               f"Choose a different output file.")
            if out_path.exists() and not overwrite:
                raise BpsError(f"{out_path} already exists. Choose a different output file, or "
                               f"let the launcher pick a free name.")
            out_path.parent.mkdir(parents=True, exist_ok=True)
            created = False
            out_fh = out_vview = None
            failure: BpsError | None = None
            try:
                # "w+b", not "wb": the output is memory-mapped for writing, and Windows refuses a
                # PAGE_READWRITE mapping of a file opened write-only (it needs read access too).
                out_fh = open(out_path, "w+b")
                created = True
                out_fh.truncate(header.target_size)
                out_vview = (mmap.mmap(out_fh.fileno(), 0, access=mmap.ACCESS_WRITE)
                             if header.target_size > 0 else b"")
                _decode(header, patch_view, src_fh, out_vview, progress)
                if isinstance(out_vview, mmap.mmap):
                    out_vview.flush()
            except BpsError as exc:
                failure = exc
            except (OSError, ValueError, MemoryError) as exc:
                failure = BpsError(
                    f"Could not write {out_path.name}: {exc}. If the disk is full, free space and "
                    f"try again.")
            finally:
                # Close the mapping and the handle FIRST: on Windows a file cannot be removed while
                # this process still holds it open, so discarding a half-written output only works
                # after the close.
                _close(out_vview)
                if out_fh is not None:
                    out_fh.close()
            if failure is not None:
                _discard(out_path, created)
                raise failure

            # 4. the finished image, checked before it is handed over.
            tgt_crc = _crc32_file(out_path)
            if tgt_crc != header.target_crc:
                try:
                    out_path.unlink()
                except OSError:
                    pass
                raise BpsError(
                    f"The patched image failed its target checksum (expected "
                    f"{header.target_crc:08X}, found {tgt_crc:08X}), so it was deleted. The "
                    f"source image was not modified.")
            say(f"   target CRC32 {tgt_crc:08X} verified.")
            return BpsResult(ok=True, source=base, output=out_path,
                             source_crc=header.source_crc, target_crc=tgt_crc,
                             bytes_written=header.target_size,
                             elapsed=time.monotonic() - started)
        finally:
            src_fh.close()
    finally:
        _close(patch_view)
        patch_fh.close()


def _same_file(a: pathlib.Path, b: pathlib.Path) -> bool:
    """True when two paths name the same file (or would, once created)."""
    try:
        if a.exists() and b.exists():
            return os.path.samefile(a, b)
    except OSError:
        pass
    try:
        return a.resolve() == b.resolve()
    except OSError:
        return False


def _discard(path: pathlib.Path, created: bool) -> None:
    """Remove a half-written output we created, so a failure leaves nothing behind."""
    if not created:
        return
    try:
        path.unlink()
    except OSError:
        pass


def _crc32_file(path: pathlib.Path) -> int:
    size = path.stat().st_size
    if size == 0:
        return 0
    with open(path, "rb") as fh:
        return _crc32_stream(fh, size)


def _decode(header: BpsHeader, patch_view, src_fh, out_view, progress) -> int:
    """Run the action stream, writing the rebuilt image into `out_view`. Returns bytes written.

    `src_fh` is the source image opened read-only; it is read in bounded chunks rather than mapped,
    so the source never sits in the Python heap.
    """
    end = header.patch_size - TRAILER_SIZE
    r = _Reader(patch_view, header.action_offset, end)
    out_off = 0
    src_rel = 0
    tgt_rel = 0
    total = header.target_size
    reported = 0

    def tick_from(base: int):
        """Report progress while a long copy runs; `base` is the write offset it starts at."""
        def tick(added: int) -> None:
            nonlocal reported
            done = base + added
            if progress is not None and done - reported >= PROGRESS_STEP:
                reported = done
                progress(done, total)
        return tick

    while r.pos < end:
        data = r.number()
        length = (data >> 2) + 1
        kind = data & 3
        before = out_off

        if kind == SOURCE_READ:
            # SourceRead takes the source bytes aligned with the output position: with no separate
            # source cursor, the action copies source[out_off : out_off + length].
            if out_off + length > header.source_size:
                raise BpsError("The patch is invalid: a SourceRead runs past the end of the "
                               "source image.")
            _copy_from_file(src_fh, out_view, out_off, out_off, length, tick=tick_from(before))
        elif kind == TARGET_READ:
            if r.pos + length > end:
                raise BpsError("The patch is truncated: a TargetRead runs past the end of the "
                               "patch data.")
            _copy(patch_view, r.pos, out_view, out_off, length, tick=tick_from(before))
            r.pos += length
        elif kind == SOURCE_COPY:
            src_rel += r.signed()
            if src_rel < 0 or src_rel + length > header.source_size:
                raise BpsError("The patch is invalid: a SourceCopy reads outside the source image.")
            _copy_from_file(src_fh, out_view, src_rel, out_off, length, tick=tick_from(before))
            src_rel += length
        else:  # TARGET_COPY
            tgt_rel += r.signed()
            if tgt_rel < 0 or tgt_rel >= out_off:
                raise BpsError("The patch is invalid: a TargetCopy reads data that has not been "
                               "written yet.")
            _copy(out_view, tgt_rel, out_view, out_off, length, tick=tick_from(before),
                  chunk_limit=out_off - tgt_rel)
            tgt_rel += length

        out_off = before + length
        if out_off > total:
            raise BpsError("The patch is invalid: it writes more bytes than the target size it "
                           "declares.")
        if progress is not None and out_off - reported >= PROGRESS_STEP:
            reported = out_off
            progress(out_off, total)

    if out_off != total:
        raise BpsError(f"The patch is invalid: it produced {out_off:,} bytes but declares a "
                       f"target of {total:,}.")
    if progress is not None:
        progress(total, total)
    return out_off
