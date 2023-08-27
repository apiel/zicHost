#ifndef _UI_EVENT_H_
#define _UI_EVENT_H_

#include "../interfacePlugin.h"
#include "def.h"
#include "state.h"
#include "viewMain.h"

int lastLog = 0;
void handleMotion(int x, int y, int id)
{
    if (SDL_GetTicks() - lastLog > 100) {
        lastLog = SDL_GetTicks();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Motion %d, %d (id: %d)", x, y, id);
    }
}

void handleMotionUp(int x, int y, int id)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MotionUp %d, %d (id: %d)", x, y, id);
}

void handleMotionDown(int x, int y, int id)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "MotionDown %d, %d (id: %d)", x, y, id);
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Quit");
            return false;

        case SDL_MOUSEMOTION:
            handleMotion(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_MOUSEBUTTONDOWN:
            handleMotionDown(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_MOUSEBUTTONUP:
            handleMotionUp(event.motion.x, event.motion.y, event.motion.which);
            return true;

        case SDL_FINGERMOTION:
            handleMotion(event.tfinger.x, event.tfinger.y, event.tfinger.fingerId);
            return true;

        case SDL_FINGERDOWN:
            handleMotionDown(event.tfinger.x, event.tfinger.y, event.tfinger.fingerId);
            return true;

        case SDL_FINGERUP:
            handleMotionUp(event.tfinger.x, event.tfinger.y, event.tfinger.fingerId);
            return true;
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