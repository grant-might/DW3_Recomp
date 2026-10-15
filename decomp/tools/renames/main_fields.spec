# The shared headers' field renames (tools/rename_field.py --spec), in the
# order they were made. Run again after a rebase: done renames do nothing.

# round 4: the cards' images (include/engine/gfx.h), from CARDPAK0-4's headers
CardImage unk6 level --via 'card'
CardImage unk8 rank --via 'card' --via current --via header
CardImage unkA comboCard --via 'card'

# round 4: the items (include/engine/game_state.h)
ItemInfo unk8 kind --via 'getItem\(item\)'
WeaponData unk0 charisma --via 'weapon'
WeaponData unk4 partners --via 'weapon|armor|acc'
ArmorData unk0 charisma --via 'armor|acc'
ArmorData unk4 partners --via 'weapon|armor|acc'
AccessoryData unk0 charisma --via 'armor|acc'
AccessoryData unk4 partners --via 'weapon|armor|acc'

# round 4: the icon STGMCARD_runSaves writes (include/engine/memcard.h)
MemCard unk324 iconIndex --via 'MEMCARD'

# final round: fields nothing reads (include/engine/game_state.h, text.h)
GameState unkC unusedC --via 'GAME'
TextWindow unkC4 unusedC4 --via 'obj'
TextWindow setUnkC4 setUnusedC4 --via 'obj'
ZoomBox unk68 unused68 --via 'task'
