/* The field's own stage: the ends of its events and its setup */

#include "field/field_mode.h"

/* The ends of the events of question n's answers (FIELDSTG_events): both set
   event flag 0x400 + n, and the first answer's also applies actions a and b */
#define END_CHOICE(n, a, b)                     \
    void FIELDSTG_endChoice##n##Answer0(void) { \
        FLAGS_00.applyAction(a, 1);             \
        FLAGS_00.applyAction(b, 1);             \
        FLAGS_00.applyAction(0x400 + n, 1);     \
    }                                           \
                                                \
    void FIELDSTG_endChoice##n##Answer1(void) { \
        FLAGS_00.applyAction(0x400 + n, 1);     \
    }

END_CHOICE(0, 0x707E, 0x8B19)
END_CHOICE(1, 0x707E, 0x8B1F)
END_CHOICE(2, 0x707F, 0x8B1A)
END_CHOICE(3, 0x707F, 0x8B20)
END_CHOICE(4, 0x7080, 0x8489)
END_CHOICE(5, 0x7080, 0x8495)
END_CHOICE(6, 0x7081, 0x847C)
END_CHOICE(7, 0x7081, 0x8462)
END_CHOICE(8, 0x7082, 0x8ADE)
END_CHOICE(9, 0x7082, 0x8AE8)
END_CHOICE(10, 0x7083, 0x8AF4)
END_CHOICE(11, 0x7083, 0x8AF3)
END_CHOICE(12, 0x7084, 0x8B01)
END_CHOICE(13, 0x7085, 0x8B0D)
END_CHOICE(14, 0x7086, 0x8B02)
END_CHOICE(15, 0x7087, 0x8B0F)

/* The color the field's stage starts with (FieldState.spriteColor) */
const CVECTOR FIELDSTG_startColor = {0x80, 0x80, 0x80, 0};

#if VERSION_US || VERSION_JP
#define FIELD_FILE 0x19F
#elif VERSION_EU
#define FIELD_FILE 0x1AD
#endif

/*
 * The setup of the field's own stage (FIELDSTG_initFuncs), which fills
 * FIELDSTG_state as the stage overlays' setup functions do; some points of the
 * story change its soundBank and music. As theirs, the match depends on the
 * start position being set with a constructor, (Vec2){x, y}.
 */
void FIELDSTG_setupField(void) {
#if VERSION_US || VERSION_JP
    FIELDSTG_state.textFile = TEXT_FILE(0xF0);
#endif
    FIELDSTG_state.mapFile = FIELD_FILE - 1;
    FIELDSTG_state.sheetEntry = FIELD_FILE << 16;
    FIELDSTG_state.objects = FIELDSTG_mapObjects;
    FIELDSTG_state.slots = FIELDSTG_slots;
#if VERSION_US || VERSION_JP
    FIELDSTG_state.imageFile = 0x31D;
#elif VERSION_EU
    FIELDSTG_state.imageFile = 0x32C;
    FIELDSTG_state.textFile = LANGUAGE + 0xE8;
#endif
    FIELDSTG_state.start = (Vec2){0x6700, 0x12300};
    FIELDSTG_state.images.field = &FIELDSTG_images.field;
    FIELDSTG_state.soundBank = 0x42;
    FIELDSTG_state.actors = FIELDSTG_actorList;
    FIELDSTG_state.startDir = 0;
    FIELDSTG_state.music = MUSIC(0x42, 2);
    FIELDSTG_state.spriteColor = FIELDSTG_startColor;
    FIELDSTG_state.events = FIELDSTG_events;
    FIELDSTG_map.setFile(0, FIELD_FILE << 16 | 2);
    FIELDSTG_map.setFile(1, FIELD_FILE << 16 | 3);
    FIELDSTG_map.setFile(FIELD_MAP_TRIGGERS, FIELD_FILE << 16 | 1);
    FIELDSTG_map.setFirstMap(0);
    switch (GAME.progress) {
    case 5:
    case 8:
    case 12:
    case 14:
    case 16:
    case 22:
    case 24:
    case 26:
    case 28:
    case 30:
    case 31:
    case 34:
    case 36:
    case 37:
    case 38:
    case 39:
        FIELDSTG_state.soundBank = 0x42;
        FIELDSTG_state.music = MUSIC(0x42, 0);
        break;
    }
}
