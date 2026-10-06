# DW3 Recompiled+

A PySide6 launcher for native Windows recompilations of **Digimon World 3** (NTSC-U, `SLUS-01436`)
and **Digimon World 2003** (PAL, `SLES-03936`).

The launcher ships **no game code and no game data**. You point it at your own disc image, it
recompiles that disc into a native build on your machine, and then it runs it.

## What you need

- Windows with a **C/C++ toolchain**: Visual Studio Build Tools (the "Desktop development with
  C++" workload) or clang, plus **CMake** and **Ninja**. The Play tab pre-flights all three and
  tells you exactly what is missing before it starts anything.
- Your own **Digimon World 3 / 2003 disc image** (`.cue` + 2352-byte-per-sector `.bin`, or a bare
  `.bin`). Both regions are supported and nothing is refused for its region.
- Python 3 with the pinned dependencies (see `requirements.txt`). A `.venv` is used below.

The bundled recompiler (`engine/`) and its MIT OpenBIOS image are included, so you supply only the
disc. Internet access is needed on the first build: the runtime fetches SDL3 with CMake.

## Running it

```
run.bat
```

or

```
.venv\Scripts\python.exe main.py
```

## Building the game (the Play tab)

1. **Add disc image…** — pick your `.cue` or `.bin`. The launcher reads the disc's own boot serial
   (`SLES`/`SCES` = PAL Europe, `SLUS`/`SCUS` = NTSC-U) and names the region.
2. **Check disc** — optional: shows the region, and DiscTool's own report if a copy happens to sit
   beside the launcher. It never gates anything.
3. **Build selected disc** — the launcher pre-flights the toolchain, runs the bundled recompiler
   over your disc, compiles the generated project locally with your CMake + Ninja + compiler, and
   places the finished build in `Builds/EUR/` or `Builds/USA/` beside the launcher. Expect
   10-15 minutes; the Activity log streams every line of real progress.
4. **Play (USA)** / **Play (Europe)** — run the regional build. Each build is self-contained
   (exe + `game.toml` + `bios/` + its input files) and is started with `PSX_DEV_INPUT=1`, which
   merges the keyboard and every connected controller onto Player 1.

Discs are read from `Discs/` beside the launcher, or from wherever you picked yours — the path is
recorded in that build's own `game.toml`. A recompile is only needed once per region.

## Tabs

| Tab | What it does |
|---|---|
| **Play** | Builds the game from your own disc (above), then runs the two regional builds. |
| **Memory Card** | Opens memory cards in the **embedded save editor** — its own window class, in-process, no file dialog. |
| **Mods** | Lists the runtime's mod payloads, enables/disables by moving them, and ships the rules. |
| **Decompilation** | A read-only browser over the 100% complete decompilation tree. |
| **Settings** | Reads and writes the runtime's own settings file through a **comment-preserving** writer. |

The header carries an **Open Builds folder** button only; the window's own close control (the
title-bar X, or Alt+F4) is the way out.

## Legal

- The launcher itself is **PolyForm Noncommercial 1.0.0** (see `LICENSE.md`).
- **No game code, no game data, no disc images and no retail BIOS** are distributed. Everything
  derived from your disc (`build/`, `Builds/`, the generated C) is produced locally and is kept out
  of version control by `.gitignore`.
- `engine/` is the PSXRecomp recompiler, under the same PolyForm Noncommercial license; its
  `LICENSE` and `THIRD_PARTY_ATTRIBUTION.md` travel with it. The bundled BIOS is **OpenBIOS**
  (PCSX-Redux, MIT), redistributable, with its notice as `OpenBIOS.LICENSE`.

## Design decisions worth knowing

- **Pre-flight before a long build.** A recompile takes 10-15 minutes; the launcher checks for
  CMake, Ninja and a compiler first and says precisely what is missing rather than failing halfway.
- **The CLI is handed a short path.** The recompiler builds its helper command lines with a shell
  and does not survive a space or a bracket in a path. When the launcher's own folder has one, the
  builder hands the CLI the same folder under its Windows 8.3 short name.
- **tomlkit, not a naive TOML round-trip.** Build `game.toml` files are hand-editable and carry
  comments; writes touch one key and keep everything else.
- **The editor is embedded, not reimplemented.** The Memory Card tab imports the save editor's own
  `MainWindow`, so any editor improvement is picked up automatically.
- **Offscreen renders.** Visual checks are done with `Qt.WidgetAttribute.WA_DontShowOnScreen`; the
  launcher never flashes windows on the desktop.

## Dev

`verify_launcher.py` is the project's own gate — run it after any change:

```
.venv\Scripts\python.exe verify_launcher.py
```

`smoke_test.py` builds the whole UI headlessly and names any tab that fails.
