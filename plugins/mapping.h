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
    float value_f;
    char* value_s = NULL;

    void (*onUpdate)(float, void* data) = [](float, void* data) {};
    void* onUpdateData = NULL;

public:
    AudioPlugin::ValueProps props;

    const char* key;
    T& (T::*callback)(float value);

    Val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value), AudioPlugin::ValueProps props = {})
        : instance(instance)
        , props(props)
        , value_f(initValue)
        , key(_key)
        , callback(_callback)
    {
    }

    AudioPlugin::ValueProps* getProps()
    {
        return &props;
    }

    const char* getLabel()
    {
        return props.label ? props.label : key;
    }

    inline float get()
    {
        return value_f;
    }

    char* getString()
    {
        return value_s;
    }

    void set(char* value)
    {
        value_s = value;
    }

    void set(float value)
    {
        value_f = range(value, 0.0, 1.0);
    }

    void call(float value)
    {
        (instance->*(callback))(value);
        (*onUpdate)(value, onUpdateData);
    }

    void setOnUpdate(void (*callback)(float, void* data), void* data)
    {
        onUpdate = callback;
        onUpdateData = data;
    }
};

template <typename T>
class Mapping : public AudioPlugin {
public:
    std::vector<Val<T>*> mapping;

    Mapping(AudioPlugin::Props& props, std::vector<Val<T>*> mapping)
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

    char* getValueString(int valueIndex)
    {
        return mapping[valueIndex]->getString();
    }

    void setValue(int valueIndex, float value)
    {
        mapping[valueIndex]->call(value);
    }

    const char* getValueKey(int valueIndex)
    {
        return mapping[valueIndex]->key;
    }

    AudioPlugin::ValueProps* getValueProps(int valueIndex)
    {
        return mapping[valueIndex]->getProps();
    }

    const char* getValueLabel(int valueIndex)
    {
        return mapping[valueIndex]->getLabel();
    }

    void setValueWatcher(int valueIndex, void (*callback)(float, void* data), void* data)
    {
        mapping[valueIndex]->setOnUpdate(callback, data);
    }
};

#endif