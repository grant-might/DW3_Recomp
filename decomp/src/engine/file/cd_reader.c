#include "engine/game.h"
#include <libgs.h>
#include <libetc.h>
#include <libsnd.h>

/* -G8 unit: small variables defined here are reached through $gp */
static u_char CD_MODE[8];

/* The CD reader */
CdReader CD_READER = {
    0, 0, 0, 0, 0, NULL, {0}, 0, 0, 0, 0,
    isCdReading,
    readFile,
};

/* Checks that the sector just read is the next one */
s32 cdCheckSector(void) {
    s32 pos;

    CdGetSector(&CD_SECTOR_HEADER, 3);
    pos = CdPosToInt(&CD_SECTOR_HEADER.pos);
    if (pos == CD_READER.nextSector) {
        CD_READER.nextSector = pos + 1;
        return 0;
    }
    return -1;
}

/*
 * The CdReadyCallback: copies each sector as it arrives, pauses at the end.
 * The two callbacks take the status as an int, not CdlCB's u_char, which
 * GCC would truncate on entry (more code), so they are registered with a cast.
 */
void cdReadyCallback(s32 status, u_char *result) {
    if (status == CdlDataReady) {
        if (cdCheckSector() != 0) {
            goto error;
        }
        CdGetSector(CD_READER.dst, CD_SECTOR_SIZE / sizeof(u_long));
        CD_READER.dst += CD_SECTOR_SIZE;
        if (--CD_READER.sectorsLeft != 0) {
            return;
        }
    } else {
    error:
        CD_READER.sectorsLeft = -1;
    }
    CdReadyCallback(NULL);
    CdControlF(CdlPause, 0);
}

/* The CdSyncCallback: Setloc, Setmode (double speed, whole sectors), ReadN,
   and CdlPause once the sectors are in */
void cdSyncCallback(s32 status, u_char *result) {
    switch (status) {
    case CdlDiskError:
        if (CD_READER.state == CD_READ_SECTORS) {
            CdControlF(CdlPause, 0);
        } else {
            startCdRead();
        }
        break;
    case CdlComplete:
        switch (CD_READER.state) {
        case CD_READ_SETLOC:
            CD_MODE[0] = CdlModeSpeed | CdlModeSize1;
            CdControlF(CdlSetmode, CD_MODE);
            CD_READER.state++;
            break;
        case CD_READ_SETMODE:
            CdReadyCallback((CdlCB)cdReadyCallback);
            CdControlF(CdlReadN, 0);
            CD_READER.state++;
            break;
        case CD_READ_READN:
            CD_READER.state = CD_READ_SECTORS;
            break;
        case CD_READ_SECTORS:
            CdSyncCallback(NULL);
            if (CD_READER.sectorsLeft == 0) {
                CD_READER.state = CD_READ_IDLE;
                if (CD_READER.done != NULL) {
                    *CD_READER.done = 1;
                }
            } else {
                startCdRead();
            }
            break;
        }
        break;
    }
}

/* Whether the CD reader is busy */
s32 isCdReading(void) {
    return CD_READER.state != CD_READ_IDLE;
}

/* Starts the read readFile set up: seeks to its first sector */
void startCdRead(void) {
    CD_READER.state = CD_READ_SETLOC;
    CD_READER.nextSector = CD_READER.sector;
    CD_READER.dst = CD_READER.buffer;
    CD_READER.sectorsLeft = CD_READER.sectorCount;
    CdSyncCallback((CdlCB)cdSyncCallback);
    CdControlF(CdlSetloc, CD_READER.loc);
}

/* Starts reading `size` sectors (0: all) of a file into buffer; ignored while busy */
void readFile(s32 file, s32 offset, s32 size, void *buffer, s32 *done) {
    if (isCdReading() == 0) {
        CD_READER.file = file;
        CD_READER.offset = offset;
        CD_READER.buffer = buffer;
        CD_READER.done = done;
        if (done != NULL) {
            *done = 0;
        }
        if (size == 0) {
            CD_READER.sectorCount = FILE_TABLE.getSectorCount(file);
        } else {
            CD_READER.sectorCount = size;
        }
        FILE_TABLE.getPos(file, offset, CD_READER.loc);
        CD_READER.sector = FILE_TABLE.getSector(file) + offset;
        startCdRead();
    }
}
