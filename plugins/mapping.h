#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "../helpers/range.h"

#include "audioPlugin.h"

template <typename T>
class Val : public ValueInterface {
protected:
    T* instance;
    float value_f;
    char* value_s = NULL;

    void (*onUpdatePtr)(float, void* data) = [](float, void* data) {};
    void* onUpdateData = NULL;

    ValueInterface::Props _props;

public:
    const char* _key;
    T& (T::*callback)(float value);

    Val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value), ValueInterface::Props props = {})
        : instance(instance)
        , _props(props)
        , value_f(initValue)
        , _key(_key)
        , callback(_callback)
    {
    }

    ValueInterface::Props& props()
    {
        return _props;
    }

    const char* key()
    {
        return _key;
    }

    const char* label()
    {
        return _props.label ? _props.label : _key;
    }

    inline float get()
    {
        return value_f;
    }

    inline int getAsInt()
    {
        return value_f * _props.stepMultiplier * (_props.stepCount - 1) + _props.stepStart;
    }

    char* string()
    {
        return value_s;
    }

    void setString(char* value)
    {
        value_s = value;
    }

    void setFloat(float value)
    {
        value_f = range(value, 0.0, 1.0);
    }

    void set(float value)
    {
        (instance->*(callback))(value);
        (*onUpdatePtr)(value, onUpdateData);
    }

    void onUpdate(void (*callback)(float, void* data), void* data)
    {
        onUpdatePtr = callback;
        onUpdateData = data;
    }
};

template <typename T>
class Mapping : public AudioPlugin {
protected:
    std::vector<Val<T>*> mapping;

    Val<T>& val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value), ValueInterface::Props props = {})
    {
        Val<T>* v = new Val<T>(instance, initValue, _key, _callback, props);
        mapping.push_back(v);
        // debug("-------- Mapping: %s\n", v->key());
        return *v;
    }

public:
    Mapping(AudioPlugin::Props& props, char * _name, std::vector<Val<T>*> mapping = {})
        : AudioPlugin(props, _name)
        , mapping(mapping)
    {
    }

    int getValueIndex(const char* key)
    {
        for (int i = 0; i < mapping.size(); i++) {
            if (strcmp(mapping[i]->key(), key) == 0) {
                return i;
            }
        }
        return -1;
    }

    int getValueCount()
    {
        return mapping.size();
    }
    ValueInterface* getValue(int valueIndex)
    {
        if (valueIndex < 0 || valueIndex >= mapping.size()) {
            return NULL;
        }
        return mapping[valueIndex];
    }

    ValueInterface* getValue(const char* key)
    {
        for (int i = 0; i < mapping.size(); i++) {
            if (strcmp(mapping[i]->key(), key) == 0) {
                return mapping[i];
            }
        }
        return NULL;
    }
};

#endif