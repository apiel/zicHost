#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "plugins/audioPlugin.h"

struct Plugin {
    void* handle;
    AudioPlugin* instance;
};

#endif