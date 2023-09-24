#ifndef _AUDIO_PLUGIN_HANDLER_INTERFACE_H_
#define _AUDIO_PLUGIN_HANDLER_INTERFACE_H_

#include <vector>

#include "audioPlugin.h"

class AudioPluginHandlerInterface {
public:
    struct Plugin {
        void* handle;
        AudioPlugin* instance;
    };

    std::vector<Plugin> plugins;

    virtual AudioPlugin& getPlugin(const char* name) = 0;
};

#endif