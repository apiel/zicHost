#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "interfacePlugin.h"
#include "ui/def.h"
#include "ui/state.h"
#include "ui/event.h"

class UserInterface : public InterfacePlugin {
protected:
public:
    UserInterface(InterfacePluginProps& props)
        : InterfacePlugin(props)
    {
    }

    bool config(const char* key, const char* value)
    {
        return false;
    }

    const char* name()
    {
        return "User interface";
    }

    void start()
    {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
            return;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL video driver: %s", SDL_GetCurrentVideoDriver());

        window = SDL_CreateWindow(
            "Zic",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_W, SCREEN_H,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
            return;
        }

        TTF_Init();

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
        SDL_SetRenderTarget(renderer, texture);

        SDL_CreateThread(eventThread, "eventThread", this);
    }
};

#endif
