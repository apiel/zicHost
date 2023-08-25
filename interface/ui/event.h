#ifndef _UI_EVENT_H_
#define _UI_EVENT_H_

#include "def.h"
#include "state.h"
#include "../interfacePlugin.h"
#include "viewMain.h"

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
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Event thread started");

    InterfacePlugin* userInterface = (InterfacePlugin*)data;

    ViewMain& viewMain = ViewMain::get();
    viewMain.render();
    draw();

    unsigned long lastUpdate = SDL_GetTicks();
    while (handleEvent()) {
        unsigned long now = SDL_GetTicks();

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    userInterface->quit();
    return 0;
}

#endif