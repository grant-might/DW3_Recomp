# DW3 Recompiled+

A Windows launcher and local builder for native recompilations of **Digimon World 3** and
**Digimon World 2003**.

It ships **no game code and no game data**. You point it at your own disc image, it recompiles that
disc on your own machine with the engine it bundles, and then it runs the result. Both regions are
supported: Digimon World 3 (NTSC-U, `SLUS-01436`) and Digimon World 2003 (PAL, `SLES-03936`).
Nothing is refused for its region.

## Screenshots

![The Play tab, with a disc image routed to its region](https://github.com/user-attachments/assets/44e1c7db-240e-4f75-ad9a-ff475aa28ee6)

The Play tab is the builder. Point it at your own disc image and it names the region from the
disc's own serial, then recompiles it into `Builds/USA` or `Builds/EUR`.

![The Memory Card tab, listing the cards it found](https://github.com/user-attachments/assets/75d322ad-5aed-408e-8e27-61f2f8330d4a)

The Memory Card tab lists every card it finds, including each build's own `card1.mcd` and
`card2.mcd`, and opens one in the embedded save editor.

![The Decompilation tab browsing the decompilation tree](https://github.com/user-attachments/assets/3737e81b-66d2-4812-9cf5-aab402840a37)

The Decompilation tab is a read-only browser over the decompilation tree, with the project's
published progress shown above it.

![The Settings tab](https://github.com/user-attachments/assets/83926aa5-98c7-42e5-b423-fcb6215a8b7b)

The Settings tab reads and writes a build's own `settings.toml` and keeps its comments and
unknown keys.

## What you need

- **Your own disc image**, one you legitimately own: either the European release (`SLES-03936`,
  PAL) or the USA release (`SLUS-01436`, NTSC-U). Supply the `.cue` plus its 2352-byte-per-sector
  `.bin`, or a bare `.bin`. A plain `.iso` will not work, because the movies are stored as raw
  sectors.
- **Windows 10 or 11, 64-bit (x64).**
- **Python 3.12 or newer.** The launcher runs from source, so Python is not bundled.
- **A C/C++ compiler**: either Visual Studio Build Tools 2022 or newer with the "Desktop
  development with C++" workload, or clang.
- **CMake.**
- **Ninja.**
- **About 3 GB of free disk space**: your disc image plus the generated build, which is about
  1.5 GB.
- **Internet access for the first build.** CMake fetches SDL3 while configuring the project.

Before it starts, the Play tab checks for **CMake, Ninja and a compiler**. If one is missing it
names it, for example `Missing: CMake, Ninja, a C/C++ compiler (MSVC Build Tools or clang)`, and
prints what to install for each. It does not check for Python or for free disk space, so make sure
those two are in place yourself.

The recompiler (`engine/`) and its MIT OpenBIOS image are bundled, so the only thing you supply is
the disc.

## Running it

```
run.bat
```

or

```
.venv\Scripts\python.exe main.py
```

To set the environment up first:

```
python -m venv .venv
.venv\Scripts\pip install -r requirements.txt
```

Then use the **Play** tab.

## Building the game (the Play tab)

1. **Add disc image...** and pick your `.cue` or `.bin`. The launcher reads the disc's own boot
   serial (`SLES`/`SCES` = PAL Europe, `SLUS`/`SCUS` = NTSC-U) and names the region.
2. **Check disc** (optional) reports that region. It never gates a build.
3. **Build selected disc** pre-flights the toolchain, runs the bundled recompiler over your disc,
   compiles the generated project with your CMake and Ninja, and places the finished build in
   `Builds/EUR/` or `Builds/USA/` beside the launcher. The first build of a region takes roughly
   10 to 20 minutes; the Activity log streams the real progress.
4. **Play (USA)** or **Play (Europe)** runs the regional build. Each build is self-contained (exe,
   `game.toml`, `bios/`, its input files, its memory cards) and is started with `PSX_DEV_INPUT=1`,
   which merges the keyboard and every connected controller onto Player 1.

Discs are read from `Discs/` beside the launcher, or from wherever you picked yours; the path is
recorded in that build's own `game.toml`. A recompile is only needed once per region.

## Tabs

| Tab | What it does |
|---|---|
| **Play** | Builds the game from your own disc (above), then runs the two regional builds. |
| **Memory Card** | Opens memory cards in the embedded save editor. It finds each build's `card1.mcd` / `card2.mcd` and any cards in `cards/`, or you can open one directly. |
| **Mods** | Lists a build's mod folders and enables/disables them by moving them. Rules in `docs/MODS.md`. |
| **Decompilation** | A read-only browser over the 100% complete decompilation tree. |
| **Settings** | Reads and writes a build's own `settings.toml` through a comment-preserving writer. |

The header carries an **Open Builds folder** button only. The window's own close control (the
title-bar X, or Alt+F4) is the way out.

## The folders beside the launcher

- `Builds/EUR/`, `Builds/USA/`: the builds you make from your discs (one folder each).
- `Discs/`: your disc images, if you keep them here. Optional; you can point at any path.
- `cards/`: extra PS1 memory cards you want the Memory Card tab to list. Optional.
- `editor/`: the save editor the Memory Card tab embeds. Ships with the launcher.
- `engine/`: the bundled recompiler, the OpenBIOS image, and the overlay toolchain inputs.

## Licences

- **The launcher** is licensed under **PolyForm Noncommercial 1.0.0 with additional terms**, in
  `LICENSE.md`: free to use, change and share for anything noncommercial; commercial use needs a
  licence from the author; improvements come back to the project.
- **The bundled engine** (`engine/`) is licensed under **PolyForm Noncommercial 1.0.0**, in
  `engine/LICENSE`.
- **OpenBIOS**, the free PS1 BIOS image the engine ships so you do not need a retail one, is
  **MIT** licensed. Its notice is `OpenBIOS.LICENSE` and `engine/framework/bios/OpenBIOS.LICENSE`.
- **TinyCC** (`engine/overlay_toolchain_inputs/tcc-0.9.27-win64-bin.zip`), the overlay compiler the
  builder bundles beside each finished build, is **LGPL-2.1**. The licence text is
  `engine/packaging/licenses/LGPL-2.1.txt`, and the bundling step writes a written offer for the
  compiler's source alongside it.
- **CPython** (`engine/overlay_toolchain_inputs/python-3.12.10-embed-amd64.zip`), the embedded
  interpreter that runs the overlay compiler, is **PSF-2.0**. Its licence text ships inside the
  distribution and is copied into the build's bundle.
- The complete list of third-party components and their licences is in
  `THIRD_PARTY_ATTRIBUTION.md`.

## Legal

- **No game code, no game data, no disc images and no retail BIOS are distributed.** Everything
  derived from your disc (`build/`, `Builds/`, the generated C) is produced locally and kept out of
  version control by `.gitignore`.
- `editor/` is the save editor the Memory Card tab embeds, and carries its own `LICENSE.md`
  (PolyForm Noncommercial 1.0.0 with additional terms).
- The only BIOS image in this repository is the MIT-licensed OpenBIOS at
  `engine/framework/bios/openbios.bin`. No retail PS1 BIOS is included.

## Design notes

- **Pre-flight before a long build.** A recompile takes 10 to 20 minutes, so the launcher checks
  for CMake, Ninja and a compiler first and says precisely what is missing.
- **A short path for the CLI.** The recompiler builds its helper command lines with a shell and
  does not survive a space or a bracket in a path. When the launcher's own folder has one, it hands
  the CLI the same folder under its Windows 8.3 short name.
- **tomlkit, not a naive TOML round-trip.** The build `game.toml` and the runtime `settings.toml`
  are hand-editable and carry comments; a write touches one key and keeps everything else.
- **The editor is embedded, not reimplemented.** The Memory Card tab imports the save editor's own
  window, so an editor update is picked up automatically.

## Dev

`verify_launcher.py` is the project's own gate. Run it after any change:

```
.venv\Scripts\python.exe verify_launcher.py
```

or `make test`. `smoke_test.py` builds the whole UI headlessly and names any tab that fails.
