#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* Starts libmcrd and sets up MEMCARD: the save file name and the section sizes */
void initMemCard(void) {
    MemCardInit(0);
    MemCardStart();
    HEAP.zero(&MEMCARD, sizeof(MemCard));
    MEMCARD.maxRetries = 3;
    MEMCARD.iconCount = -1;
    setSaveFileName();
    MEMCARD.infoSize = 0x100;
    MEMCARD.dataSize = 0x2700;
}

#if VERSION_US
const char SAVE_FILE_NAME_JP[24] = "BISLPS-99999DMW3-JPN";

const char SAVE_FILE_NAMES[6][24] = {
    "BASLUS-01436DMW3-USA",
    "BESLPS-99999DMW3-ENG",
    "BESLPS-99999DMW3-FRA",
    "BESLPS-99999DMW3-ITA",
    "BESLPS-99999DMW3-GER",
    "BESLPS-99999DMW3-SPN",
};
#elif VERSION_JP
/* Only the first is read: the second is the USA release's name, the third
   the placeholder the USA version keeps as SAVE_FILE_NAME_JP */
const char SAVE_FILE_NAMES[3][24] = {
    "BISLPS-03446DMW3-JPN",
    "BASLUS-01436DMW3-USA",
    "BISLPS-99999DMW3-JPN",
};
#endif

#if VERSION_US || VERSION_JP
/* The release saves as its own name, BASLUS-01436DMW3-USA in the US one
   and BISLPS-03446DMW3-JPN in the Japanese one */
void setSaveFileName(void) {
    MEMCARD.fileName = SAVE_FILE_NAMES[0];
}
#elif VERSION_EU
const char SAVE_FILE_NAME_JP[24] = "BISLPS-03446DMW3-JPN";

const char SAVE_FILE_NAMES[2][24] = {
    "BASLUS-01436DMW3-USA",
    "BESLES-03936DMW3-EUR",
};

/*
 * The save file name by the language: the Japanese, US or European
 * release's, the same for each European language
 */
void setSaveFileName(void) {
    switch (LANGUAGE) {
    default:
    case 0:
        MEMCARD.fileName = SAVE_FILE_NAME_JP;
        break;
    case 1:
        MEMCARD.fileName = SAVE_FILE_NAMES[0];
        break;
    case 2:
        MEMCARD.fileName = SAVE_FILE_NAMES[1];
        break;
    case 3:
        MEMCARD.fileName = SAVE_FILE_NAMES[1];
        break;
    case 4:
        MEMCARD.fileName = SAVE_FILE_NAMES[1];
        break;
    case 5:
        MEMCARD.fileName = SAVE_FILE_NAMES[1];
        break;
    case 6:
        MEMCARD.fileName = SAVE_FILE_NAMES[1];
        break;
    }
}
#endif

/* The save file header: title (Shift-JIS), icon CLUT and 1-3 icon frames */
void setSaveHeader(char *title, CardClut *clut, s32 count, void **icons) {
    s32 i;

    if (count >= 1 && count <= SAVE_MAX_ICONS && (u32)strlen(title) <= sizeof(MEMCARD.header.title)) {
        MEMCARD.iconCount = count;
        HEAP.zero(&MEMCARD.header, sizeof(CardHeader));
        MEMCARD.header.magic[0] = 'S';
        MEMCARD.header.magic[1] = 'C';
        MEMCARD.header.blocks = SAVE_BLOCKS;
        MEMCARD.header.type = MEMCARD.iconCount | CARD_HEADER_ICONS;
        strcpy(MEMCARD.header.title, title);
        MEMCARD.header.clut = *clut;
        for (i = 0; i < MEMCARD.iconCount; i++) {
            MEMCARD.icons[i] = icons[i];
        }
    }
}

/* MemCardSync, retrying a failed command up to maxRetries times */
s32 syncMemCard(void) {
    long cmds;
    u_long result;
    s32 ret = MemCardSync(1, &cmds, &result);

    if (ret == 1) {
        MEMCARD.cmd = cmds;
        MEMCARD.result = result;
        if (result <= CARD_ERR_NO_CARD || result == CARD_ERR_NEW_CARD) {
            MEMCARD.retries = 0;
        } else {
            if (++MEMCARD.retries < MEMCARD.maxRetries) {
                MEMCARD.restart = ret;
                return 0;
            }
            MEMCARD.retries = 0;
            MEMCARD.restart = 0;
        }
    }
    return ret;
}

/* Checks whether a card is in port `port` (MemCardExist) */
s32 checkMemCard(s32 port) {
    switch (MEMCARD.state) {
    case MEMCARD_IDLE:
    default:
        while (MemCardExist(port << 4) == 0) {
            syncMemCard();
        }
        MEMCARD.state = MEMCARD_CHECKING;
        break;
    case MEMCARD_CHECKING:
        if (syncMemCard() != 0) {
            MEMCARD.state = MEMCARD_IDLE;
            if (MEMCARD.result == CARD_ERR_NONE) {
                return 1;
            }
            return MEMCARD.result + 1;
        }
        if (MEMCARD.restart != 0) {
            MEMCARD.restart = 0;
            while (MemCardExist(port << 4) == 0) {
                syncMemCard();
            }
        }
        break;
    }
    return 0;
}

/*
 * Checks a card in port `port` in full, finding out whether it is new or unformatted
 * (MemCardAccept)
 */
s32 acceptMemCard(s32 port) {
    switch (MEMCARD.state) {
    case MEMCARD_IDLE:
    default:
        while (MemCardAccept(port << 4) == 0) {
            syncMemCard();
        }
        MEMCARD.state = MEMCARD_ACCEPTING;
        break;
    case MEMCARD_ACCEPTING:
        if (syncMemCard() != 0) {
            MEMCARD.state = MEMCARD_IDLE;
            if (MEMCARD.result == CARD_ERR_NONE) {
                return 1;
            }
            return MEMCARD.result + 1;
        }
        if (MEMCARD.restart != 0) {
            MEMCARD.restart = 0;
            while (MemCardAccept(port << 4) == 0) {
                syncMemCard();
            }
        }
        break;
    }
    return 0;
}

/* Reads a section of the save (SAVE_SECTION_*, then 2-4 for the data), a sector a call */
s32 readSave(s32 port, void *buf, s32 size, s32 section) {
    u8 *dst;

    if (buf == NULL || size == 0) {
        return 1;
    }
    if ((u32)(MEMCARD.iconCount - 1) >= SAVE_MAX_ICONS) {
        return 1;
    }
    dst = buf;
    switch (MEMCARD.state) {
    case MEMCARD_IDLE:
    default:
        if (checkMemCard(port) == 0) {
            return 0;
        }
        switch (MEMCARD.result) {
        case CARD_ERR_NONE:
            MEMCARD.progress = 0;
            switch (section) {
            case SAVE_SECTION_HEADER:
            default:
                MEMCARD.offset = 0;
                break;
            case SAVE_SECTION_INFO:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE;
                break;
            case SAVE_SECTION_DATA:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize;
                break;
            case SAVE_SECTION_DATA + 1:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize + MEMCARD.dataSize;
                break;
            case SAVE_SECTION_DATA + 2:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize + MEMCARD.dataSize * 2;
                break;
            }
            while (MemCardReadFile(port << 4, MEMCARD.fileName, dst, MEMCARD.offset, CARD_SECTOR_SIZE) == 0) {
                syncMemCard();
            }
            MEMCARD.state = MEMCARD_READING;
            break;
        default:
            MEMCARD.state = MEMCARD_IDLE;
            return MEMCARD.result + 1;
        }
        break;
    case MEMCARD_READING:
        if (syncMemCard() != 0) {
            switch (MEMCARD.result) {
            case CARD_ERR_NONE:
                MEMCARD.progress += CARD_SECTOR_SIZE;
                if (MEMCARD.progress >= size) {
                    MEMCARD.state = MEMCARD_IDLE;
                    return 1;
                }
                while (1) {
                    if (MemCardReadFile(port << 4, MEMCARD.fileName, dst + MEMCARD.progress, MEMCARD.offset + MEMCARD.progress, CARD_SECTOR_SIZE) != 0) {
                        return 0;
                    }
                    syncMemCard();
                }
            default:
                MEMCARD.state = MEMCARD_IDLE;
                return MEMCARD.result + 1;
            }
        }
        if (MEMCARD.restart != 0) {
            MEMCARD.restart = 0;
            MEMCARD.progress = 0;
            while (MemCardReadFile(port << 4, MEMCARD.fileName, dst, MEMCARD.offset, CARD_SECTOR_SIZE) == 0) {
                syncMemCard();
            }
            return 0;
        }
        break;
    }
    return 0;
}

/* Writes a section (as readSave; section 0 takes the offset from bits 8 and up) */
s32 writeSave(s32 port, void *buf, s32 size, s32 section) {
    u8 *dst;

    if (buf == NULL || size == 0) {
        return 1;
    }
    if ((u32)(MEMCARD.iconCount - 1) >= SAVE_MAX_ICONS) {
        return 1;
    }
    dst = buf;
    switch (MEMCARD.state) {
    case MEMCARD_IDLE:
    default:
        if (checkMemCard(port) == 0) {
            return 0;
        }
        switch (MEMCARD.result) {
        case CARD_ERR_NONE:
            MEMCARD.progress = 0;
            switch (section & 0xFF) {
            case SAVE_SECTION_HEADER:
            default:
                MEMCARD.offset = section >> 8;
                break;
            case SAVE_SECTION_INFO:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE;
                break;
            case SAVE_SECTION_DATA:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize;
                break;
            case SAVE_SECTION_DATA + 1:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize + MEMCARD.dataSize;
                break;
            case SAVE_SECTION_DATA + 2:
                MEMCARD.offset = MEMCARD.iconCount * CARD_SECTOR_SIZE + CARD_SECTOR_SIZE + MEMCARD.infoSize + MEMCARD.dataSize * 2;
                break;
            }
            while (MemCardWriteFile(port << 4, MEMCARD.fileName, dst, MEMCARD.offset, CARD_SECTOR_SIZE) == 0) {
                syncMemCard();
            }
            MEMCARD.state = MEMCARD_WRITING;
            break;
        default:
            MEMCARD.state = MEMCARD_IDLE;
            return MEMCARD.result + 1;
        }
        break;
    case MEMCARD_WRITING:
        if (syncMemCard() != 0) {
            switch (MEMCARD.result) {
            case CARD_ERR_NONE:
                MEMCARD.progress += CARD_SECTOR_SIZE;
                if (MEMCARD.progress >= size) {
                    MEMCARD.state = MEMCARD_IDLE;
                    return 1;
                }
                while (1) {
                    if (MemCardWriteFile(port << 4, MEMCARD.fileName, dst + MEMCARD.progress, MEMCARD.offset + MEMCARD.progress, CARD_SECTOR_SIZE) != 0) {
                        return 0;
                    }
                    syncMemCard();
                }
            default:
                MEMCARD.state = MEMCARD_IDLE;
                return MEMCARD.result + 1;
            }
        }
        if (MEMCARD.restart != 0) {
            MEMCARD.restart = 0;
            MEMCARD.progress = 0;
            while (MemCardWriteFile(port << 4, MEMCARD.fileName, dst, MEMCARD.offset, CARD_SECTOR_SIZE) == 0) {
                syncMemCard();
            }
            return 0;
        }
        break;
    }
    return 0;
}

/* Reads the directory, creates the save file, formats or unformats (MEMCARD_OP_*) */
s32 memCardCommand(s32 port, s32 cmd) {
    switch (MEMCARD.state) {
    case MEMCARD_IDLE:
    default:
        if (acceptMemCard(port) == 0) {
            return 0;
        }
        switch (MEMCARD.result) {
        case CARD_ERR_NONE:
            MEMCARD.state = MEMCARD_COMMAND;
            break;
        case CARD_ERR_UNFORMATTED:
            if (cmd == MEMCARD_OP_FORMAT) {
                MEMCARD.state = MEMCARD_COMMAND;
                break;
            }
            MEMCARD.state = MEMCARD_IDLE;
            return CARD_ERR_UNFORMATTED + 1;
        default:
            MEMCARD.state = MEMCARD_IDLE;
            return MEMCARD.result + 1;
        }
        break;
    case MEMCARD_COMMAND:
        if ((MEMCARD.result == CARD_ERR_NONE && (cmd == MEMCARD_OP_LIST || cmd == MEMCARD_OP_CREATE || cmd == MEMCARD_OP_UNFORMAT)) ||
            (MEMCARD.result == CARD_ERR_UNFORMATTED && cmd == MEMCARD_OP_FORMAT)) {
            HEAP.zero(&MEMCARD.fileCount, sizeof(MEMCARD.fileCount) + sizeof(MEMCARD.files));
            MEMCARD.cmd = MEMCARD_SYNC_CMDS[cmd];
            switch (cmd) {
            case MEMCARD_OP_LIST:
            default:
                MEMCARD.result = MemCardGetDirentry(port << 4, STR_ALL_FILES, MEMCARD.files, &MEMCARD.fileCount, 0, CARD_MAX_FILES);
                break;
            case MEMCARD_OP_CREATE:
                MEMCARD.result = MemCardCreateFile(port << 4, MEMCARD.fileName, SAVE_BLOCKS);
                break;
            case MEMCARD_OP_FORMAT:
                MEMCARD.result = MemCardFormat(port << 4);
                break;
            case MEMCARD_OP_UNFORMAT:
                MEMCARD.result = MemCardUnformat(port << 4);
                break;
            }
            if (MEMCARD.result == -1) {
                MEMCARD.result = CARD_ERR_NOT_STARTED;
            }
        }
        MEMCARD.state = MEMCARD_IDLE;
        if (MEMCARD.result == CARD_ERR_NONE) {
            return 1;
        }
        return MEMCARD.result + 1;
    }
    return 0;
}

/* Reads the card's directory into MEMCARD.files */
s32 listSaves(s32 port) {
    s32 ret = memCardCommand(port, MEMCARD_OP_LIST);

    if (ret == 1) {
        return 1;
    }
    return ret;
}

/* Creates the save file on the card */
s32 createSave(s32 port) {
    s32 ret = memCardCommand(port, MEMCARD_OP_CREATE);

    if (ret == 1) {
        return 1;
    }
    return ret;
}

/* Formats an unformatted card */
s32 formatMemCard(s32 port) {
    s32 ret = memCardCommand(port, MEMCARD_OP_FORMAT);

    if (ret == 1) {
        return 1;
    }
    return ret;
}

/* memCardCommand's MEMCARD_OP_UNFORMAT, left out: always fails as if there were no card */
s32 unformatMemCard(void) {
    return CARD_ERR_NO_CARD + 1;
}

/* XOR of every byte */
s32 verifyChecksum(void *data, s32 size, char expected) {
    u8 *byte = data;
    u8 sum = 0;
    s32 i;

    for (i = 0; i < size; i++) {
        sum ^= *byte++;
    }
    return ((expected ^ sum) & 0xFF) == 0;
}

/* The XOR of every byte, which verifyChecksum checks */
u8 computeChecksum(void *data, s32 size) {
    u8 *byte = data;
    u8 sum = 0;
    s32 i;

    for (i = 0; i < size; i++) {
        sum ^= *byte++;
    }
    return sum;
}

MemCard MEMCARD = { 0 };

MemCardFuncs MEMCARD_FUNCS = {
    initMemCard, setSaveFileName, setSaveHeader, checkMemCard, acceptMemCard, readSave, writeSave,
    listSaves, createSave, formatMemCard, unformatMemCard, verifyChecksum, computeChecksum,
};

/* The commands of memCardCommand's operations */
s32 MEMCARD_SYNC_CMDS[MEMCARD_OP_UNFORMAT + 1] = { 7, 8, 9, 10 };
