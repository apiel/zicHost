#ifndef _AUDIO_PLUGIN_SETTER_H_
#define _AUDIO_PLUGIN_SETTER_H_

#include <stdint.h>
#include <string.h>

#include <cstdio> // printf

template <typename T>
class AudioPluginSetter {
public:
    void (T::*setPtr)(float value);
    const char* key;
};

template <typename T>
class AudioPluginSetters {
public:
    uint16_t setterCount;
    AudioPluginSetter<T>* setters;

    AudioPluginSetters(AudioPluginSetter<T> * setters, uint16_t setterCount)
        : setters(setters)
        , setterCount(setterCount)
    {
    }

    bool set(uint16_t param, float value)
    {
        if (param >= setterCount) {
            return false;
        }
        // (this->*setters[param].setPtr)(value);
        // (*setters[param].setPtr)(value);
        printf(">>>>>>>>>>>>>>> set %d %f for %s\n", param, value, setters[param].key);
        return true;
    }

    uint16_t getParamKey(const char* name)
    {
        for (uint16_t i = 0; i < setterCount; i++) {
            if (strcmp(name, setters[i].key) == 0) {
                return i;
            }
        }
        return -1;
    }
};

#endif