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

    AudioPluginSetters(AudioPluginSetter<T>* setters, uint16_t setterCount)
        : setters(setters)
        , setterCount(setterCount)
    {
    }

    bool set(T * obj, uint16_t param, float value)
    {
        if (param >= setterCount) {
            return false;
        }
        printf(">>>>>>>>>>>>>>> set %d %f for %s\n", param, value, setters[param].key);
        // setters[param].setPtr(value);
        // (this->*setters[param].setPtr)(value);
        // (this->*setters[param].setPtr)(value);

        // typedef void (Some_class::*Some_fnc_ptr)();
        // Some_fnc_ptr fnc_ptr = &Some_class::some_function;
        // Some_class sc;
        // (sc.*fnc_ptr)();

        typedef void (T::*setPtr)(float value);
        setPtr fnc_ptr = setters[param].setPtr;
        // (this->*fnc_ptr)(value);
        // (obj.*fnc_ptr)(value);
        (obj->*fnc_ptr)(value);

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