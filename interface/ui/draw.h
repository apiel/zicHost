#ifndef _UI_DRAW_H_
#define _UI_DRAW_H_

#include "def.h"
#include "state.h"
// #include "drawButton.h"

void draw()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, texture);
}

int drawText(Point position, const char* text, SDL_Color color = APP_DEFAULT_FONT_COLOR, uint32_t size = APP_DEFAULT_FONT_SIZE, const char* fontPath = APP_FONT)
{
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open font\n");
        return 0;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render text\n");
        return 0;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture\n");
        return 0;
    }

    SDL_Rect rect = { position.x, position.y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    int x = surface->w + position.x;

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    return x;
}

void drawClear(SDL_Color color = COLOR_BACKGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void drawFilledRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { position.x, position.y, size.w, size.h };
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(Point position, Size size, SDL_Color color = COLOR_FOREGROUND)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { position.x, position.y, size.w, size.h };
    SDL_RenderDrawRect(renderer, &rect);
}

#endif