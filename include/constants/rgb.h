#ifndef GUARD_RGB_H
#define GUARD_RGB_H

#define GET_R(color) ((color) & 0x1F)
#define GET_G(color) (((color) >> 5) & 0x1F)
#define GET_B(color) (((color) >> 10) & 0x1F)

#define RGB(r, g, b)  ((r) | ((g) << 5) | ((b) << 10))
#define RGB2(r, g, b) (((b) << 10) | ((g) << 5) | (r))
#define _RGB(r, g, b) ((((b) & 0x1F) << 10) + (((g) & 0x1F) << 5) + ((r) & 0x1F))
#define RGB2GBA(r, g, b) (((r >> 3) & 31) | (((g >> 3) & 31) << 5) | (((b >> 3) & 31) << 10))

#define RGB_ALPHA       (1 << 15)
#define IS_ALPHA(color) ((color) & RGB_ALPHA)

#define RGB_EMERALD    RGB(24, 31, 12)
#define RGB_BLACK      RGB(0, 0, 0)
#define RGB_WHITE      RGB(31, 31, 31)
#define RGB_GRAY       RGB(15, 15, 15)
#define RGB_RED        RGB(31, 0, 0)
#define RGB_GREEN      RGB(0, 31, 0)
#define RGB_BLUE       RGB(0, 0, 31)
#define RGB_PURPLE     RGB(24, 0, 24)
#define RGB_YELLOW     RGB(31, 31, 0)
#define RGB_MAGENTA    RGB(31, 0, 31)
#define RGB_CYAN       RGB(0, 31, 31)
#define RGB_WHITEALPHA (RGB_WHITE | RGB_ALPHA)

#define RGB_LIME_GREEN  RGB2GBA(222, 230, 49)

#define RGB_IKIGAI_BLUE         RGB2GBA(204, 229, 236)
#define RGB_IKIGAI_PINK         RGB2GBA(249, 219, 253)
#define RGB_IKIGAI_DARKEST_GREY RGB2GBA(24, 24, 24)
#define RGB_IKIGAI_DARK_GREY    RGB2GBA(48, 48, 48)
#define RGB_IKIGAI_GREY         RGB2GBA(72, 72, 72)
#define RGB_IKIGAI_BLUE_ALPHA  (RGB_IKIGAI_BLUE | RGB_ALPHA)
#define RGB_IKIGAI_PINK_ALPHA  (RGB_IKIGAI_PINK | RGB_ALPHA)

#endif // GUARD_RGB_H
