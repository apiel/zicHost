#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "../helpers/range.h"
#include "midiMapping.h"

#include "audioPlugin.h"

template <typename T>
class Val {
protected:
    T* instance;

    float getValue_f() { return value_f; }
    float getValue_i() { return value_i * 0.01; }
    float (Val::*getValuePtr)() = &Val::getValue_f;

public:
    const char* key;
    float value_f;
    uint8_t value_i;
    T& (T::*callback)(float value);
    MidiMappingItem<T> midi;

    Val(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value))
        : instance(instance)
        , value_f(initValue)
        , key(_key)
        , callback(_callback)
        , midi(instance, _key, _callback)
    {
    }

    // Val(T* instance, uint8_t initValue, const char* _key, T& (T::*_callback)(float value))
    //     : instance(instance)
    //     , value_i(initValue)
    //     , key(_key)
    //     , callback(_callback)
    //     , midi(instance, _key, _callback)
    // {
    //     getValuePtr = &Val::getValue_i;
    // }

    float get()
    {
        return (*this.*getValuePtr)();
    }

    void set(float value)
    {
        value_f = value;
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

    // TODO move midi logic out of here
    bool midi(std::vector<unsigned char>* message)
    {
        for (Val<T>* item : mapping) {
            if (item->midi.handle(message)) {
                return true;
            }
        }
        return false;
    }

    // TODO move midi logic out of here
    bool assignMidiMapping(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1)
    {
        for (Val<T>* item : mapping) {
            if (strcmp(item->key, key) == 0) {
                item->midi.set(size, valuePosition, msg0, msg1);
                return true;
            }
        }
        return false;
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
};

#endif