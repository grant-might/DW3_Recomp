#!/usr/bin/env python3
"""Writes a built executable and overlays into a copy of the disc image.

    VERSION=<version> patch_disc.py [--fit] IMAGE LINKDIR OUT

Copies the original disc image IMAGE (a raw .bin of 2352-byte sectors) to
OUT/disc.bin, writes over it the executable and the AAA/PRO/*.PRO files that
LINKDIR (build/<version>, or a padding build's build/<version>/pad<pad>)
holds, and writes OUT/disc.cue for it. The files are written in place, so
each one must keep its size, as a padding build keeps every section's: the
rest of the disc (the filesystem, the data, the XA audio and the videos)
stays as it is. With --fit, for a build with another toolchain
(build/<version>/gcc), a file may be smaller, and is padded with zeros to
its size on the disc: the game loads each of its files as the number of
sectors that its file table gives, the original's (getFileSectorCount,
src/engine/file/file_table.c), and the BIOS the executable's .text size from
its header. Every sector written gets its Mode 2 Form 1 EDC and ECC again.
Writing the build itself back gives the original image, byte for byte. OUT
must be under the repository's build/, so that the copy of the disc never
ends up anywhere else.

The sector layout, the EDC and the ECC follow psx-spx
(https://psx-spx.consoledev.net/ps1/cdr/cdromformat/#cdrom-sector-encoding),
and so does the ISO 9660 filesystem
(https://psx-spx.consoledev.net/ps1/cdr/cdromformat/#primary-volume-descriptor-sector-16-on-psx-disks).
"""
import argparse
import os
import shutil
import struct
import sys
from pathlib import Path

from version import EXE_NAME, ROOT

# a raw sector: sync, header, subheader, data, EDC, ECC
# (https://psx-spx.consoledev.net/ps1/cdr/cdromformat/#930h-byte-sectors)
SECTOR = 2352
SYNC = 12
HEADER = 4
SUBHEADER = 8
USER_DATA = SYNC + HEADER + SUBHEADER
DATA_SIZE = 2048
EDC_START = SYNC + HEADER  # the EDC covers the subheader and the data
EDC_END = USER_DATA + DATA_SIZE
SUBMODE = SYNC + HEADER + 2
SUBMODE_FORM2 = 0x20

# the EDC: a CRC-32 with the polynomial 0xD8018001, reflected
EDC_TABLE = []
for _i in range(256):
    _edc = _i
    for _ in range(8):
        _edc = (_edc >> 1) ^ (0xD8018001 if _edc & 1 else 0)
    EDC_TABLE.append(_edc)

# the ECC: Reed-Solomon products over GF(2^8) with the polynomial 0x11D
ECC_F = [0] * 256
ECC_B = [0] * 256
for _i in range(256):
    _j = ((_i << 1) ^ (0x11D if _i & 0x80 else 0)) & 0xFF
    ECC_F[_i] = _j
    ECC_B[_i ^ _j] = _i


def edc(data):
    crc = 0
    for byte in data:
        crc = (crc >> 8) ^ EDC_TABLE[(crc ^ byte) & 0xFF]
    return crc


def ecc_block(src, major_count, minor_count, major_mult, minor_inc):
    """One ECC part (P or Q) of the bytes from the header on."""
    size = major_count * minor_count
    out = bytearray(2 * major_count)
    for major in range(major_count):
        index = (major >> 1) * major_mult + (major & 1)
        ecc_a = ecc_b = 0
        for _ in range(minor_count):
            byte = src[index]
            index += minor_inc
            if index >= size:
                index -= size
            ecc_a = ECC_F[ecc_a ^ byte]
            ecc_b ^= byte
        ecc_a = ECC_B[ECC_F[ecc_a] ^ ecc_b]
        out[major] = ecc_a
        out[major + major_count] = ecc_a ^ ecc_b
    return out


def encode(sector):
    """Fills in the EDC and the ECC of a Mode 2 Form 1 sector."""
    sector[EDC_END : EDC_END + 4] = struct.pack("<I", edc(sector[EDC_START:EDC_END]))
    # Mode 2 computes the ECC as if the header were zero
    src = bytearray(4) + sector[SYNC + HEADER : SECTOR]
    p = ecc_block(src, 86, 24, 2, 86)
    sector[EDC_END + 4 : EDC_END + 4 + len(p)] = p
    src[2076 - SYNC : 2076 - SYNC + len(p)] = p
    q = ecc_block(src, 52, 43, 86, 88)
    sector[SECTOR - len(q) : SECTOR] = q


def files(image):
    """The disc's files, {path: (lba, length)}, from every directory that the
    path table lists (tools/extract_disc.py)."""
    def sector(lba):
        image.seek(lba * SECTOR + USER_DATA)
        return image.read(DATA_SIZE)

    pvd = sector(16)
    pt_size = struct.unpack("<I", pvd[132:136])[0]
    pt_lba = struct.unpack("<I", pvd[140:144])[0]
    pt = b"".join(sector(pt_lba + i) for i in range((pt_size + DATA_SIZE - 1) // DATA_SIZE))[:pt_size]
    dirs = []
    i = 0
    while i < len(pt):
        name_len = pt[i]
        extent = struct.unpack("<I", pt[i + 2 : i + 6])[0]
        parent = struct.unpack("<H", pt[i + 6 : i + 8])[0]
        dirs.append((pt[i + 8 : i + 8 + name_len].decode("ascii"), extent, parent))
        i += 8 + name_len + (name_len & 1)

    def path(k):
        name, _, parent = dirs[k]
        return "" if k == 0 else os.path.join(path(parent - 1), name)

    found = {}
    for k, (_, extent, _) in enumerate(dirs):
        size = struct.unpack("<I", sector(extent)[10:14])[0]  # the "." record
        data = b"".join(sector(extent + j) for j in range((size + DATA_SIZE - 1) // DATA_SIZE))
        j = 0
        while j < len(data):
            rec_len = data[j]
            if rec_len == 0:
                j = (j // DATA_SIZE + 1) * DATA_SIZE
                continue
            rec = data[j : j + rec_len]
            j += rec_len
            name = rec[33 : 33 + rec[32]]
            if rec[25] & 2 or name in (b"\x00", b"\x01"):
                continue
            lba = struct.unpack("<I", rec[2:6])[0]
            length = struct.unpack("<I", rec[10:14])[0]
            found[os.path.join(path(k), name.decode("ascii").split(";")[0])] = (lba, length)
    return found


def patch(image, linkdir, out, fit=False):
    """Writes OUT/disc.bin and OUT/disc.cue, and returns the image's path;
    with FIT, a file smaller than the disc's is padded to its size."""
    out = Path(out).resolve()
    if not out.is_relative_to(ROOT / "build"):
        sys.exit(f"{out} isn't under {ROOT / 'build'}")
    built = [EXE_NAME] + sorted(f"AAA/PRO/{p.name}" for p in (linkdir / "AAA" / "PRO").glob("*.PRO"))

    with open(image, "rb") as f:
        on_disc = files(f)
    missing = [p for p in built if p not in on_disc]
    if missing:
        sys.exit(f"not on the disc: {' '.join(missing)}")

    out.mkdir(parents=True, exist_ok=True)
    disc = out / "disc.bin"
    shutil.copyfile(image, disc)
    with open(disc, "r+b") as f:
        for name in built:
            data = (linkdir / name).read_bytes()
            lba, length = on_disc[name]
            if len(data) != length and not (fit and len(data) < length):
                sys.exit(f"{name}: {len(data):#x} bytes, {length:#x} on the disc")
            data += bytes(length - len(data))
            for i in range(0, length, DATA_SIZE):
                f.seek((lba + i // DATA_SIZE) * SECTOR)
                sector = bytearray(f.read(SECTOR))
                if sector[SUBMODE] & SUBMODE_FORM2:
                    sys.exit(f"{name}: sector {lba + i // DATA_SIZE} isn't Form 1")
                chunk = data[i : i + DATA_SIZE]
                sector[USER_DATA : USER_DATA + len(chunk)] = chunk
                encode(sector)
                f.seek(-SECTOR, os.SEEK_CUR)
                f.write(sector)
    (out / "disc.cue").write_text('FILE "disc.bin" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
    print(f"{len(built)} files written into {disc}")
    return disc


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--fit", action="store_true", help="pad a smaller file to its size on the disc")
    parser.add_argument("image", type=Path)
    parser.add_argument("linkdir", type=Path)
    parser.add_argument("out", type=Path)
    args = parser.parse_args()
    patch(args.image, args.linkdir, args.out, args.fit)


if __name__ == "__main__":
    main()
