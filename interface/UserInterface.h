#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "interfacePlugin.h"
#include "ui/def.h"

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
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());
            return;
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL video driver: %s", SDL_GetCurrentVideoDriver());

        SDL_Window* window = SDL_CreateWindow(
            "Zic",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_W, SCREEN_H,
            SDL_WINDOW_SHOWN);

        if (window == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
            return;
        }
    }
};

#endif
