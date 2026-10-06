# Mods — guide and rules

This is the document the launcher's **Mods** tab ships and links to. Anything distributed as a
mod for this native build should follow it, because the launcher enforces the parts it can and
the rest is what keeps players' saves intact.

## What a mod is here

A mod is **one folder** (or one file) inside `<install>/mods/`. The launcher lists what is in
that folder and lets players enable or disable each item; disabling moves it to
`<install>/mods/.disabled/` and enabling moves it back. Nothing else about the game tree is
touched by the toggle, so removal is always clean.

## The rules

1. **Stay inside the install.** A mod may add or replace files under the game install tree. It
   must not write anywhere else on the machine, and must not need administrator rights.
2. **Never touch `DMW3Game/SAVEDATA`.** That is the player's own data and it is owned by the
   launcher's Memory Card tab. A mod that rewrites saves is not a mod, it is data loss.
3. **Never modify `Binaries/*.exe`.** The runtime is a recompiled binary; patching it breaks
   every future fix and cannot be toggled off reliably.
4. **Keep the original.** When you replace a file, ship the stock one beside it as
   `<name>.orig-stock` — the project already uses that convention (`logo_titulo.png.orig-stock`
   does exactly this). A mod must be revertible by a player who does not trust it yet.
5. **No network at run time.** A mod may not phone home, download assets, or require a service.
6. **Declare save compatibility.** If saves created with the mod will not load without it, say so
   in the mod's `README.md` in those words.
7. **One purpose per mod.** Two changes that players would plausibly want independently belong in
   two folders, so the toggle is meaningful.

## What a mod folder should contain

```
mods/
  my-mod/
    README.md          what it changes, and how to uninstall it
    <files it overrides, mirroring the install tree>
    <original>.orig-stock
```

## What the launcher checks

- the item is inside `mods/` (not a stray symlink or a path that escapes the install),
- a replacement file has its `.orig-stock` partner,
- the mod does not target `DMW3Game/SAVEDATA` or `Binaries/*.exe`.

Anything else is on the mod author's honour — and on the player's judgement, which is why rule 7
and rule 6 matter more than they look.

## Working with the engine's existing mod surface

The install already carries mod-style data under `Engine/` (zone tables, boss and farming data,
a replacement title logo). Those files are the safe, supported seams: they are data, they are
replaced wholesale, and the stock copies are kept. Prefer them over anything that patches code.

## A note for players

Enabling mods changes the game data the runtime reads — keep the launcher's Memory Card tab for
your saves, and back up `DMW3Game/SAVEDATA` before adding a mod that claims to change balance.
