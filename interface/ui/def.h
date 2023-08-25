#ifndef _UI_DEF_H_
#define _UI_DEF_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef APP_FONT
#define APP_FONT "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
#endif

#ifndef APP_FONT_BOLD
#define APP_FONT_BOLD "/usr/share/fonts/truetype/liberation2/LiberationSans-Bold.ttf"
#endif

#ifndef APP_FONT_ITALIC
#define APP_FONT_ITALIC "/usr/share/fonts/truetype/liberation2/LiberationSans-Italic.ttf"
#endif

#ifndef APP_DEFAULT_FONT_SIZE
#define APP_DEFAULT_FONT_SIZE 16
#endif

#define SCREEN_W 800
#define SCREEN_H 480

struct Size {
    uint32_t w;
    uint32_t h;
};

struct Point {
    uint32_t x;
    uint32_t y;
};

#endif
