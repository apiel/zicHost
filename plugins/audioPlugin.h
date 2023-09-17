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
        uint8_t channels;
    };

    AudioPlugin(Props& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;

    virtual ValueProps* getValueProps(int valueIndex)
    {
        return NULL;
    }

    virtual const char* getValueLabel(int valueIndex)
    {
        return NULL;
    }

    virtual float getValue(int valueIndex)
    {
        return 0.0f;
    }

    virtual void setValue(int valueIndex, float value) { }

    virtual int getValueCount()
    {
        return 0;
    }

    virtual const char* getValueKey(int valueIndex)
    {
        return NULL;
    }

    virtual int getValueIndex(const char* key)
    {
        return -1;
    }

    virtual void setValueWatcher(int valueIndex, void (*callback)(float, void* data), void* data) { }
    
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