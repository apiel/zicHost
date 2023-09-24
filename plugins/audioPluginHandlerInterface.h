#ifndef _AUDIO_PLUGIN_HANDLER_INTERFACE_H_
#define _AUDIO_PLUGIN_HANDLER_INTERFACE_H_

#include <vector>
#include <stdexcept>

#include "audioPlugin.h"

struct Plugin {
    void* handle;
    AudioPlugin* instance;
};

class AudioPluginHandlerInterface {
public:
    std::vector<Plugin> plugins;

    AudioPlugin& getPlugin(const char* name)
    {
        for (Plugin& plugin : plugins) {
            if (strcmp(plugin.instance->name(), name) == 0) {
                return *plugin.instance;
            }
        }
        throw std::runtime_error("Could not find plugin");
    }
};

#endif