#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "../helpers/range.h"

#include "audioPlugin.h"

struct ValOptions {
    const char* label = NULL;
    int stepCount = 100;
    ValueType type = VALUE_BASIC;
};

template <typename T>
class Val {
protected:
    T* instance;

    int stepCount;
    ValueType type;
    const char* label;

    float value_f;

public:
    const char* key;
    T& (T::*callback)(float value);

    Val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value), ValOptions options = {})
        : instance(instance)
        , stepCount(options.stepCount)
        , type(options.type)
        , label(options.label)
        , value_f(initValue)
        , key(_key)
        , callback(_callback)
    {
    }

    const char* getLabel()
    {
        return label ? label : key;
    }

    ValueType getType()
    {
        return type;
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

    const char* getValueKey(int valueIndex)
    {
        return mapping[valueIndex]->key;
    }

    int getValueStepCount(int valueIndex)
    {
        return mapping[valueIndex]->getStepCount();
    }

    ValueType getValueType(int valueIndex)
    {
        return mapping[valueIndex]->getType();
    }

    const char* getValueLabel(int valueIndex)
    {
        return mapping[valueIndex]->getLabel();
    }
};

#endif