#include "engine/game.h"
#include "libsnd.h"

/* Each bank's files (SOUND_BANK_FILES[id]), numbered in the Japanese disc as
   in the USA one */
#if VERSION_US || VERSION_JP
SoundFiles SOUND_FILES_01 = {370, 357, 0x1650000, 0x1720000, {0x1650001, 0x1650002, 0}};
SoundFiles SOUND_FILES_02 = {361, 348, 0x15C0000, 0x1690000, {0x15C0001, 0}};
SoundFiles SOUND_FILES_03 = {1122, 1120, 0x4600000, 0x4620000, {0x4600001, 0}};
SoundFiles SOUND_FILES_04 = {362, 349, 0x15D0000, 0x16A0000, {0x15D0001, 0}};
SoundFiles SOUND_FILES_05 = {363, 350, 0x15E0000, 0x16B0000, {0x15E0001, 0}};
SoundFiles SOUND_FILES_06 = {514, 512, 0x2000000, 0x2020000, {0x2000001, 0}};
SoundFiles SOUND_FILES_07 = {364, 351, 0x15F0000, 0x16C0000, {0x15F0001, 0}};
SoundFiles SOUND_FILES_08 = {365, 352, 0x1600000, 0x16D0000, {0x1600001, 0}};
SoundFiles SOUND_FILES_09 = {366, 353, 0x1610000, 0x16E0000, {0x1610001, 0}};
SoundFiles SOUND_FILES_0A = {522, 519, 0x2070000, 0x20A0000, {0x2070001, 0}};
SoundFiles SOUND_FILES_0B = {367, 354, 0x1620000, 0x16F0000, {0x1620001, 0}};
SoundFiles SOUND_FILES_0C = {368, 355, 0x1630000, 0x1700000, {0x1630001, 0}};
SoundFiles SOUND_FILES_0D = {540, 534, 0x2160000, 0x21C0000, {0x2160001, 0}};
SoundFiles SOUND_FILES_0E = {553, 552, 0x2280000, 0x2290000, {0x2280001, 0}};
SoundFiles SOUND_FILES_0F = {563, 562, 0x2320000, 0x2330000, {0x2320001, 0}};
SoundFiles SOUND_FILES_10 = {568, 567, 0x2370000, 0x2380000, {0x2370001, 0}};
SoundFiles SOUND_FILES_11 = {588, 587, 0x24B0000, 0x24C0000, {0x24B0001, 0}};
SoundFiles SOUND_FILES_12 = {594, 593, 0x2510000, 0x2520000, {0x2510001, 0}};
SoundFiles SOUND_FILES_13 = {650, 649, 0x2890000, 0x28A0000, {0x2890001, 0}};
SoundFiles SOUND_FILES_14 = {613, 612, 0x2640000, 0x2650000, {0x2640001, 0}};
SoundFiles SOUND_FILES_15 = {701, 697, 0x2B90000, 0x2BD0000, {0x2B90001, 0}};
SoundFiles SOUND_FILES_16 = {517, 516, 0x2040000, 0x2050000, {0x2040001, 0}};
SoundFiles SOUND_FILES_17 = {805, 804, 0x3240000, 0x3250000, {0x3240001, 0}};
SoundFiles SOUND_FILES_18 = {541, 536, 0x2180000, 0x21D0000, {0x2180001, 0}};
SoundFiles SOUND_FILES_19 = {702, 698, 0x2BA0000, 0x2BE0000, {0x2BA0001, 0}};
SoundFiles SOUND_FILES_1A = {703, 699, 0x2BB0000, 0x2BF0000, {0x2BB0001, 0}};
SoundFiles SOUND_FILES_1B = {369, 356, 0x1640000, 0x1710000, {0x1640001, 0}};
SoundFiles SOUND_FILES_1C = {1064, 1062, 0x4260000, 0x4280000, {0x4260001, 0}};
SoundFiles SOUND_FILES_1D = {621, 620, 0x26C0000, 0x26D0000, {0x26C0001, 0}};
SoundFiles SOUND_FILES_1E = {704, 700, 0x2BC0000, 0x2C00000, {0x2BC0001, 0}};
SoundFiles SOUND_FILES_1F = {515, 513, 0x2010000, 0x2030000, {0x2010001, 0}};
SoundFiles SOUND_FILES_20 = {500, 498, 0x1F20000, 0x1F40000, {0x1F20001, 0}};
SoundFiles SOUND_FILES_21 = {720, 719, 0x2CF0000, 0x2D00000, {0x2CF0001, 0}};
SoundFiles SOUND_FILES_22 = {542, 537, 0x2190000, 0x21E0000, {0x2190001, 0}};
SoundFiles SOUND_FILES_23 = {1123, 1121, 0x4610000, 0x4630000, {0x4610001, 0}};
SoundFiles SOUND_FILES_24 = {1360, 1359, 0x54F0000, 0x5500000, {0x54F0001, 0}};
SoundFiles SOUND_FILES_25 = {1979, 1949, 0x79D0000, 0x7BB0000, {0x79D0001, 0}};
SoundFiles SOUND_FILES_26 = {1980, 1976, 0x7B80000, 0x7BC0000, {0x7B80001, 0}};
SoundFiles SOUND_FILES_27 = {1100, 1099, 0x44B0000, 0x44C0000, {0x44B0001, 0}};
SoundFiles SOUND_FILES_28 = {1211, 1210, 0x4BA0000, 0x4BB0000, {0x4BA0001, 0}};
SoundFiles SOUND_FILES_29 = {371, 358, 0x1660000, 0x1730000, {0x1660001, 0}};
SoundFiles SOUND_FILES_2A = {849, 847, 0x34F0000, 0x3510000, {0x34F0001, 0}};
SoundFiles SOUND_FILES_2B = {850, 848, 0x3500000, 0x3520000, {0x3500001, 0}};
SoundFiles SOUND_FILES_2C = {543, 538, 0x21A0000, 0x21F0000, {0x21A0001, 0}};
SoundFiles SOUND_FILES_2D = {372, 359, 0x1670000, 0x1740000, {0x1670001, 0}};
SoundFiles SOUND_FILES_2E = {881, 879, 0x36F0000, 0x3710000, {0x36F0001, 0}};
SoundFiles SOUND_FILES_2F = {882, 880, 0x3700000, 0x3720000, {0x3700001, 0}};
SoundFiles SOUND_FILES_30 = {927, 922, 0x39A0000, 0x39F0000, {0x39A0001, 0}};
SoundFiles SOUND_FILES_31 = {928, 923, 0x39B0000, 0x3A00000, {0x39B0001, 0}};
SoundFiles SOUND_FILES_32 = {373, 360, 0x1680000, 0x1750000, {0x1680001, 0}};
SoundFiles SOUND_FILES_33 = {501, 499, 0x1F30000, 0x1F50000, {0x1F30001, 0}};
SoundFiles SOUND_FILES_34 = {929, 924, 0x39C0000, 0x3A10000, {0x39C0001, 0}};
SoundFiles SOUND_FILES_35 = {930, 925, 0x39D0000, 0x3A20000, {0x39D0001, 0}};
SoundFiles SOUND_FILES_36 = {931, 926, 0x39E0000, 0x3A30000, {0x39E0001, 0}};
SoundFiles SOUND_FILES_37 = {942, 939, 0x3AB0000, 0x3AE0000, {0x3AB0001, 0}};
SoundFiles SOUND_FILES_38 = {943, 940, 0x3AC0000, 0x3AF0000, {0x3AC0001, 0}};
SoundFiles SOUND_FILES_39 = {944, 941, 0x3AD0000, 0x3B00000, {0x3AD0001, 0}};
SoundFiles SOUND_FILES_3A = {998, 992, 0x3E00000, 0x3E60000, {0x3E00001, 0}};
SoundFiles SOUND_FILES_3B = {999, 993, 0x3E10000, 0x3E70000, {0x3E10001, 0}};
SoundFiles SOUND_FILES_3C = {1000, 994, 0x3E20000, 0x3E80000, {0x3E20001, 0}};
SoundFiles SOUND_FILES_3D = {1001, 995, 0x3E30000, 0x3E90000, {0x3E30001, 0}};
SoundFiles SOUND_FILES_3E = {1002, 996, 0x3E40000, 0x3EA0000, {0x3E40001, 0}};
SoundFiles SOUND_FILES_3F = {1003, 997, 0x3E50000, 0x3EB0000, {0x3E50001, 0}};
SoundFiles SOUND_FILES_40 = {1065, 1063, 0x4270000, 0x4290000, {0x4270001, 0}};
SoundFiles SOUND_FILES_41 = {1578, 1577, 0x6290000, 0x62A0000, {0x6290001, 0}};
SoundFiles SOUND_FILES_42 = {544, 539, 0x21B0000, 0x2200000, {0x21B0001, 0}};
SoundFiles SOUND_FILES_43 = {1939, 1938, 0x7920000, 0x7930000, {0x7920001, 0}};
SoundFiles SOUND_FILES_44 = {2087, 2086, 0x8260000, 0x8270000, {0x8260001, 0}};
SoundFiles SOUND_FILES_45 = {1826, 1824, 0x7200000, 0x7220000, {0x7200001, 0}};
SoundFiles SOUND_FILES_46 = {1827, 1825, 0x7210000, 0x7230000, {0x7210001, 0}};
SoundFiles SOUND_FILES_47 = {2186, 2185, 0x8890001, 0x88A0000, {0x8890000, 0}};
#elif VERSION_EU
SoundFiles SOUND_FILES_01 = {384, 371, 0x1730000, 0x1800000, {0x1730001, 0x1730002, 0}};
SoundFiles SOUND_FILES_02 = {375, 362, 0x16A0000, 0x1770000, {0x16A0001, 0}};
SoundFiles SOUND_FILES_03 = {1138, 1136, 0x4700000, 0x4720000, {0x4700001, 0}};
SoundFiles SOUND_FILES_04 = {376, 363, 0x16B0000, 0x1780000, {0x16B0001, 0}};
SoundFiles SOUND_FILES_05 = {377, 364, 0x16C0000, 0x1790000, {0x16C0001, 0}};
SoundFiles SOUND_FILES_06 = {529, 527, 0x20F0000, 0x2110000, {0x20F0001, 0}};
SoundFiles SOUND_FILES_07 = {378, 365, 0x16D0000, 0x17A0000, {0x16D0001, 0}};
SoundFiles SOUND_FILES_08 = {379, 366, 0x16E0000, 0x17B0000, {0x16E0001, 0}};
SoundFiles SOUND_FILES_09 = {380, 367, 0x16F0000, 0x17C0000, {0x16F0001, 0}};
SoundFiles SOUND_FILES_0A = {537, 534, 0x2160000, 0x2190000, {0x2160001, 0}};
SoundFiles SOUND_FILES_0B = {381, 368, 0x1700000, 0x17D0000, {0x1700001, 0}};
SoundFiles SOUND_FILES_0C = {382, 369, 0x1710000, 0x17E0000, {0x1710001, 0}};
SoundFiles SOUND_FILES_0D = {555, 549, 0x2250000, 0x22B0000, {0x2250001, 0}};
SoundFiles SOUND_FILES_0E = {568, 567, 0x2370000, 0x2380000, {0x2370001, 0}};
SoundFiles SOUND_FILES_0F = {578, 577, 0x2410000, 0x2420000, {0x2410001, 0}};
SoundFiles SOUND_FILES_10 = {583, 582, 0x2460000, 0x2470000, {0x2460001, 0}};
SoundFiles SOUND_FILES_11 = {603, 602, 0x25A0000, 0x25B0000, {0x25A0001, 0}};
SoundFiles SOUND_FILES_12 = {609, 608, 0x2600000, 0x2610000, {0x2600001, 0}};
SoundFiles SOUND_FILES_13 = {665, 664, 0x2980000, 0x2990000, {0x2980001, 0}};
SoundFiles SOUND_FILES_14 = {628, 627, 0x2730000, 0x2740000, {0x2730001, 0}};
SoundFiles SOUND_FILES_15 = {716, 712, 0x2C80000, 0x2CC0000, {0x2C80001, 0}};
SoundFiles SOUND_FILES_16 = {532, 531, 0x2130000, 0x2140000, {0x2130001, 0}};
SoundFiles SOUND_FILES_17 = {820, 819, 0x3330000, 0x3340000, {0x3330001, 0}};
SoundFiles SOUND_FILES_18 = {556, 551, 0x2270000, 0x22C0000, {0x2270001, 0}};
SoundFiles SOUND_FILES_19 = {717, 713, 0x2C90000, 0x2CD0000, {0x2C90001, 0}};
SoundFiles SOUND_FILES_1A = {718, 714, 0x2CA0000, 0x2CE0000, {0x2CA0001, 0}};
SoundFiles SOUND_FILES_1B = {383, 370, 0x1720000, 0x17F0000, {0x1720001, 0}};
SoundFiles SOUND_FILES_1C = {1080, 1078, 0x4360000, 0x4380000, {0x4360001, 0}};
SoundFiles SOUND_FILES_1D = {636, 635, 0x27B0000, 0x27C0000, {0x27B0001, 0}};
SoundFiles SOUND_FILES_1E = {719, 715, 0x2CB0000, 0x2CF0000, {0x2CB0001, 0}};
SoundFiles SOUND_FILES_1F = {530, 528, 0x2100000, 0x2120000, {0x2100001, 0}};
SoundFiles SOUND_FILES_20 = {515, 513, 0x2010000, 0x2030000, {0x2010001, 0}};
SoundFiles SOUND_FILES_21 = {735, 734, 0x2DE0000, 0x2DF0000, {0x2DE0001, 0}};
SoundFiles SOUND_FILES_22 = {557, 552, 0x2280000, 0x22D0000, {0x2280001, 0}};
SoundFiles SOUND_FILES_23 = {1139, 1137, 0x4710000, 0x4730000, {0x4710001, 0}};
SoundFiles SOUND_FILES_24 = {1376, 1375, 0x55F0000, 0x5600000, {0x55F0001, 0}};
SoundFiles SOUND_FILES_25 = {1994, 1964, 0x7AC0000, 0x7CA0000, {0x7AC0001, 0}};
SoundFiles SOUND_FILES_26 = {1995, 1991, 0x7C70000, 0x7CB0000, {0x7C70001, 0}};
SoundFiles SOUND_FILES_27 = {1116, 1115, 0x45B0000, 0x45C0000, {0x45B0001, 0}};
SoundFiles SOUND_FILES_28 = {1227, 1226, 0x4CA0000, 0x4CB0000, {0x4CA0001, 0}};
SoundFiles SOUND_FILES_29 = {385, 372, 0x1740000, 0x1810000, {0x1740001, 0}};
SoundFiles SOUND_FILES_2A = {864, 862, 0x35E0000, 0x3600000, {0x35E0001, 0}};
SoundFiles SOUND_FILES_2B = {865, 863, 0x35F0000, 0x3610000, {0x35F0001, 0}};
SoundFiles SOUND_FILES_2C = {558, 553, 0x2290000, 0x22E0000, {0x2290001, 0}};
SoundFiles SOUND_FILES_2D = {386, 373, 0x1750000, 0x1820000, {0x1750001, 0}};
SoundFiles SOUND_FILES_2E = {897, 895, 0x37F0000, 0x3810000, {0x37F0001, 0}};
SoundFiles SOUND_FILES_2F = {898, 896, 0x3800000, 0x3820000, {0x3800001, 0}};
SoundFiles SOUND_FILES_30 = {943, 938, 0x3AA0000, 0x3AF0000, {0x3AA0001, 0}};
SoundFiles SOUND_FILES_31 = {944, 939, 0x3AB0000, 0x3B00000, {0x3AB0001, 0}};
SoundFiles SOUND_FILES_32 = {387, 374, 0x1760000, 0x1830000, {0x1760001, 0}};
SoundFiles SOUND_FILES_33 = {516, 514, 0x2020000, 0x2040000, {0x2020001, 0}};
SoundFiles SOUND_FILES_34 = {945, 940, 0x3AC0000, 0x3B10000, {0x3AC0001, 0}};
SoundFiles SOUND_FILES_35 = {946, 941, 0x3AD0000, 0x3B20000, {0x3AD0001, 0}};
SoundFiles SOUND_FILES_36 = {947, 942, 0x3AE0000, 0x3B30000, {0x3AE0001, 0}};
SoundFiles SOUND_FILES_37 = {958, 955, 0x3BB0000, 0x3BE0000, {0x3BB0001, 0}};
SoundFiles SOUND_FILES_38 = {959, 956, 0x3BC0000, 0x3BF0000, {0x3BC0001, 0}};
SoundFiles SOUND_FILES_39 = {960, 957, 0x3BD0000, 0x3C00000, {0x3BD0001, 0}};
SoundFiles SOUND_FILES_3A = {1014, 1008, 0x3F00000, 0x3F60000, {0x3F00001, 0}};
SoundFiles SOUND_FILES_3B = {1015, 1009, 0x3F10000, 0x3F70000, {0x3F10001, 0}};
SoundFiles SOUND_FILES_3C = {1016, 1010, 0x3F20000, 0x3F80000, {0x3F20001, 0}};
SoundFiles SOUND_FILES_3D = {1017, 1011, 0x3F30000, 0x3F90000, {0x3F30001, 0}};
SoundFiles SOUND_FILES_3E = {1018, 1012, 0x3F40000, 0x3FA0000, {0x3F40001, 0}};
SoundFiles SOUND_FILES_3F = {1019, 1013, 0x3F50000, 0x3FB0000, {0x3F50001, 0}};
SoundFiles SOUND_FILES_40 = {1081, 1079, 0x4370000, 0x4390000, {0x4370001, 0}};
SoundFiles SOUND_FILES_41 = {1594, 1593, 0x6390000, 0x63A0000, {0x6390001, 0}};
SoundFiles SOUND_FILES_42 = {559, 554, 0x22A0000, 0x22F0000, {0x22A0001, 0}};
SoundFiles SOUND_FILES_43 = {1954, 1953, 0x7A10000, 0x7A20000, {0x7A10001, 0}};
SoundFiles SOUND_FILES_44 = {2104, 2103, 0x8370000, 0x8380000, {0x8370001, 0}};
SoundFiles SOUND_FILES_45 = {1842, 1840, 0x7300000, 0x7320000, {0x7300001, 0}};
SoundFiles SOUND_FILES_46 = {1843, 1841, 0x7310000, 0x7330000, {0x7310001, 0}};
SoundFiles SOUND_FILES_47 = {2203, 2202, 0x89A0001, 0x89B0000, {0x89A0000, 0}};
#endif

/* The banks by id: bits 18-24 of a sound id */
SoundFiles *SOUND_BANK_FILES[72] = {
    NULL,
    &SOUND_FILES_01, &SOUND_FILES_02, &SOUND_FILES_03, &SOUND_FILES_04, &SOUND_FILES_05, &SOUND_FILES_06,
    &SOUND_FILES_07, &SOUND_FILES_08, &SOUND_FILES_09, &SOUND_FILES_0A, &SOUND_FILES_0B, &SOUND_FILES_0C,
    &SOUND_FILES_0D, &SOUND_FILES_0E, &SOUND_FILES_0F, &SOUND_FILES_10, &SOUND_FILES_11, &SOUND_FILES_12,
    &SOUND_FILES_13, &SOUND_FILES_14, &SOUND_FILES_15, &SOUND_FILES_16, &SOUND_FILES_17, &SOUND_FILES_18,
    &SOUND_FILES_19, &SOUND_FILES_1A, &SOUND_FILES_1B, &SOUND_FILES_1C, &SOUND_FILES_1D, &SOUND_FILES_1E,
    &SOUND_FILES_1F, &SOUND_FILES_20, &SOUND_FILES_21, &SOUND_FILES_22, &SOUND_FILES_23, &SOUND_FILES_24,
    &SOUND_FILES_25, &SOUND_FILES_26, &SOUND_FILES_27, &SOUND_FILES_28, &SOUND_FILES_29, &SOUND_FILES_2A,
    &SOUND_FILES_2B, &SOUND_FILES_2C, &SOUND_FILES_2D, &SOUND_FILES_2E, &SOUND_FILES_2F, &SOUND_FILES_30,
    &SOUND_FILES_31, &SOUND_FILES_32, &SOUND_FILES_33, &SOUND_FILES_34, &SOUND_FILES_35, &SOUND_FILES_36,
    &SOUND_FILES_37, &SOUND_FILES_38, &SOUND_FILES_39, &SOUND_FILES_3A, &SOUND_FILES_3B, &SOUND_FILES_3C,
    &SOUND_FILES_3D, &SOUND_FILES_3E, &SOUND_FILES_3F, &SOUND_FILES_40, &SOUND_FILES_41, &SOUND_FILES_42,
    &SOUND_FILES_43, &SOUND_FILES_44, &SOUND_FILES_45, &SOUND_FILES_46, &SOUND_FILES_47,
};

/* Each slot's buffer for the VAB header and the SEPs, in this unit's .bss (see the Makefile) */
s32 SOUND_HEAD_BUFFER_0[14336];
s32 SOUND_HEAD_BUFFER_1[10240];
s32 SOUND_HEAD_BUFFER_2[10240];

/* Each slot's buffer, and its SPU address */
void *SOUND_HEAD_BUFFERS[SOUND_SLOT_COUNT] = {SOUND_HEAD_BUFFER_0, SOUND_HEAD_BUFFER_1, SOUND_HEAD_BUFFER_2};
s32 SOUND_SPU_ADDRS[SOUND_SLOT_COUNT] = {0x1010, 0x49C10, 0x62410};

/* rsin and rcos (libgte) read rsin_tbl[a - 0x800] and the like: their
   addresses before the table fall in seqTable */
SoundState SOUND = {
    {0},
    {{0}},
    0,
    0,
    {NULL},
    initSound,
    playSound,
    soundKeyOn,
    soundKeyOff,
    loadSoundBank,
    loadSoundBankInto,
    updateSoundLoading,
    isSoundLoading,
    stopAllSounds,
    stopSound,
    fadeOutSound,
};

/* The slot (0-2) that holds bank `id`, or -1 */
s32 findSoundBank(s32 id) {
    s32 i;

    for (i = 0; i < SOUND_SLOT_COUNT; i++) {
        if (SOUND.banks[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Plays a packed sound id (see engine/sound.h); returns the voice of a key-on */
short playSound(s32 packed) {
    s32 id = SOUND_BANK(packed);
    u32 keyOn = SOUND_IS_KEY_ON(packed);
    s32 exclusive = SOUND_IS_EXCLUSIVE(packed);
    s32 prog = SOUND_PROG(packed);
    s32 tone = SOUND_TONE(packed);
    s32 note = SOUND_NOTE(packed);
    s32 seq = SOUND_SEQ(packed);
    s32 sep = SOUND_SEP(packed);
    s32 slot = findSoundBank(id);
    s32 voice = -1;

    if (slot != -1) {
        if (slot != 0) {
            SOUND.lastSlot = slot;
        }
        if (exclusive) {
            if (SOUND.music == packed) {
                return 0;
            }
            if (SOUND.music != 0) {
                SOUND.stopSound(SOUND.music);
            }
            SOUND.music = packed;
        }
        if (keyOn) {
            voice = SsUtKeyOn(SOUND.banks[slot].vabId, prog, tone, note, 0, SOUND_VOLUME_MAX, SOUND_VOLUME_MAX);
        } else {
            id = sep;
            SsSepStop(SOUND.banks[slot].seqs[seq], id);
            SsSepSetVol(SOUND.banks[slot].seqs[seq], id, SOUND_VOLUME_MAX, SOUND_VOLUME_MAX);
            SsSepPlay(SOUND.banks[slot].seqs[seq], id, 1, 1);
        }
        return voice;
    }
    return 0;
}

/* Stops every track of the loaded banks and every voice */
void stopAllSounds(void) {
    s32 i;
    s32 j;
    s32 k;
    SoundBank *bank;

    for (i = 0; i < SOUND_SLOT_COUNT; i++) {
        bank = &SOUND.banks[i];
        if (bank->vabId != -1) {
            for (j = 0; j < bank->numSeqs; j++) {
                for (k = 0; k < SOUND_SEP_COUNT; k++) {
                    SsSepStop(bank->seqs[j], k);
                }
            }
        }
    }
    SsUtAllKeyOff(0);
    SOUND.music = 0;
}

/* Stops a sequence sound (key-on sounds are left alone) */
void stopSound(s32 packed) {
    s32 id = SOUND_BANK(packed);
    u32 keyOn = SOUND_IS_KEY_ON(packed);
    s32 seq = SOUND_SEQ(packed);
    s32 sep = SOUND_SEP(packed);
    s32 slot = findSoundBank(id);

    if (slot != -1 && !keyOn) {
        SsSepStop(SOUND.banks[slot].seqs[seq], sep);
        if (SOUND.music == packed) {
            SOUND.music = 0;
        }
    }
}

/* Fades a sequence sound out over a second (key-on sounds are left alone) */
void fadeOutSound(s32 packed) {
    s32 id = SOUND_BANK(packed);
    u32 keyOn = SOUND_IS_KEY_ON(packed);
    s32 seq = SOUND_SEQ(packed);
    s32 sep = SOUND_SEP(packed);
    s32 slot = findSoundBank(id);

    if (slot != -1 && !keyOn) {
#if PAL_VIDEO
        /* one second, in frames */
        SsSepSetDecrescendo(SOUND.banks[slot].seqs[seq], sep, 0x80, NTSC_MODE ? 0x3C : 0x32);
#else
        SsSepSetDecrescendo(SOUND.banks[slot].seqs[seq], sep, 0x80, 0x3C);
#endif
        if (SOUND.music == packed) {
            SOUND.music = 0;
        }
    }
}

/* Whether a bank is still being loaded */
s32 isSoundLoading(void) {
    return SOUND.loader.state != SOUND_LOAD_IDLE;
}

/* Closes what slot `slot` holds and starts loading bank `id` into it */
void loadSoundBankInto(s32 slot, s32 id) {
    SoundBank *bank = &SOUND.banks[slot];
    SoundLoader *loader = &SOUND.loader;
    s32 i;
    s32 j;

    bank->id = id;
    if (bank->vabId != -1) {
        for (i = 0; i < bank->numSeqs; i++) {
            for (j = 0; j < SOUND_SEP_COUNT; j++) {
                SsSepStop(bank->seqs[i], j);
            }
            SsSepClose(bank->seqs[i]);
        }
        SsVabClose(bank->vabId);
        bank->vabId = -1;
    }
    loader->state = SOUND_LOAD_HEAD;
    loader->files = SOUND_BANK_FILES[id];
    loader->slot = slot;
    FILE_CACHE.request(loader->files->headFile);
}

/* Loads a bank into slot 1 or 2, whichever was not used last */
void loadSoundBank(s32 id) {
    if (SOUND.banks[1].id != id && SOUND.banks[2].id != id) {
        if (SOUND.lastSlot == 1) {
            loadSoundBankInto(2, id);
            SOUND.lastSlot = 2;
        } else {
            loadSoundBankInto(1, id);
            SOUND.lastSlot = 1;
        }
    }
}

/* Header: copied to the slot's buffer, then the body goes to the SPU */
void updateSoundLoading(void) {
    SoundLoader *loader = &SOUND.loader;
    s32 slot = loader->slot;
    SoundBank *bank = &SOUND.banks[slot];
    s32 *src;
    s32 *dst;
    s32 n;
    s32 i;
    s32 j;

    switch (loader->state) {
    case SOUND_LOAD_IDLE:
        break;
    case SOUND_LOAD_HEAD:
        if (FILE_CACHE.isLoading(loader->files->headFile) != 0) {
            return;
        }
        src = FILE_CACHE.load(loader->files->headFile);
        dst = bank->headBuffer;
        n = FILE_TABLE.getSectorCount(loader->files->headFile) * (CD_SECTOR_SIZE / sizeof(s32));
        for (j = 0; j < n; j++) {
            *dst++ = *src++;
        }
        FILE_CACHE.free(loader->files->headFile);
        bank->vabId = SsVabOpenHeadSticky(FILE_CACHE.getArchiveEntry(loader->files->vhIndex, bank->headBuffer), slot, bank->spuAddr);
        FILE_CACHE.request(loader->files->bodyFile);
        loader->state++;
        /* fallthrough */
    case SOUND_LOAD_BODY:
        if (FILE_CACHE.isLoading(loader->files->bodyFile) != 0) {
            return;
        }
        HEAP.lock(FILE_CACHE.load(loader->files->bodyFile), 1);
        bank->vabId = SsVabTransBody(FILE_CACHE.getEntry(loader->files->bodyEntry), bank->vabId);
        loader->state++;
        /* fallthrough */
    case SOUND_LOAD_TRANSFER:
        if (SsVabTransCompleted(0) == 0) {
            return;
        }
        i = 0;
        HEAP.lock(FILE_CACHE.load(loader->files->bodyFile), 0);
        FILE_CACHE.free(loader->files->bodyFile);
        for (; loader->files->seps[i] != 0; i++) {
            bank->seqs[i] = SsSepOpen(FILE_CACHE.getArchiveEntry(loader->files->seps[i], bank->headBuffer), bank->vabId, SOUND_SEP_COUNT);
        }
        bank->numSeqs = i;
        loader->state = SOUND_LOAD_IDLE;
    }
}

/* Plays one note of a program from the bank in slot `slot`; returns the voice */
short soundKeyOn(s32 slot, short prog, short note) {
    return SsUtKeyOn(SOUND.banks[slot].vabId, prog, 0, note, 0, SOUND_VOLUME_MAX, SOUND_VOLUME_MAX);
}

/* Releases the voice that a key-on sound id started */
void soundKeyOff(s32 packed, s16 voice) {
    s32 id = SOUND_BANK(packed);
    s32 prog = SOUND_PROG(packed);
    s32 tone = SOUND_TONE(packed);
    s32 note = SOUND_NOTE(packed);
    s32 slot = findSoundBank(id);

    if (voice != -1 && slot != -1) {
        SsUtKeyOff(voice, SOUND.banks[slot].vabId, prog, tone, note);
    }
}

/* Starts libsnd, gives each slot its buffers and loads the system sounds (bank 1) into slot 0 */
void initSound(void) {
    s32 i;

    SsSetTableSize(SOUND.seqTable, 6, 16);
#if PAL_VIDEO
    if (NTSC_MODE) {
        SsSetTickMode(SS_NOTICK);
    } else {
        SsSetTickMode(SS_NOTICK | 50); /* 50 ticks a second */
    }
#else
    SsSetTickMode(SS_NOTICK);
#endif
    SsStart2();
    SsSetMVol(SOUND_VOLUME_MAX, SOUND_VOLUME_MAX);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, SOUND_VOLUME_MAX, SOUND_VOLUME_MAX);
    SsUtSetReverbType(SS_REV_TYPE_STUDIO_B);
    SsUtSetReverbDepth(0, 0);
    SsUtReverbOn();
    for (i = 0; i < SOUND_SLOT_COUNT; i++) {
        SOUND.banks[i].vabId = -1;
        SOUND.banks[i].numSeqs = 0;
        SOUND.banks[i].headBuffer = SOUND_HEAD_BUFFERS[i];
        SOUND.banks[i].spuAddr = SOUND_SPU_ADDRS[i];
    }
    SOUND.loader.slot = 0;
    SOUND.loader.state = SOUND_LOAD_IDLE;
    SOUND.loader.files = NULL;
    loadSoundBankInto(0, 1);
    while (isSoundLoading() != 0) {
        FILE_CACHE.update();
        updateSoundLoading();
    }
}
