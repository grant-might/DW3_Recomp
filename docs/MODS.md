# Mods: guide and rules

This is the document the launcher's **Mods** tab ships and links to. Anything distributed as a mod
for these builds should follow it, because the launcher enforces the parts it can and the rest is
what keeps players' saves intact.

Each regional build lives in its own folder (`Builds/EUR`, `Builds/USA`) and is its own install, so
a mod belongs to one build at a time. The Mods tab has a build picker at the top.

## What a mod is here

A mod is **one folder** (or one file) inside a build's `mods/` folder. The launcher lists what is in
that folder and lets players enable or disable each item; disabling moves it to `mods/.disabled/`
and enabling moves it back. Nothing else about the build tree is touched by the toggle, so removal
is always clean.

## The rules

1. **Stay inside the build.** A mod may add or replace files under that build's folder. It must not
   write anywhere else on the machine, and must not need administrator rights.
2. **Never touch the memory cards.** The build's `card1.mcd` / `card2.mcd` are the player's own data
   and they belong to the launcher's Memory Card tab. A mod that rewrites saves is not a mod, it is
   data loss.
3. **Never modify the recompiled exe.** It is a build output, and patching it breaks every future
   fix and cannot be toggled off reliably.
4. **Keep the original.** When you replace a file, ship the stock one beside it as
   `<name>.orig-stock`, so the swap is reversible. A mod must be revertible by a player who does not
   trust it yet.
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
    <files it overrides, mirroring the build tree>
    <original>.orig-stock
```

## What the launcher checks

- the item is inside that build's `mods/`,
- a replacement that overrides an existing file has its `.orig-stock` partner.

Anything else is on the mod author's honour, and on the player's judgement, which is why rule 7
and rule 6 matter more than they look.

## A note for players

Enabling mods changes the game data the runtime reads. Keep the launcher's Memory Card tab for your
saves, and copy `card1.mcd` / `card2.mcd` somewhere safe before adding a mod that claims to change
balance.
