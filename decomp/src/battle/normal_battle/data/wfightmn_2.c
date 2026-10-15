/* WFIGHTMN's second object's data: the screen, the technique tables of the
   last enemy and the event states. */

#include "battle/normal_battle.h"

RECT WFIGHTMN_screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
/* A technique's look (its scriptEffect and scriptSound) by its effect, for
   WFIGHTMN_bringLastEnemy; the list ends at the first -1, so the USA
   version's three entries after it are never read */
#if VERSION_US
EffectLook WFIGHTMN_effectVisuals[] = {
    { TECH_EFFECT_POISON, 19, 26 },
    { TECH_EFFECT_PARALYSIS, 20, 26 },
    { TECH_EFFECT_CONFUSION, 21, 27 },
    { TECH_EFFECT_SLEEP, 22, 50 },
    { TECH_EFFECT_KNOCK_OUT, 26, 50 },
    { TECH_EFFECT_DRAIN, 28, 39 },
    { -1, 37, 49 },
    { TECH_EFFECT_LOWER_DEFENSE, 39, 49 },
    { 28, 41, 49 },
    { -1, 0, 0 },
};
#elif VERSION_EU || VERSION_JP
EffectLook WFIGHTMN_effectVisuals[] = {
    { TECH_EFFECT_POISON, 19, 26 },
    { TECH_EFFECT_PARALYSIS, 20, 26 },
    { TECH_EFFECT_CONFUSION, 21, 27 },
    { TECH_EFFECT_SLEEP, 22, 50 },
    { TECH_EFFECT_KNOCK_OUT, 26, 50 },
    { TECH_EFFECT_DRAIN, 28, 39 },
    { -1, 37, 49 },
};
#endif
/* A technique's look (its scriptEffect, scriptSound and scriptStage) by its
   element, for WFIGHTMN_bringLastEnemy when WFIGHTMN_effectVisuals gives
   none */
ElementLook WFIGHTMN_elementVisuals[] = {
    { 2, 5, 64, 34 },
    { 3, 9, 45, 37 },
    { 4, 11, 44, 40 },
    { 5, 14, 29, 43 },
    { 6, 16, 44, 46 },
    { 7, 65, 64, 49 },
    { 8, 7, 64, 52 },
    { -1, 0, 0, 0 },
};
void (*WFIGHTMN_states[])(BattleMenu *task, BattleMenuChildren *children) = {
    NULL, NULL, NULL, WFIGHTMN_runCommand,
    WFIGHTMN_digivolve, WFIGHTMN_tag, WFIGHTMN_pairTech, WFIGHTMN_endBattle,
    WFIGHTMN_runAway, WFIGHTMN_endAutoRecover, WFIGHTMN_recover, WFIGHTMN_clearField,
    WFIGHTMN_takeDamage, WFIGHTMN_cureStatus, WFIGHTMN_cureStatus, WFIGHTMN_cureStatus,
    WFIGHTMN_endBoost, WFIGHTMN_runConfusedCommand, WFIGHTMN_endRestriction, WFIGHTMN_blast,
    WFIGHTMN_endBlast, WFIGHTMN_knockOut, WFIGHTMN_endSpecial, WFIGHTMN_digidevolve,
    WFIGHTMN_showWon, WFIGHTMN_bringLastEnemy, WFIGHTMN_restoreEnemy,
};
/* WFIGHTMN_startTech's effects (BattleScript.effect and sound) by the first of
   FIGHTSTG_action.effects[2..12] that is set: the kinds of WFIGHTMN_effectVisuals */
ScriptLook WFIGHTMN_actionEffects[] = {
    {19, 26}, {20, 26}, {21, 27}, {22, 50}, {26, 50}, {0, 0},
    {28, 39}, {0, 0}, {46, 30}, {0, 59}, {31, 58},
};
