/* The movies, streamed from the disc and decoded with libpress */

#include "menus/title.h"

/* Resets the GPU and clears the whole VRAM to black, around the movies */
void STDWTITL_clearVram(void) {
    ResetGraph(1);
    ClearImage2(&STDWTITL_vramRect, 0, 0, 0);
    DrawSync(0);
}

/* Sets up the movie decoder: its VLC and slice buffers, and the two places in VRAM
   (x0, y0 and x1, y1) its frames alternate between */
void STDWTITL_initDecEnv(DecEnv *dec, s16 x0, s16 y0, s16 x1, s16 y1) {
    dec->vlcbuf[0] = STDWTITL_vlcBuffer0;
    dec->vlcbuf[1] = STDWTITL_vlcBuffer1;
    dec->vlcid = GFX.buffer ^ 1;
    dec->imgbuf[0] = STDWTITL_imageBuffer0;
    dec->imgbuf[1] = STDWTITL_imageBuffer1;
    dec->imgid = GFX.buffer ^ 1;
    dec->rect[0].x = x0;
    dec->rect[0].y = y0;
    dec->rect[1].x = x1;
    dec->rect[1].y = y1;
    dec->rectid = GFX.buffer ^ 1;
    dec->slice.x = x0;
    dec->slice.y = y0;
    dec->slice.w = 24;
    dec->isdone = 0;
}

/* Seeks the CD to loc and starts streaming the movie from there, retrying until it works */
void STDWTITL_readStream(CdlLOC *loc) {
    u_char param;

    param = CdlModeSpeed;
    do {
        /* libcd takes a command's parameters as bytes */
        while (CdControl(CdlSetloc, (u_char *)loc, 0) == 0) {
        }
        while (CdControl(CdlSetmode, &param, 0) == 0) {
        }
    } while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSize1) == 0);
}

/* Resets the MDEC, has callback run for each decoded slice, sets up the ring buffer
   and starts streaming the movie from loc */
void STDWTITL_initStream(CdlLOC *loc, void (*callback)()) {
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(STDWTITL_ringBuffer, 32);
    StSetStream(1, 1, -1, 0, 0);
    STDWTITL_readStream(loc);
}

/* Waits for the next movie frame in the ring buffer and returns it (NULL if none
   came); flags the movie's end at its last frame and clears VRAM when the size changes */
u_long *STDWTITL_getNextFrame(DecEnv *dec) {
    u_long *addr;
    StHEADER *sector;
    s32 count = 2000;

    /* libcd hands the sector's header back as words */
    while (StGetNext(&addr, (u_long **)&sector) != 0) {
        if (--count == 0) {
            return NULL;
        }
    }
    if (sector->frameCount >= STDWTITL_movieEndFrame) {
        STDWTITL_movieEnded = 1;
    }
    if (STDWTITL_movieWidth != sector->width || STDWTITL_movieHeight != sector->height) {
        STDWTITL_clearVram();
        STDWTITL_movieWidth = sector->width;
        STDWTITL_movieHeight = sector->height;
    }
    dec->rect[0].w = dec->rect[1].w = STDWTITL_movieWidth * 3 / 2;
    dec->rect[0].h = dec->rect[1].h = STDWTITL_movieHeight;
    dec->slice.h = STDWTITL_movieHeight;
    return addr;
}

/* Takes the next movie frame and decodes its VLC data into the other VLC buffer;
   -1 if no frame came, 0 otherwise */
s32 STDWTITL_decodeNextFrame(DecEnv *dec) {
    s32 count = 2000;
    u_long *next;

    while ((next = STDWTITL_getNextFrame(dec)) == NULL) {
        if (--count == 0) {
            return -1;
        }
    }
    dec->vlcid = dec->vlcid == 0;
    DecDCTvlc2(next, dec->vlcbuf[dec->vlcid], STDWTITL_vlcTable);
    StFreeRing(next);
    return 0;
}

/* Called by the MDEC for each decoded slice of a movie frame: copies it to VRAM
   and decodes the next one, or marks the frame done and switches to the other buffer */
void STDWTITL_onSliceDecoded(void) {
    RECT rect;
    s32 id;

    if (StCdIntrFlag) {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    id = STDWTITL_decEnv.imgid;
    rect = STDWTITL_decEnv.slice;
    STDWTITL_decEnv.imgid = STDWTITL_decEnv.imgid == 0;
    STDWTITL_decEnv.slice.x += STDWTITL_decEnv.slice.w;
    if (STDWTITL_decEnv.rectid) {
        rect.x += 480;
    }
    rect.y = 36;
    if (STDWTITL_decEnv.slice.x < STDWTITL_decEnv.rect[STDWTITL_decEnv.rectid].x + STDWTITL_decEnv.rect[STDWTITL_decEnv.rectid].w) {
        DecDCTout(STDWTITL_decEnv.imgbuf[STDWTITL_decEnv.imgid], STDWTITL_decEnv.slice.w * STDWTITL_decEnv.slice.h / 2);
    } else {
        STDWTITL_decEnv.isdone = 1;
        STDWTITL_decEnv.rectid = STDWTITL_decEnv.rectid == 0;
        STDWTITL_decEnv.slice.x = STDWTITL_decEnv.rect[STDWTITL_decEnv.rectid].x;
        STDWTITL_decEnv.slice.y = STDWTITL_decEnv.rect[STDWTITL_decEnv.rectid].y;
    }
    DrawSync(0);
    LoadImage(&rect, STDWTITL_decEnv.imgbuf[id]);
}

/* Waits until the MDEC has decoded the whole frame (switching buffers itself if it
   takes far too long); mode is not used */
void STDWTITL_waitFrameDecoded(DecEnv *dec, s32 mode) {
    volatile s32 count = 0x800000;

    while (dec->isdone == 0) {
        if (--count == 0) {
            dec->isdone = 1;
            dec->rectid = dec->rectid == 0;
            dec->slice.x = dec->rect[dec->rectid].x;
            dec->slice.y = dec->rect[dec->rectid].y;
        }
    }
    dec->isdone = 0;
}

/* The movie player's task: allocates its buffers and starts the movie, then shows a
   frame each update until the movie's last frame or Start; when killed, stops the
   CD, frees the buffers and clears VRAM */
void STDWTITL_tickMoviePlayer(MoviePlayerTask *task) {
    switch (task->state) {
    case TASK_INIT:
    default:
        STDWTITL_clearVram();
        STDWTITL_ringBuffer = HEAP.alloc(0x10000, 2);
        STDWTITL_vlcBuffer0 = HEAP.alloc(0x28000, 2);
        STDWTITL_vlcBuffer1 = HEAP.alloc(0x28000, 2);
        STDWTITL_imageBuffer0 = HEAP.alloc(0x4E00, 2);
        STDWTITL_imageBuffer1 = HEAP.alloc(0x4E00, 2);
        STDWTITL_vlcTable = HEAP.alloc(0x11000, 2);
        STDWTITL_initDecEnv(&STDWTITL_decEnv, 0, 0, 0, 416);
        FILE_TABLE.getPos(STDWTITL_movieFile, 0, &task->loc);
        STDWTITL_initStream(&task->loc, STDWTITL_onSliceDecoded);
        DecDCTvlcBuild(STDWTITL_vlcTable);
        STDWTITL_decodeNextFrame(&STDWTITL_decEnv);
        STDWTITL_movieEnded = 0;
        task->nextState(task);
        /* fallthrough */
    case TASK_RUN:
        DecDCTin(STDWTITL_decEnv.vlcbuf[STDWTITL_decEnv.vlcid], 3);
        DecDCTout(STDWTITL_decEnv.imgbuf[STDWTITL_decEnv.imgid], STDWTITL_decEnv.slice.w * STDWTITL_decEnv.slice.h / 2);
        STDWTITL_decodeNextFrame(&STDWTITL_decEnv);
        STDWTITL_waitFrameDecoded(&STDWTITL_decEnv, 0);
        if (STDWTITL_movieEnded == 1 || (PAD.getPressed(0) & (1 << PAD_START))) {
            task->setState(task, TASK_KILL);
        }
        break;
    case TASK_DONE:
        break;
    case TASK_KILL:
        CdControlB(CdlPause, 0, 0);
        DecDCToutCallback(NULL);
        StUnSetRing();
        HEAP.free(STDWTITL_ringBuffer);
        HEAP.free(STDWTITL_vlcBuffer0);
        HEAP.free(STDWTITL_vlcBuffer1);
        HEAP.free(STDWTITL_imageBuffer0);
        HEAP.free(STDWTITL_imageBuffer1);
        HEAP.free(STDWTITL_vlcTable);
        DrawSync(0);
        STDWTITL_clearVram();
        GFX.funcs.setDisplayArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        break;
    }
}

/* Creates the movie player (task) for the movie in file, which ends on frame endFrame */
MoviePlayerTask *STDWTITL_startMoviePlayerTask(s32 file, u32 endFrame) {
    MoviePlayerTask *task = createTask(STDWTITL_tickMoviePlayer, sizeof(MoviePlayerTask), 0);

    STDWTITL_movieFile = file;
    STDWTITL_movieEndFrame = endFrame;
    return task;
}

/* A movie's task: picks the movie's file and next mode (the European version's last
   movie by the language), plays it, then reloads the font and goes on to that mode */
void STDWTITL_tickMovie(MovieTask *task, MoviePlayerTask **player) {
    switch (task->state) {
    case TASK_INIT:
    default:
        GFX.funcs.reset();
        GFX.funcs.allocPrimBuffers(0x2800);
        GFX.funcs.setDisplayMode(320, 480, 1, 1);
#if VERSION_US
        if (task->movie != 10) {
            *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[task->movie].file, STDWTITL_movies[task->movie].endFrame);
            task->nextMode = STDWTITL_movies[task->movie].nextMode;
        } else {
            *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[11].file, STDWTITL_movies[11].endFrame);
            task->nextMode = STDWTITL_movies[11].nextMode;
        }
#elif VERSION_EU || VERSION_JP
        /* movie 11 is one of three, by the language */
        if (task->movie != 11) {
            *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[task->movie].file, STDWTITL_movies[task->movie].endFrame);
        } else {
            switch (LANGUAGE) {
            case 0:
                *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[11].file, STDWTITL_movies[11].endFrame);
                break;
            case 1:
                *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[12].file, STDWTITL_movies[12].endFrame);
                break;
            case 2 ... 6:
                *player = STDWTITL_startMoviePlayerTask(STDWTITL_movies[13].file, STDWTITL_movies[13].endFrame);
                break;
            }
        }
        if (task->movie != 11) {
            task->nextMode = STDWTITL_movies[task->movie].nextMode;
            if (task->movie == 2 && GAME.progress == 0x2D) {
                task->nextMode = 0x276;
            }
        } else {
            switch (LANGUAGE) {
            case 0:
                task->nextMode = STDWTITL_movies[11].nextMode;
                break;
            case 1:
                task->nextMode = STDWTITL_movies[12].nextMode;
                break;
            case 2 ... 6:
                task->nextMode = STDWTITL_movies[13].nextMode;
                break;
            }
        }
#endif
        SOUND.stopAll();
        task->nextState(task);
        break;
    case TASK_RUN:
        if (*player == NULL) {
            FONT.load();
            GAME.funcs.requestMode(task->nextMode, 0);
            task->setState(task, TASK_DONE);
        }
        break;
    case TASK_DONE:
    case TASK_KILL:
        break;
    }
}

/* Creates the task that plays STDWTITL_movies[movie] */
MovieTask *STDWTITL_startMovieTask(s32 movie) {
    MovieTask *task = createTask(STDWTITL_tickMovie, sizeof(MovieTask), sizeof(MoviePlayerTask *));

    task->movie = movie;
    return task;
}
