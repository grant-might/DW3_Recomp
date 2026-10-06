# Portability and licensing

This repository is the public copy of the DW3 Recompiled+ launcher and its tooling. It is
published so people can read the code, run the launcher against their own copy of the game, and
send improvements back. It is a code and tooling release only.

## What this repository may contain

- The launcher source and its documentation.
- Launcher artwork and placeholder assets made for this project.
- Configuration templates, tests and build scripts.
- License and attribution files for this project and for third party components.

## What it must never contain

- Game code. No extracted or decompiled game programs, and no recompiler output generated from
  the game.
- Disc images. No .bin, .cue, .iso, .chd or any other copy of the game disc.
- Retail BIOS files. No SCPH*.BIN or any other dumped retail BIOS image.

If any of these are ever committed, they must be removed before the tree is published.

## The BIOS

The only BIOS image this project is allowed to ship is OpenBIOS, the free PS1 BIOS from the
PCSX-Redux project, which is released under the MIT license. Its notice is kept in
OpenBIOS.LICENSE beside this file, and OpenBIOS is credited in THIRD_PARTY_ATTRIBUTION.md.
Retail BIOS dumps are not shipped here.

## The game disc

Players must supply their own Digimon World 3 disc. The launcher locates the player's own disc
image, checks it, and builds a working tree from it on their machine. The disc itself is never
part of this repository.

## Licensing of this project

This project is released under the PolyForm Noncommercial License 1.0.0 with additional terms.
The full text is in LICENSE.md. It is free to use, change and share for any noncommercial
purpose. Commercial use needs a license from the author, changes should come back to the
project, and the project name and marks stay with the author.

Third party components and their licenses are listed in THIRD_PARTY_ATTRIBUTION.md.
