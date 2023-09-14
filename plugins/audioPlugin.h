#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

enum ValueType {
    VALUE_BASIC,
    VALUE_CENTERED,
    VALUE_CENTERED_ONE_SIDED,
    VALUE_STRING,
};

class AudioPlugin {
public:
    struct ValueProps {
        const char* label = NULL;
        int stepCount = 100;
        ValueType type = VALUE_BASIC;
        const char* unit = NULL;
        int stepStart = 0;
    };
    struct Props {
        int (*debug)(const char* format, ...);
        uint64_t sampleRate;
        uint8_t channel;
    };

    AudioPlugin(Props& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;

    virtual ValueProps* getValueProps(int valueIndex) = 0;
    virtual const char* getValueLabel(int valueIndex) = 0;
    virtual float getValue(int valueIndex) = 0;
    virtual void setValue(int valueIndex, float value) = 0;
    virtual int getValueCount() = 0;
    virtual const char* getValueKey(int valueIndex) = 0;
    virtual int getValueIndex(const char* key) = 0;
    virtual void setValueWatcher(int valueIndex, void (*callback)(float, void* data), void* data) = 0;
    virtual char* getValueString(int valueIndex)
    {
        return NULL;
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

    int (*debug)(const char* format, ...);
};

#endif