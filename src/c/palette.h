#ifndef PALETTE_H
#define PALETTE_H

#include <pebble.h>

#if defined(PBL_COLOR)
#define PAL_BG         GColorBlack
#define PAL_GOLD       GColorBrass
#define PAL_ACCENT     GColorRajah
#define PAL_CREAM      GColorPastelYellow
#define PAL_HOUR_RED   GColorSunsetOrange
#define PAL_RED        GColorRed
#define PAL_HEAT1      GColorOrange
#define PAL_HEAT2      GColorSunsetOrange
#define PAL_HEAT3      GColorChromeYellow
#define PAL_HEAT4      GColorRajah
#define PAL_BASELINE   GColorBulgarianRose
#define PAL_DISABLED   GColorDarkGray
#define PAL_QUIET      GColorWindsorTan
#else
#define PAL_BG         GColorBlack
#define PAL_GOLD       GColorWhite
#define PAL_ACCENT     GColorWhite
#define PAL_CREAM      GColorWhite
#define PAL_HOUR_RED   GColorWhite
#define PAL_RED        GColorWhite
#define PAL_HEAT1      GColorWhite
#define PAL_HEAT2      GColorWhite
#define PAL_HEAT3      GColorWhite
#define PAL_HEAT4      GColorWhite
#define PAL_BASELINE   GColorWhite
#define PAL_DISABLED   GColorWhite
#define PAL_QUIET      GColorWhite
#endif

#endif
