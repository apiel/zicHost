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

// TODO somehow would be great to give an ID or key to the plugin
// so we can set config by this value instead of
// VOLUME=1.0
// we would have
// MASTER:VOLUME=1.0
// or something like that
class AudioPlugin {
public:
    AudioPlugin(AudioPluginProps& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;
    virtual bool set(uint16_t param, float value) = 0;
    virtual int16_t getParamKey(const char* name) = 0;

    int (*debug)(const char* format, ...);
};

#endif