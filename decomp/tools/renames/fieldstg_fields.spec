# FIELDSTG's field renames (tools/rename_field.py --spec), in the order they
# were made. Run again after a rebase: done renames do nothing.

# the map's tile file: its first word counts the tiles with data (every
# S###PACK.BIN of both versions)
MapFileHeader unk0 tileCount --via header
# the battles' lists: a number of their own, 1 to 448 over the stages
FieldBattles unk0 serial --via 'battles|list'
# the kind of an encounter, which FIGHTSTG's enemies test (condition 13)
Encounter unkD kind --via 'FIELDSTG_encounters\[encounter\]'
BattleSetup unk3D encounterKind --via BATTLE_SETUP
