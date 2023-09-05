#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

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

    virtual int getStepCount(int valueIndex) = 0;
    virtual float getValue(int valueIndex) = 0;
    virtual void setValue(int valueIndex, float value) = 0;
    virtual int getValueCount() = 0;
    virtual const char* getValueName(int valueIndex) = 0;
    virtual int getValueIndex(const char* key) = 0;

    int (*debug)(const char* format, ...);
};

#endif