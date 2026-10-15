# The battle overlays' field renames (tools/rename_field.py --spec), in the
# order they were made. Run again after a rebase: done renames do nothing.

# FIGHTSTG's models
ModelBone unk8 keyFile --via bone --via 'bones\[[^]]*\]'
Mesh drawAlt drawWireframe
MotionStep unk6 endFrame --via step --via 'step\[-1\]'
Model unkD24 blendTargets
Model unk19A4 blendSources
FightStageInfo unk10 noBoundsBones --via 'stages\[[^]]*\]'
FighterInfo unk10 distance --via info --files src/battle/battle_mode/models.c
BattleScriptChildren unk4 jump --via children --files 'src/battle/battle_mode/*.c'
BattleScriptChildren unk8 move --via children --files 'src/battle/battle_mode/*.c'
BattleScriptChildren unk10 spriteEffects --via children --files 'src/battle/battle_mode/*.c'

# the executable's battle records, read by the battle
BattleSetup unk0 randomBattles --via BATTLE_SETUP
BattleSetup unk4 debugUpDown --via BATTLE_SETUP
BattleSetup unk8 debugLeftRight --via BATTLE_SETUP
BattleSetup unk3E blocks --via BATTLE_SETUP
BattleEnemy unkA strength --via 'enemies\[[^]]*\]'
TechData unkD scriptStage --via tech --via info --via dst
TechData unkE scriptEffect --via tech --via info --via dst
TechData unkF scriptSound --via tech --via info --via dst
TechData unk10 script --via tech --via info --via entry
DigimonData unk2A pairTech --via data
DigimonData unk3D pairPartner --via data --via 'GET_DIGIMON\([^)]*\)'
DigimonData unk2C statusResists --via digimon --via other
DigimonData unk50 blastForms --via digimon

# STAGSLCT's windows of BATTLE_SETUP's values
StageSelectWindows unk88 randomBattles --via win
StageSelectWindows unk94 debugUpDown --via win
StageSelectWindows unk98 debugLeftRight --via win

# CARDGAME
CardPlay unk2 mark --via 'plays\[[^]]*\]'
CardWindow unkE showCount --via 'windows\[[^]]*\]' --via window
CardScreen unk5C opponent --via screen

# The card opponent's level and the plays a resolve starts with
CardOpponent unkC8 level --via opponent
CardBattle unk2E9 opponentLevel --via battle
CardScreen unk5E opponentLevel --via screen
CardBattle unk4DE playsToResolve --via battle
