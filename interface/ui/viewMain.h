#ifndef _VIEW_MAIN_H
#define _VIEW_MAIN_H

#include "view.h"
#include "draw.h"

class ViewMain : public View {
protected:
    static ViewMain* instance;

    ViewMain() { }

public:
    static ViewMain& get()
    {
        if (!instance) {
            instance = new ViewMain();
        }
        return *instance;
    }

    void render()
    {
        drawClear();
        // drawText({10, 10}, "Hello World! 12345 # $ +");
        drawRect({10, 20}, {100, 100}, {255, 0, 0, 255});
        draw();
    }

    void handle()
    {
    }
};

ViewMain* ViewMain::instance = NULL;

#endif