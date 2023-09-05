#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "../helpers/range.h"

#include "audioPlugin.h"

template <typename T>
class Val {
protected:
    T* instance;

    int stepCount;

    float value_f;

public:
    const char* key;
    T& (T::*callback)(float value);

    Val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value), int stepCount = 100)
        : instance(instance)
        , stepCount(stepCount)
        , value_f(initValue)
        , key(_key)
        , callback(_callback)
    {
    }

    float getStepCount()
    {
        return stepCount;
    }

    inline float get()
    {
        return value_f;
    }

    void set(float value)
    {
        value_f = range(value, 0.0, 1.0);
    }

    void call(float value)
    {
        (instance->*(callback))(value);
    }
};

template <typename T>
class Mapping : public AudioPlugin {
public:
    std::vector<Val<T>*> mapping;

    Mapping(AudioPluginProps& props, std::vector<Val<T>*> mapping)
        : AudioPlugin(props)
        , mapping(mapping)
    {
    }

    int getValueIndex(const char* key)
    {
        for (int i = 0; i < mapping.size(); i++) {
            if (strcmp(mapping[i]->key, key) == 0) {
                return i;
            }
        }
        return -1;
    }

    int getValueCount()
    {
        return mapping.size();
    }
    float getValue(int valueIndex)
    {
        return mapping[valueIndex]->get();
    }
    void setValue(int valueIndex, float value)
    {
        mapping[valueIndex]->call(value);
    }

    const char* getValueName(int valueIndex)
    {
        return mapping[valueIndex]->key;
    }

    int getStepCount(int valueIndex)
    {
        return mapping[valueIndex]->getStepCount();
    }
};

#endif