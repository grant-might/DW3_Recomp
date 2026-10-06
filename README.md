# Digimon World 3 — Native Launcher

A GUI launcher for the native (recompiled) Digimon World 3 build. It locates the install,
validates and installs the player's own disc, starts the game, opens their saves in the save
editor, manages mods, and exposes the runtime's settings.

## Running it

```
run.bat
```

or

```
.venv\Scripts\python.exe main.py
```

The `.venv` is already built (PySide6 6.11.2 + tomlkit 0.15.1 — the same PySide6 the save
editor uses, so the editor embeds cleanly).

## Tabs

| Tab | What it does |
|---|---|
| **Play** | Finds the install, lists disc images in `<install>/discs/`, validates one with the project's own `DiscTool.exe ver`, extracts it with `DiscTool sacar`, launches the game, shows the last run report and the live controller settings. |
| **Memory Card** | Auto-detects the saves the runtime is using (`<install>/DMW3Game/SAVEDATA/save*.sav`) plus any memory cards in `discs/`, `Binaries/`, `cards/`, and opens them in the **embedded save editor** — its own window class, in-process, no file dialog. |
| **Mods** | Lists mods in `<install>/mods/`, enables/disables by moving them to `.disabled/`, creates a mod skeleton, and ships the rules (`docs/MODS.md`). |
| **Settings** | Reads and writes the runtime's own `Binaries/settings.toml` — video, audio, loading, controller — through a **comment-preserving** writer, so the file's documentation and any unknown key survive. |

The header carries an **Install folder** button only; the window's own close control (the title-bar X,
or Alt+F4) is the way out.

## Scope, stated honestly

**This runtime is compiled from the European disc.** `Engine/Config/game.toml` pins
`id = "SLES-03936"` and the game code is compiled *into* `DigimonWorld2003.exe`, and
`DiscTool.exe` refuses anything else in as many words ("*…no la edicion europea… Hace falta la
version europea (SLES-03936)*").

So today: **PAL/Europe plays; USA does not.** The launcher detects a USA disc and says exactly
that instead of installing something that cannot boot. Playing USA natively needs a *second
recompilation* of the SLUS binary (its own `game.toml`, seeds, rebuild and validation against
the runtime) — engine work, not launcher work. Everything here is already multi-profile in
structure so a second region can be added without redesign.

## Design decisions worth knowing

- **tomlkit, not a naive TOML round-trip.** `settings.toml` is hand-editable and its comments
  explain every key; a plain parse/dump would destroy that. Writes go through tomlkit and touch
  only the keys the launcher owns. If tomlkit is missing the Settings tab refuses to write
  rather than damage the file.
- **`input.ini` / `keybinds.ini` are never rewritten.** Controller bindings belong to the
  runtime and its own settings menu; the launcher only displays them and reveals the folder.
- **The editor is embedded, not reimplemented.** The Memory Card tab imports the save editor's
  own `MainWindow` and reuses its open sequence, so any editor improvement is picked up
  automatically.
- **Art is optional at every slot.** The launcher draws fallbacks, so it looks finished now and
  upgrades as art lands. Sizes are in `docs/ASSETS.md`.

## Open items

- **Volume**: `settings.toml` has no volume key (only `spu_hq` and `mute_unfocused`), so the
  launcher cannot offer a volume slider yet — the in-game menu owns it. If the engine gains a
  volume key, it is a one-line addition.
- **Controller editing**: currently displayed read-only. `p1_device`, `p1_mode` and `deadzone`
  are in `settings.toml`, so making them editable is straightforward once the valid device/mode
  values are confirmed against the engine.
- **USA region**: blocked on a second recompilation (see above).

## Dev

`smoke_test.py` builds the whole UI headlessly and names any tab that fails:
`set QT_QPA_PLATFORM=offscreen` then run it with the venv python.
