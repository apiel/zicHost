#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

enum ValueType {
    VALUE_BASIC,
    VALUE_CENTERED,
};

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

    virtual int getValueStepCount(int valueIndex) = 0;
    virtual const char* getValueLabel(int valueIndex) = 0;
    virtual ValueType getValueType(int valueIndex) = 0;
    virtual float getValue(int valueIndex) = 0;
    virtual void setValue(int valueIndex, float value) = 0;
    virtual int getValueCount() = 0;
    virtual const char* getValueKey(int valueIndex) = 0;
    virtual int getValueIndex(const char* key) = 0;
    virtual const char* getValueUnit(int valueIndex) = 0;
    virtual void setValueWatcher(int valueIndex, void (*callback)(float, void * data), void * data) = 0;

    int (*debug)(const char* format, ...);
};

#endif