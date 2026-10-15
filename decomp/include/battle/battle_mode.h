#ifndef FIGHTSTG_H
#define FIGHTSTG_H

/* FIGHTSTG.PRO: the battle. It loads WFIGHTMN (file 0x1FA) for a normal
   battle, or WFIGHTTS (file 0x1FB) for the battle test, at STAGE_VRAM.
   Its declarations are in include/battle/battle_mode/, by module; this header
   includes them all.

   The Japanese version's battle (FIGHTSTG, WFIGHTMN) is the European one
   wherever that differs from the USA one (the counterattack, the revival,
   the camera, the battle menu's fixes), but for the files it loads, which
   it numbers as the USA disc does: what the comments here say of the
   European version holds for the Japanese one too. */

#include "battle/battle_mode/types.h"
#include "battle/battle_mode/model.h"
#include "battle/battle_mode/stage.h"
#include "battle/battle_mode/camera.h"
#include "battle/battle_mode/effect.h"
#include "battle/battle_mode/battle.h"
#include "battle/battle_mode/action.h"
#include "battle/battle_mode/menu.h"
#include "battle/battle_mode/task.h"

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) FIGHTSTG_##name

/* The sub-overlays (FIGHTSTG_updateRoot) and their entry points */
#if VERSION_US || VERSION_JP
#define FILE_WFIGHTMN 0x1FA
#define FILE_WFIGHTTS 0x1FB
#elif VERSION_EU
#define FILE_WFIGHTMN 0x208
#define FILE_WFIGHTTS 0x209
#endif
Task *WFIGHTMN_start(void);
Task *WFIGHTTS_start(void);
/* WFIGHTMN's functions FIGHTSTG calls */
BattleScript *WFIGHTMN_startTech(u8 actor, s32 id);
void WFIGHTMN_checkEquip(s32 fighter);
void WFIGHTMN_chargeGauge(u8 side, s32 damage);
s32 WFIGHTMN_setIdleMotion(u8 id, s32 damage);
void WFIGHTMN_countHit(u8 side, s32 damage);
void WFIGHTMN_endWeakness(u8 side);
s32 WFIGHTMN_limitDamage(u8 side, s32 damage, s32 hits);

#endif /* FIGHTSTG_H */
