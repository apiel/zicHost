#ifndef _UI_EVENT_H_
#define _UI_EVENT_H_

#include "def.h"
#include "state.h"

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Quit");
            return false;
        }
    }
    return true;
}

int eventThread(void* data)
{
    // UserInterface* userInterface = (UserInterface*)data;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Event thread started");
    unsigned long lastUpdate = SDL_GetTicks();
    while (handleEvent()) {
        unsigned long now = SDL_GetTicks();

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

#endif