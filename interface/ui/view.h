#ifndef _UI_VIEW_H_
#define _UI_VIEW_H_

#include "def.h"

class View {
public:
    virtual void render() = 0;
    virtual void handle() = 0;
};

#endif
