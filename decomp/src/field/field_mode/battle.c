/* The battles that start on the field: the steps to the next one and the
   battles of the map's areas */

#include "field/field_mode.h"

/* Rolls the steps to the next battle (GAME.battleSteps): under 0x100, or 0x100 to 0x4FF */
void FIELDSTG_rollBattleSteps(void) {
    s32 value = RANDOM.next() % 2304;

    if (value < 0x100) {
        GAME.battleSteps = value;
    } else {
        GAME.battleSteps = (value + 0x100) / 2;
    }
}

/* Starts one of the eight battles of the player's battle area, at random */
void FIELDSTG_startAreaBattle(void) {
    Actor *actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
    Vec2 tile;
    s32 area;
    s32 index;
    AreaBattle *battle;

    tile = actor->tile;
    area = (u8)FIELDSTG_map.getCell(FIELD_MAP_AREAS, &tile) - 1;
    index = RANDOM.next() & 7;
    battle = FIELDSTG_state.battles->battles[area]->battles[index];
    FIELDSTG_startBattle(battle);
}

/* Counts down to the next battle at each step by the area's rate
   (FIELDSTG_battleRates), unless the field is busy, and starts it when they
   run out */
void FIELDSTG_countBattleSteps(void) {
    Actor *actor;
    Vec2 tile;
    s32 area;
    s32 rate;

    if (FIELDSTG_map.files[FIELD_MAP_AREAS] != 0 && FIELDSTG_state.battles != NULL && FIELDSTG_state.battleStarting == 0 &&
        FIELDSTG_state.busy == 0 && FIELDSTG_state.acting == 0 && FIELDSTG_state.frozen == 0) {
        actor = TASK_REGISTRY.funcs.find(FIELD_TASK_ACTOR, -1, 0);
        tile = actor->tile;
        area = (u8)FIELDSTG_map.getCell(FIELD_MAP_AREAS, &tile);
        if (area != 0) {
            area--;
            rate = FIELDSTG_battleRates[FIELDSTG_state.battles->battles[area]->count];
            GAME.battleSteps -= rate;
            if (GAME.battleSteps <= 0) {
                if (BATTLE_SETUP.randomBattles != 0) {
                    FIELDSTG_startAreaBattle();
                }
                FIELDSTG_rollBattleSteps();
            }
        }
    }
}

/* Starts battle index of the fourth area, the one events start */
void FIELDSTG_startEventBattle(s32 index) {
    AreaBattle *battle;

    if (FIELDSTG_state.battles != NULL) {
        battle = FIELDSTG_state.battles->battles[3]->battles[index];
        FIELDSTG_startBattle(battle);
    }
}
