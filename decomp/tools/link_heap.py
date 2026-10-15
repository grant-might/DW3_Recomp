#!/usr/bin/env python3
"""Write where the heap begins, for the executable's link.

    link_heap.py OUT ELF... [--disc DIR]

The heap (initHeap, src/engine/system/heap.c) runs from HEAP_START to 0x801FF000,
above every overlay. The CD reader loads a file in whole 2 KB sectors
(cdReadyCallback, src/engine/file/cd_reader.c), so an overlay takes its size rounded
up to 2 KB from its address. OUT assigns OVERLAYS_END, the highest end of the
overlays (the ELFs) so loaded, and HEAP_BASE, that rounded up to 2 KB, which
gives the original's HEAP_START (0x800AA800 in the USA version, 0x800AB800 in
the European one): src/engine/data/heap_start.c points HEAP_START at it, so that
the heap moves with the overlays in a padding build.

The CD reader loads as many sectors as the executable's file table gives
(getFileSectorCount, src/engine/file/file_table.c), the original files'.
With --disc, the extracted disc (disks/<version>), an overlay is loaded
that far too, for a build whose files may be smaller than the original's
(make TOOLCHAIN=gcc).
"""
import argparse
from pathlib import Path

from elftools.elf.elffile import ELFFile

SECTOR = 0x800


def loaded_end(path, disc=None):
    """Where the CD reader stops writing the overlay."""
    with open(path, "rb") as f:
        sections = [s for s in ELFFile(f).iter_sections() if s["sh_flags"] & 2 and s["sh_size"]]
    start = min(s["sh_addr"] for s in sections)
    end = max(s["sh_addr"] + s["sh_size"] for s in sections)
    if disc:
        end = max(end, start + (disc / "AAA" / "PRO" / f"{path.stem.upper()}.PRO").stat().st_size)
    return start + (end - start + SECTOR - 1) // SECTOR * SECTOR


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("out")
    ap.add_argument("elfs", nargs="+", type=Path)
    ap.add_argument("--disc", type=Path, help="the extracted disc, whose overlays' sizes the CD reader loads")
    args = ap.parse_args()
    end, highest = max((loaded_end(path, args.disc), path.stem) for path in args.elfs)
    with open(args.out, "w") as f:
        f.write("/* %s's end, in whole sectors (tools/link_heap.py) */\n" % highest.upper())
        f.write("OVERLAYS_END = 0x%08X;\n" % end)
        f.write("HEAP_BASE = ALIGN(OVERLAYS_END, 0x%X);\n" % SECTOR)


if __name__ == "__main__":
    main()
