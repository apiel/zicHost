#ifndef _AUDIO_PLUGIN_SETTER_H_
#define _AUDIO_PLUGIN_SETTER_H_

#include <stdint.h>
#include <string.h>

#define SETTER_METHODS                    \
    bool set(uint16_t param, float value) \
    {                                     \
        return setters.set(param, value); \
    }                                     \
    int16_t getParamKey(const char* name) \
    {                                     \
        return setters.getParamKey(name); \
    }

template <typename T>
class AudioPluginSetter {
public:
    T& (T::*setPtr)(float value);
    const char* key;
};

template <typename T>
class AudioPluginSetters {
public:
    int16_t setterCount;
    AudioPluginSetter<T>* setters;
    T* instance;

    AudioPluginSetters(T* instance, AudioPluginSetter<T>* setters, int16_t setterCount)
        : instance(instance)
        , setters(setters)
        , setterCount(setterCount)
    {
    }

    bool set(int16_t param, float value)
    {
        if (param >= setterCount) {
            return false;
        }

        (instance->*(setters[param].setPtr))(value);
        return true;
    }

    int16_t getParamKey(const char* name)
    {
        for (int16_t i = 0; i < setterCount; i++) {
            if (strcmp(name, setters[i].key) == 0) {
                return i;
            }
        }
        return -1;
    }
};

#endif