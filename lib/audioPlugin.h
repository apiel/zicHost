#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>

class AudioPluginProps {
public:
    int (*debug)(const char* format, ...);

    AudioPluginProps(int (*debug)(const char* format, ...))
        : debug(debug)
    {
    }
};

class AudioPlugin {
public:
    AudioPlugin(AudioPluginProps& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;
    virtual bool set(uint16_t param, float value) = 0;
    virtual uint16_t getParamKey(const char* name) = 0;

    int (*debug)(const char* format, ...);
};

#endif