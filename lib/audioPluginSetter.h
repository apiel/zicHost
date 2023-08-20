#ifndef _AUDIO_PLUGIN_SETTER_H_
#define _AUDIO_PLUGIN_SETTER_H_

#include <stdint.h>
#include <string.h>

template <typename T>
class AudioPluginSetter {
public:
    void (T::*setPtr)(float value);
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

        typedef void (T::*setPtr)(float value);
        setPtr fnc_ptr = setters[param].setPtr;
        (instance->*fnc_ptr)(value);

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