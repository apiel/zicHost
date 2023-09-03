#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "lib/audioPlugin.h"

struct Plugin {
    void* handle;
    AudioPlugin* instance;
};

#endif