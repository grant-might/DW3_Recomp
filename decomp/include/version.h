#ifndef VERSION_H
#define VERSION_H

/*
 * The version of the game being built. The Makefile passes exactly one
 * -DVERSION_<VERSION> (make VERSION=eu gives -DVERSION_EU); here every
 * version's macro becomes 0 or 1, so that code tests them with #if:
 *
 *     #if VERSION_EU
 *     #if VERSION_US || VERSION_EU
 *
 * never with #ifdef or defined(), which can't tell a version that is off
 * from a misspelt one (-Wundef warns about the misspelling in #if). A
 * condition names the versions it is for: the versions have no order.
 * A new version gets its line here and in the Makefile's VERSIONS.
 */
#if defined(VERSION_US) + defined(VERSION_EU) + defined(VERSION_JP) != 1
#error "build with exactly one of -DVERSION_US, -DVERSION_EU and -DVERSION_JP, as make VERSION=<version> does"
#endif

#ifndef VERSION_US
#define VERSION_US 0
#endif
#ifndef VERSION_EU
#define VERSION_EU 0
#endif
#ifndef VERSION_JP
#define VERSION_JP 0
#endif

/*
 * What the versions differ in, where the same difference recurs: each 0 or
 * 1, tested with #if as the versions are, so that the code says what the
 * difference is.
 *
 * PAL_VIDEO: the European release runs at 50 Hz (PAL), its clocks a step
 * longer, unless NTSC_MODE is set, and moves its display down
 * (SHIFT_PAL_SCREEN); the USA and Japanese ones are NTSC only.
 */
#define PAL_VIDEO VERSION_EU

#endif /* VERSION_H */
