#!/usr/bin/env python3
"""Assemble the end-user `overlay_toolchain/` bundle beside a game executable.

WHY
---
PS1 games stream *overlays* — chunks of code loaded from disc into RAM at
runtime. The static recompiler never saw them, so on first visit they run on the
dirty-RAM interpreter (correct, but slow). The runtime's `tcc` backend tier
compiles those gaps into native shards, but only if it finds a self-contained
toolchain beside the exe; otherwise every gap falls to the interpreter, which
stalls the frame timer for ~1 second at a time (the watchdog writes
`psx_freeze_dump_*.json`).

`runtime/src/main.cpp` probes exactly:

    <exe_dir>/overlay_toolchain/python/python.exe

and, when present, spawns

    <exe_dir>/overlay_toolchain/python/python.exe \
        <exe_dir>/overlay_toolchain/compile_overlays.py \
        --captures <...> --game-toml <...> \
        --recompiler <exe_dir>/overlay_toolchain/psxrecomp-game.exe \
        --runtime-include <exe_dir>/overlay_toolchain/include \
        --out-dir <exe_dir>/cache [--cps] \
        --compiler tcc --tcc <exe_dir>/overlay_toolchain/tcc/tcc.exe

so this script lays out exactly that tree:

    <exe_dir>/overlay_toolchain/
        python/                  embedded CPython           (PSF-2.0)
        tcc/                     TinyCC 0.9.27 + libtcc.dll + include/ + lib/  (LGPL-2.1)
        compile_overlays.py      tools/compile_overlays.py
        psxrecomp-game.exe       recompiler built from THIS source tree
        include/                 runtime/include
        THIRD_PARTY_NOTICES.txt  licence notices + SHA-256 of every input
        README.txt               what the bundle is

LICENCES (both permit redistribution; the notices are shipped alongside):
  * TinyCC 0.9.27 — LGPL-2.1. Invoked as a SEPARATE subprocess; nothing in the
    runtime links libtcc, so this is aggregation, not LGPL linkage. The full
    licence text is copied to tcc/COPYING and a written source offer is included.
  * CPython 3 — PSF-2.0 (permissive). Its LICENSE.txt rides along.

Nothing is downloaded here: pass the upstream archives or already-extracted
directories you have. Every input's SHA-256 is recorded in the notice so a
release can be audited against a known-good artefact.
"""

from __future__ import annotations

import argparse
import hashlib
import shutil
import subprocess
import sys
import tempfile
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

TCC_VERSION = "0.9.27"
TCC_URL = "https://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27-win64-bin.zip"
PYTHON_URL = "https://www.python.org/ftp/python/3.12.10/python-3.12.10-embed-amd64.zip"


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def extract_zip(src: Path, dest: Path) -> None:
    dest.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(src) as z:
        z.extractall(dest)


def find_one(root: Path, name: str) -> Path | None:
    for p in root.rglob(name):
        return p
    return None


def probe(cmd: list[str]) -> str:
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    except (OSError, subprocess.SubprocessError) as exc:
        raise SystemExit(f"overlay_toolchain: probe failed ({cmd[0]}): {exc}")
    out = ((r.stdout or "") + (r.stderr or "")).strip().splitlines()
    return out[0] if out else ""


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe-dir", required=True,
                    help="directory containing the game executable; "
                         "overlay_toolchain/ is created inside it")
    ap.add_argument("--tcc-zip", help="TinyCC win64 binary zip (contains tcc/)")
    ap.add_argument("--tcc-dir", help="already-extracted TinyCC directory (contains tcc.exe)")
    ap.add_argument("--python-zip", help="CPython embeddable amd64 zip")
    ap.add_argument("--python-dir", help="already-extracted embedded CPython directory")
    ap.add_argument("--recompiler", default=str(ROOT / "recompiler" / "build-cli" / "psxrecomp-game.exe"),
                    help="psxrecomp-game built from this source tree (default: "
                         "recompiler/build-cli/psxrecomp-game.exe)")
    ap.add_argument("--runtime-include", default=str(ROOT / "runtime" / "include"),
                    help="the framework's runtime/include to copy (default: this "
                         "tree's). Pass the include dir of the tree the GAME was "
                         "built against, so the generated overlay_codegen_hash.h "
                         "inside it matches the exe's compiled-in hash")
    ap.add_argument("--licenses-dir", default=str(ROOT / "packaging" / "licenses"))
    ap.add_argument("--force", action="store_true", help="replace an existing bundle")
    args = ap.parse_args()

    exe_dir = Path(args.exe_dir).resolve()
    if not exe_dir.is_dir():
        raise SystemExit(f"overlay_toolchain: --exe-dir {exe_dir} is not a directory")
    recompiler = Path(args.recompiler).resolve()
    if not recompiler.is_file():
        raise SystemExit(f"overlay_toolchain: recompiler not found: {recompiler}\n"
                         f"  build it first:  cmake --build recompiler/build-cli --target psxrecomp-game")
    runtime_include = Path(args.runtime_include).resolve()
    if not runtime_include.is_dir():
        raise SystemExit(f"overlay_toolchain: runtime include dir not found: {runtime_include}")

    tcc_src = Path(args.tcc_dir).resolve() if args.tcc_dir else None
    py_src = Path(args.python_dir).resolve() if args.python_dir else None
    tmp = None
    if tcc_src is None:
        if not args.tcc_zip:
            raise SystemExit("overlay_toolchain: pass --tcc-zip or --tcc-dir")
        tmp = Path(tempfile.mkdtemp(prefix="ovltk-"))
        extract_zip(Path(args.tcc_zip).resolve(), tmp / "tcc")
        tcc_src = tmp / "tcc"
    if py_src is None:
        if not args.python_zip:
            raise SystemExit("overlay_toolchain: pass --python-zip or --python-dir")
        if tmp is None:
            tmp = Path(tempfile.mkdtemp(prefix="ovltk-"))
        extract_zip(Path(args.python_zip).resolve(), tmp / "python")
        py_src = tmp / "python"

    tcc_exe = find_one(tcc_src, "tcc.exe")
    py_exe = find_one(py_src, "python.exe")
    if tcc_exe is None:
        raise SystemExit(f"overlay_toolchain: tcc.exe not found under {tcc_src}")
    if py_exe is None:
        raise SystemExit(f"overlay_toolchain: python.exe not found under {py_src}")

    # The package may be nested (tcc/tcc.exe, tcc/tcc/tcc.exe); the runtime wants
    # the directory that directly holds tcc.exe and its sibling include/ + lib/.
    tcc_root = tcc_exe.parent
    py_root = py_exe.parent

    dest = exe_dir / "overlay_toolchain"
    if dest.exists():
        if not args.force:
            raise SystemExit(f"overlay_toolchain: {dest} already exists (use --force)")
        shutil.rmtree(dest)
    dest.mkdir(parents=True)

    shutil.copytree(tcc_root, dest / "tcc")
    shutil.copytree(py_root, dest / "python")
    shutil.copy2(ROOT / "tools" / "compile_overlays.py", dest / "compile_overlays.py")
    shutil.copy2(recompiler, dest / "psxrecomp-game.exe")
    shutil.copytree(runtime_include, dest / "include")

    # Licence texts: the LGPL-2.1 text tracked in-repo for TinyCC, and CPython's
    # own LICENSE.txt straight out of the embedded distribution.
    tcc_copying = Path(args.licenses_dir).resolve() / "LGPL-2.1.txt"
    if tcc_copying.is_file():
        shutil.copy2(tcc_copying, dest / "tcc" / "COPYING")
    py_license = py_root / "LICENSE.txt"
    if py_license.is_file():
        shutil.copy2(py_license, dest / "python" / "LICENSE.txt")

    # Verify the copied tools actually run before declaring success.
    tcc_ver = probe([str(dest / "tcc" / "tcc.exe"), "-v"])
    py_ver = probe([str(dest / "python" / "python.exe"), "--version"])

    inputs = [("recompiler", recompiler)]
    if args.tcc_zip:
        inputs.append(("tcc-archive", Path(args.tcc_zip).resolve()))
    if args.python_zip:
        inputs.append(("python-archive", Path(args.python_zip).resolve()))

    notice = [
        "overlay_toolchain — third-party notices",
        "=======================================",
        "",
        "This bundle lets the psxrecomp runtime compile dynamically-loaded",
        "overlay code on a player's machine that has no C toolchain installed.",
        "Both bundled programs are invoked as separate subprocesses; nothing in",
        "the runtime links against them.",
        "",
        f"TinyCC {TCC_VERSION} — tcc/tcc.exe, tcc/libtcc.dll, tcc/include, tcc/lib",
        "  Copyright (C) 2001-2017 Fabrice Bellard and contributors.",
        "  Licensed under the GNU Lesser General Public License, version 2.1.",
        f"  Full licence text: tcc/COPYING (canonical: {TCC_URL.rsplit('/',1)[0]}/)",
        f"  Upstream source:   https://repo.or.cz/tinycc.git  (tag release_{TCC_VERSION})",
        "  The Corresponding Source for the bundled binaries is published upstream at",
        "  the repository above; this offer is written notice of its availability.",
        "",
        "CPython 3 — python/",
        "  Copyright (c) Python Software Foundation. Licensed under the PSF",
        "  License Agreement (PSF-2.0), a permissive licence: python/LICENSE.txt.",
        "  Upstream: " + PYTHON_URL.rsplit('/', 1)[0] + "/",
        "",
        "Input artefacts (SHA-256):",
    ]
    for label, path in inputs:
        notice.append(f"  {label:16s} {sha256(path)}  {path.name}")
    notice.append("")
    (dest / "THIRD_PARTY_NOTICES.txt").write_text("\n".join(notice) + "\n", encoding="utf-8")

    (dest / "README.txt").write_text(
        "overlay_toolchain — bundled overlay compiler\n"
        "============================================\n\n"
        "Assembled by tools/package_overlay_toolchain.py from the psxrecomp\n"
        "source tree. The runtime probes python/python.exe here at startup; when\n"
        "it exists the tcc backend tier compiles overlay gaps into native shards\n"
        "beside the exe (cache/<game-id>/tcc/...) instead of leaving them on the\n"
        "dirty-RAM interpreter. Delete this directory to fall back to the\n"
        "interpreter (slower, with ~1 s frame stalls on first visit).\n\n"
        "Licences: see THIRD_PARTY_NOTICES.txt (TinyCC: LGPL-2.1, text in\n"
        "tcc/COPYING; CPython: PSF-2.0, text in python/LICENSE.txt).\n",
        encoding="utf-8")

    if tmp is not None:
        shutil.rmtree(tmp, ignore_errors=True)

    print(f"overlay_toolchain: wrote {dest}")
    print(f"  tcc   : {dest / 'tcc' / 'tcc.exe'}  [{tcc_ver}]")
    print(f"  python: {dest / 'python' / 'python.exe'}  [{py_ver}]")
    for label, path in inputs:
        print(f"  {label}: sha256={sha256(path)} ({path.name})")
    print("  licences: TCC=LGPL-2.1 (tcc/COPYING), CPython=PSF-2.0 (python/LICENSE.txt)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
