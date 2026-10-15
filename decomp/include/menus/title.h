#ifndef STDWTITL_H
#define STDWTITL_H

/*
 * STDWTITL.PRO: the title screen, the opening movies and a notice screen.
 *
 * STDWTITL_start looks at the low byte of the game mode:
 * - 0 (MODE_TITLE): the title screen. A background with animated sprites,
 *   the two parts of the title sliding in, a glint, the logo and "PRESS
 *   START". Start shows the two options (new game, continue); Circle picks
 *   one, the screen fades out from its edges and the game goes on to
 *   MODE_NEW_GAME or MODE_CONTINUE. Ten seconds without Start go to
 *   MODE_OPENING instead.
 * - 1-11 (1-12 in the European version): plays movie n-1 of STDWTITL_movies
 *   (PsyQ's streaming sample player with libpress), then goes on to that
 *   movie's next mode. The European version picks its last movie by the
 *   language.
 * - 12 (13 in the European version): a still screen (SPLASH_FILE) that fades
 *   in, waits for Start or five minutes, fades out and goes on to MODE_OPENING.
 */

#include "engine/game.h"
#include <libpress.h>

/* The name of this overlay's copy of a function of src/shared/ */
#define OVL_NAME(name) STDWTITL_##name

/* Draw layers of the screens */
#define STDWTITL_SPLASH_LAYER 0x100
#define STDWTITL_TITLE_LAYER 0x1000

/* Entries of the files, read with FILE_CACHE.getEntry: the discs number
   their files differently */
#if VERSION_US || VERSION_JP
#define SPLASH_FILE 0x895
#define STDWTITL_FILE_BACKGROUND 0x876
#elif VERSION_EU
#define SPLASH_FILE 0x8A6
#define STDWTITL_FILE_BACKGROUND 0x887
#endif
#define SPLASH_SPRITES (SPLASH_FILE << 16)
#define SPLASH_IMAGES (SPLASH_FILE << 16 | 1)
#define STDWTITL_BACKGROUND(entry) (STDWTITL_FILE_BACKGROUND << 16 | (entry))

/* The VRAM row of the title's sprites (SpriteDrawer.setTexture) */
#if VERSION_US || VERSION_JP
#define STDWTITL_TEXTURE_Y 0
#elif VERSION_EU
#define STDWTITL_TEXTURE_Y 0x100
#endif

/* Sounds (SOUND.playSound) */
#define STDWTITL_TITLE_SOUND_BANK 0x47
#define STDWTITL_TITLE_MUSIC MUSIC(STDWTITL_TITLE_SOUND_BANK, 0)

/* The low byte of the game mode that shows the still screen, after the
   movies' */
#if VERSION_US
#define STDWTITL_SPLASH_MODE 12
#elif VERSION_EU || VERSION_JP
#define STDWTITL_SPLASH_MODE 13
#endif

/* STDWTITL_tickScreen's children */
typedef struct TitleModeChildren {
    /* 0x0 */ struct TitleLoaderTask *title;
    /* 0x4 */ struct MovieTask *movie;
    /* 0x8 */ Task *splash;
} TitleModeChildren;

/* Mode 12 */
typedef struct SplashTask {
    TASK_HEADER(SplashTask);
    /* 0x50 */ s16 fade; /* sprite frame: 15 is black, 0 fully visible */
    /* 0x52 */ s16 timer;
} SplashTask;

/*
 * Movies: PsyQ's STR player. DecEnv is the sample's DECENV: two VLC buffers,
 * two buffers for the decoded slices, and where the frames go in VRAM.
 */
typedef struct DecEnv {
    /* 0x00 */ u_long *vlcbuf[2];
    /* 0x08 */ s32 vlcid;
    /* 0x0C */ u_long *imgbuf[2]; /* 16-bit pixels, as words for DecDCTout and LoadImage */
    /* 0x14 */ s32 imgid;
    /* 0x18 */ RECT rect[2];
    /* 0x28 */ s32 rectid;
    /* 0x2C */ RECT slice;
    /* 0x34 */ INTERRUPT_SHARED s32 isdone; /* set by STDWTITL_onSliceDecoded, the MDEC's callback */
} DecEnv;

typedef struct MovieInfo {
    /* 0x0 */ s32 file;
    /* 0x4 */ u32 endFrame;
    /* 0x8 */ s32 nextMode;
} MovieInfo;

typedef struct MovieTask {
    TASK_HEADER(MovieTask);
    /* 0x50 */ s32 movie;
    /* 0x54 */ s32 nextMode;
} MovieTask;

typedef struct MoviePlayerTask {
    TASK_HEADER(MoviePlayerTask);
    /* 0x50 */ CdlLOC loc;
} MoviePlayerTask;

/* The title screen */
typedef struct TitleLoaderTask {
    TASK_HEADER(TitleLoaderTask);
    /* 0x50 */ s32 layerId;
    /* 0x54 */ s32 depth;
} TitleLoaderTask;

/* The logo: sprite 6, and sprites 3 and 5 animated once */
typedef struct LogoAnim {
    /* 0x0 */ s16 index;
    /* 0x2 */ s16 frame;
    /* 0x4 */ s32 done;
} LogoAnim;

typedef struct LogoTask {
    TASK_HEADER(LogoTask);
    /* 0x50 */ s32 skip; /* start fully shown */
    /* 0x54 */ s32 visible;
    /* 0x58 */ LogoAnim anims[2];
    /* 0x68 */ s32 layerId;
    /* 0x6C */ s32 depth;
    /* 0x70 */ u8 pad[8]; /* never read or written */
    /* 0x78 */ void (*show)(struct LogoTask *task);
} LogoTask;

/* A glint (sprite 4) played once, then sprite 2 lit */
typedef struct GlintTask {
    TASK_HEADER(GlintTask);
    /* 0x50 */ s32 skip;
    /* 0x54 */ s32 index;
    /* 0x58 */ s32 frame;
    /* 0x5C */ s32 lit;
    /* 0x60 */ s32 layerId;
    /* 0x64 */ s32 depth;
    /* 0x68 */ u8 pad[8]; /* never read or written */
    /* 0x70 */ void (*show)(struct GlintTask *task);
} GlintTask;

/* A part of the title that slides in from a side of the screen */
typedef struct SlideTask {
    TASK_HEADER(SlideTask);
    /* 0x50 */ s32 skip;
    /* 0x54 */ s32 steps;
    /* 0x58 */ s32 x;
    /* 0x5C */ s32 y;
    /* 0x60 */ s32 layerId;
    /* 0x64 */ s32 depth;
    /* 0x68 */ u8 pad[8]; /* never read or written */
    /* 0x70 */ void (*show)(struct SlideTask *task);
} SlideTask;

/* "PRESS START", then the two options */
typedef struct MenuOption {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ u8 pad[8]; /* never read or written */
} MenuOption;

typedef struct TitleMenuTask {
    TASK_HEADER(TitleMenuTask);
    /* 0x50 */ s16 skip;
    /* 0x52 */ s16 choice; /* 1: new game, 2: continue, 3: timed out */
    /* 0x54 */ s16 timer;
    /* 0x56 */ s16 showOptions;
    /* 0x58 */ u8 showCursor;
    /* 0x59 */ u8 blink;
    /* 0x5A */ s16 selection; /* 0, 1: the options, 2: "PRESS START" */
    /* 0x5C */ MenuOption options[2];
    /* 0x7C */ u8 pad7C[0x10]; /* never read or written */
    /* 0x8C */ s32 layerId;
    /* 0x90 */ s32 depth;
    /* 0x94 */ u8 pad94[8]; /* never read or written */
    /* 0x9C */ void (*show)(struct TitleMenuTask *task);
    /* 0xA0 */ void (*reset)(struct TitleMenuTask *task);
    /* 0xA4 */ s32 (*getChoice)(struct TitleMenuTask *task);
} TitleMenuTask;

typedef struct Point {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} Point;

/* The fade out from the screen's edges once an option is chosen */
typedef struct EdgeFadeTask {
    TASK_HEADER(EdgeFadeTask);
    /* 0x50 */ s32 done;
    /* 0x54 */ s16 level;
    /* 0x56 */ s16 time;
    /* 0x58 */ void (*start)(struct EdgeFadeTask *task);
    /* 0x5C */ s32 (*isDone)(struct EdgeFadeTask *task);
} EdgeFadeTask;

typedef struct EdgeLine {
    /* 0x0 */ s16 x1;
    /* 0x2 */ s16 y1;
    /* 0x4 */ s16 x2;
    /* 0x6 */ s16 y2;
} EdgeLine;

/* The background, and its eight looping sprite animations */
typedef struct Point16 {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} Point16;

typedef struct TitleBackgroundTask {
    TASK_HEADER(TitleBackgroundTask);
    /* 0x50 */ s32 skip;
    /* 0x54 */ AnimState anims[8];
    /* 0x74 */ Point16 pos6;
    /* 0x78 */ Point16 pos7;
    /* 0x7C */ s32 layerId;
    /* 0x80 */ s32 depth;
    /* 0x84 */ u8 pad[0x10]; /* never read or written */
    /* 0x94 */ void (*animate)(struct TitleBackgroundTask *task);
} TitleBackgroundTask;

typedef struct TitleChildren {
    /* 0x00 */ EdgeFadeTask *fade;
#if VERSION_US || VERSION_JP
    /* 0x04 */ SlideTask *title0;
    /* 0x08 */ SlideTask *title1;
    /* 0x0C */ GlintTask *glint;
#elif VERSION_EU
    /* 0x04 */ GlintTask *glint;
    /* 0x08 */ SlideTask *title1;
    /* 0x0C */ SlideTask *title0;
#endif
    /* 0x10 */ LogoTask *logo;
    /* 0x14 */ TitleMenuTask *menu;
    /* 0x18 */ TitleBackgroundTask *background;
} TitleChildren;

typedef struct TitleTask {
    TASK_HEADER(TitleTask);
    /* 0x50 */ TitleLoaderTask *parent;
    /* 0x54 */ s32 timer;
    /* 0x58 */ s32 choice;
    /* 0x5C */ s32 layerId;
    /* 0x60 */ s32 depth;
    /* 0x64 */ u8 pad[0xD8]; /* never read or written */
} TitleTask;

typedef struct TitleImages {
    /* 0x0 */ s32 images;  /* TIM archive */
    /* 0x4 */ s32 sprites; /* sprite bank */
} TitleImages;

typedef struct TitleFuncs {
    /* 0x00 */ void (*loadImages)(void);
    /* 0x04 */ void (*startFade)(PanelAnim *fade, s32 fadeIn);
    /* 0x08 */ s32 (*updateFade)(PanelAnim *fade);
    /* 0x0C */ void (*startLerp)(MenuLerp *lerp, s32 from, s32 to, s32 duration);
    /* 0x10 */ s32 (*updateLerp)(MenuLerp *lerp);
} TitleFuncs;

/* Executable symbols */
/* libcd's flag that StCdInterrupt is pending: libcd sets it as a word, the
   game reads and clears its low byte */
extern u_char StCdIntrFlag;

/* STDWTITL's data, in its order: data/stdwtitl.c's, data/title.c's and
   data/movie.c's */
extern RECT STDWTITL_screenRect;
extern s16 STDWTITL_logoFrames[2][13];
extern s32 STDWTITL_movieWidth;
extern s32 STDWTITL_movieHeight;
extern RECT STDWTITL_vramRect;
extern MovieInfo STDWTITL_movies[];
extern s16 STDWTITL_glintAltFrames[];
extern s16 STDWTITL_glintFrames[];
extern Point STDWTITL_menuCursorPositions[];
#if VERSION_EU
/* The menu's sprites (press start, new game, continue) for each language */
extern u8 STDWTITL_menuSprites[][4];
#endif

extern EdgeLine STDWTITL_edgeFadeLines[];
extern AnimFrame STDWTITL_backgroundAnim0[];
extern AnimFrame STDWTITL_backgroundAnim1[];
extern AnimFrame STDWTITL_backgroundAnim2[];
extern AnimFrame STDWTITL_backgroundAnim3[];
extern AnimFrame STDWTITL_backgroundAnim4[];
extern AnimFrame STDWTITL_backgroundAnim5[];
extern AnimFrame STDWTITL_backgroundAnim6[];
extern AnimFrame STDWTITL_backgroundAnim7[];
extern Point16 STDWTITL_background6Positions[];
extern Point16 STDWTITL_background7Positions[];
extern s32 STDWTITL_spriteBank;
extern TitleFuncs STDWTITL_titleFuncs;
extern TitleImages STDWTITL_titleImages[];

extern DecEnv STDWTITL_decEnv;
extern u_long *STDWTITL_ringBuffer;
extern u_short *STDWTITL_vlcTable;
extern u_long *STDWTITL_vlcBuffer0;
extern u_long *STDWTITL_vlcBuffer1;
extern u_long *STDWTITL_imageBuffer0;
extern u_long *STDWTITL_imageBuffer1;
extern s32 STDWTITL_movieEnded;
extern s32 STDWTITL_movieFile;
extern u32 STDWTITL_movieEndFrame;

/* scene.c */
void STDWTITL_tickSplashLoader(Task *task, SplashTask **splash);
Task *STDWTITL_startSplashLoaderTask(void);
void STDWTITL_tickScreen(Task *task, TitleModeChildren *children);

/* logo.c */
void STDWTITL_drawLogo(LogoTask *task);
void STDWTITL_tickLogo(LogoTask *task);
void STDWTITL_showLogo(LogoTask *task);
LogoTask *STDWTITL_startLogoTask(s32 skip);

/* movie.c */
void STDWTITL_clearVram(void);
void STDWTITL_initDecEnv(DecEnv *dec, s16 x0, s16 y0, s16 x1, s16 y1);
void STDWTITL_readStream(CdlLOC *loc);
void STDWTITL_initStream(CdlLOC *loc, void (*callback)());
u_long *STDWTITL_getNextFrame(DecEnv *dec);
s32 STDWTITL_decodeNextFrame(DecEnv *dec);
void STDWTITL_onSliceDecoded(void);
void STDWTITL_waitFrameDecoded(DecEnv *dec, s32 mode);
void STDWTITL_tickMoviePlayer(MoviePlayerTask *task);
MoviePlayerTask *STDWTITL_startMoviePlayerTask(s32 file, u32 endFrame);
void STDWTITL_tickMovie(MovieTask *task, MoviePlayerTask **player);
MovieTask *STDWTITL_startMovieTask(s32 movie);

/* glint.c */
void STDWTITL_drawGlintAlt(GlintTask *task);
void STDWTITL_tickGlintAlt(GlintTask *task);
void STDWTITL_showGlint(GlintTask *task);
GlintTask *STDWTITL_startGlintAltTask(s32 skip);
void STDWTITL_drawGlint(GlintTask *task);
void STDWTITL_tickGlint(GlintTask *task);
GlintTask *STDWTITL_startGlintTask(s32 skip);

/* splash.c */
void STDWTITL_drawSplash(SplashTask *task);
void STDWTITL_tickSplash(SplashTask *task);
SplashTask *STDWTITL_startSplashTask(void);

/* title_loader.c */
void STDWTITL_runTitleLoader(TitleLoaderTask *task, TitleTask **title);
void STDWTITL_tickTitleLoader(TitleLoaderTask *task, TitleTask **title);
TitleLoaderTask *STDWTITL_startTitleLoaderTask(void);

/* slides.c */
void STDWTITL_drawTitle1Alt(SlideTask *task);
void STDWTITL_tickTitle1Alt(SlideTask *task);
void STDWTITL_showTitle1(SlideTask *task);
SlideTask *STDWTITL_startTitle1AltTask(s32 skip);
void STDWTITL_drawTitle1(SlideTask *task);
void STDWTITL_tickTitle1(SlideTask *task);
SlideTask *STDWTITL_startTitle1Task(s32 skip);
void STDWTITL_drawTitle0Alt(SlideTask *task);
void STDWTITL_tickTitle0Alt(SlideTask *task);
void STDWTITL_showTitle0(SlideTask *task);
SlideTask *STDWTITL_startTitle0AltTask(s32 skip);
void STDWTITL_drawTitle0(SlideTask *task);
void STDWTITL_tickTitle0(SlideTask *task);
SlideTask *STDWTITL_startTitle0Task(s32 skip);

/* menu.c */
void STDWTITL_drawMenu(TitleMenuTask *task);
void STDWTITL_tickMenu(TitleMenuTask *task);
void STDWTITL_showMenu(TitleMenuTask *task);
void STDWTITL_resetMenu(TitleMenuTask *task);
s32 STDWTITL_getMenuChoice(TitleMenuTask *task);
TitleMenuTask *STDWTITL_startMenuTask(s16 skip);

/* edge_fade.c */
s32 STDWTITL_getEdgeFadeLevel(s32 time);
void STDWTITL_drawEdgeFade(s32 level);
void STDWTITL_startEdgeFade(EdgeFadeTask *task);
s32 STDWTITL_isEdgeFadeDone(EdgeFadeTask *task);
void STDWTITL_tickEdgeFade(EdgeFadeTask *task);
EdgeFadeTask *STDWTITL_startEdgeFadeTask(void);

/* background.c */
s32 STDWTITL_stepLoopingAnimation(AnimState *anim, AnimFrame *frames, s32 depth);
void STDWTITL_drawBackground(TitleBackgroundTask *task);
void STDWTITL_drawBackgroundSprites(TitleBackgroundTask *task);
void STDWTITL_tickBackground(TitleBackgroundTask *task);
void STDWTITL_animateBackground(TitleBackgroundTask *task);
TitleBackgroundTask *STDWTITL_startBackgroundTask(s32 skip);

/* title.c */
s32 STDWTITL_leaveTitle(TitleTask *task, TitleChildren *children);
s32 STDWTITL_stepTitle(TitleTask *task, TitleChildren *children);
void STDWTITL_tickTitle(TitleTask *task, TitleChildren *children);
TitleTask *STDWTITL_startTitleTask(TitleLoaderTask *parent);
void STDWTITL_loadTitleImages(void);
void STDWTITL_startFade(PanelAnim *fade, s32 fadeIn);
s32 STDWTITL_updateFade(PanelAnim *fade);
void STDWTITL_startLerp(MenuLerp *lerp, s32 from, s32 to, s32 frames);
s32 STDWTITL_updateLerp(MenuLerp *lerp);

#endif /* STDWTITL_H */
