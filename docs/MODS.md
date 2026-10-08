# Mods: guide and rules

This is the document the launcher's **Mods** tab ships and links to. It covers the two ways a mod
reaches a build, what the launcher does for you, and the rules that keep players' saves intact.

Each regional build lives in its own folder (`Builds/EUR`, `Builds/USA`) and is its own install, so
a mod belongs to one build at a time. The Mods tab has a build picker at the top, and it leads with
USA, the same order the Play tab uses.

## Mod packages (the normal way)

A mod package is a **ZIP whose archive root holds `manifest.toml`**. It says what the mod is, which
discs it targets, which features it can switch on, and whether it ships code. The launcher's Mods
tab reads it and installs it for you:

1. **Load mod package…** and pick the ZIP. The tab shows the id, version, name, description, the
   targets and the features, so you can see what you are about to install.
2. **Install** unpacks it into this build as `mods/packages/<id>/<version>/` and writes
   `mods/state.toml`, which is what switches a feature on or off. A fresh install switches the
   mod's features on; **Disable** / **Enable** and the feature picker change that at any time, and
   **Reinstall** replaces a version that is already installed.
3. If the package ships code, press **Rebuild (USA)** or **Rebuild (EUR)** for the region you play.
   That is the step that puts the mod's behaviour into the game, and it is the step no one should
   have to do by hand. Then press Play.

### Why a package needs a rebuild

Installing the package is data only. A mod that changes how the game behaves does it with **native
code** that has to be compiled into that build's executable: the runtime registers plugin code from
a constructor inside the binary, and the manifest's `[[plugin]]` block is only an id reference, not
a loader. A launch with a package selected whose plugin is not linked in is refused
(`trusted plugin is unavailable`), so the ZIP on its own can add nothing.

The rebuild compiles the plugin source of **every installed and enabled package** for that region in
one pass, not just the one loaded in the Mods tab: it stages each enabled package's
`plugin/<name>.c` into the build, sets that build's region define (a mod that touches game memory
carries one address table per region), rebuilds and relinks once, and replaces the executable. It
prunes the staged sources down to exactly the enabled set first, so a mod you disable stops being
compiled in rather than staying linked forever. It reuses the launcher's own build path, so nothing
about your toolchain setup changes.

**How long it takes.** If that region's build tree already exists, the rebuild is incremental: the
enabled mods' source files are compiled and the executable is relinked, which takes seconds. If the
build does not exist yet the rebuild says so and does nothing - build the disc on the Play tab first
(10 to 20 minutes, about 3 GB), and every rebuild after that is the fast path.

Two enabled packages that name each other in `conflicts` cannot run together: the rebuild refuses
before it compiles anything and names both ids, so disable one of the pair first.

A package that ships no `plugin/` folder is data only (byte patches, disc overlays): install it and
play. The Rebuild buttons say so rather than doing pointless work.

## Folder mods (no rebuild)

A folder or file dropped into a build's `mods/` folder is a data-only mod. The launcher lists what
is there and lets you enable or disable each item: disabling moves it to `mods/.disabled/` and
enabling moves it back. Nothing else about the build tree is touched by the toggle, so removal is
always clean.

## The rules

1. **Stay inside the build.** A mod may add or replace files under that build's folder. It must not
   write anywhere else on the machine, and must not need administrator rights.
2. **Never touch the memory cards.** The build's `card1.mcd` / `card2.mcd` are the player's own data
   and they belong to the launcher's Memory Card tab. A mod that rewrites saves is not a mod, it is
   data loss.
3. **Never patch the recompiled exe by hand.** It is a build output, and patching it breaks every
   future fix and cannot be toggled off reliably. A plugin mod's code goes in through the Rebuild
   buttons, which is the same build path the Play tab uses.
4. **Keep the original.** When you replace a file, ship the stock one beside it as
   `<name>.orig-stock`, so the swap is reversible. A mod must be revertible by a player who does not
   trust it yet.
5. **No network at run time.** A mod may not phone home, download assets, or require a service.
6. **Declare save compatibility.** If saves created with the mod will not load without it, say so
   in the mod's `README.md` in those words.
7. **One purpose per mod.** Two changes that players would plausibly want independently belong in
   two folders or two packages, so the toggle is meaningful.

## What a package contains

```
manifest.toml        id, version, name, description, [[target]]s, [[feature]]s, [[plugin]]s
INSTALL.md           how to install by hand, if you are building without the launcher
plugin/<name>.c      the source the build compiles in (omit for a data-only mod)
```

The runtime reads `manifest.toml` only; the other files travel with the package so it is
self-contained for the code step.

## What the launcher checks

- a loaded package is a ZIP with `manifest.toml` at its root, and it parses;
- the package/version directory does not already exist (Reinstall is the way to replace one);
- `mods/state.toml` is written in the format the runtime reads (`format_version = 2`, one
  `[[package]]` and one `[[feature]]` per package);
- a rebuild is refused, with what to do about it, when that region's build tree is missing, when the
  package ships no source, or when the game is running.

Anything else is on the mod author's honour, and on the player's judgement, which is why rule 7
and rule 6 matter more than they look.

## A note for players

Mods change the game data and the game's executable for that build. Keep the launcher's Memory Card
tab for your saves, and copy `card1.mcd` / `card2.mcd` somewhere safe before adding a mod that
claims to change balance. Removing a package and pressing Rebuild again puts the build back to the
mod-free executable.

## Patching your disc image with a .bps patch

The packages above change a recompiled build. A **.bps patch** changes the **disc image** itself,
which is a different thing and lives on the Mods tab too, in the card "Apply a .bps patch to your
disc image". That card turns your own retail image into a finished patched image in one step:

1. **Patch** lists the patches the launcher ships (a `.bps` is small and holds no game data). Pick
   the listed one, or **Choose .bps file…** for any `.bps` on your machine.
2. **Disc image** defaults to the disc the launcher already knows for this build (from `Discs/`, or
   the one baked into the build's `game.toml`). Override it with **Change disc image…**; a `.cue` is
   accepted and the `.bin` it names is used.
3. **Output** defaults to a free name beside your image (`… (patched).bin`). **Change output…** sets
   your own name; an existing file is refused rather than overwritten.
4. **Apply patch** runs off the UI thread and reports in the card's status line and log.

What the launcher guarantees while it runs:

- your disc image is opened **read-only** and is never modified;
- the patch's **source checksum is checked against your image before anything is written**, so a
  patch for a different disc is refused (the same meaning as Flips' "This patch is not intended for
  this ROM") and no output file is left behind;
- the result's **target checksum is checked after**, and if it does not match the output is deleted
  so no bad image is left looking finished;
- the images are streamed through a memory map, so a ~650 MB disc is not loaded into RAM.

A `.bps` is not a mod package and needs no rebuild: the patched image is used like any other disc
image (point the Play tab / the build's `game.toml` at it). Keep the original image: the patch only
ever writes a copy.
