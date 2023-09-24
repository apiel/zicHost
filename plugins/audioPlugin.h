#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

#include "valueInterface.h"

class AudioPlugin;

class AudioPluginHandlerInterface {
public:
    struct Plugin {
        void* handle;
        AudioPlugin* instance;
    };

    std::vector<Plugin> plugins;

    virtual AudioPlugin& getPlugin(const char* name) = 0;
};

class AudioPlugin {
public:
    AudioPluginHandlerInterface* audioPluginHandler;
    int (*debug)(const char* format, ...);

    struct Props {
        int (*debug)(const char* format, ...);
        uint64_t sampleRate;
        uint8_t channels;
        AudioPluginHandlerInterface* audioPluginHandler;
    };

    AudioPlugin(Props& props)
        : audioPluginHandler(props.audioPluginHandler)
        , debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;

    virtual ValueInterface* getValue(int valueIndex)
    {
        return NULL;
    }

    virtual ValueInterface* getValue(const char* key)
    {
        return NULL;
    }

    virtual int getValueCount()
    {
        return 0;
    }

    virtual int getValueIndex(const char* key)
    {
        return -1;
    }

    virtual void noteOn(uint8_t note, uint8_t velocity)
    {
    }

    virtual void noteOff(uint8_t note, uint8_t velocity)
    {
    }

    virtual bool config(char* key, char* value)
    {
        return false;
    }

    virtual void* data(int id)
    {
        return NULL;
    }

    virtual void onClockTick()
    {
    }

    virtual void onStart()
    {
    }

    virtual void onStop()
    {
    }
};

#endif