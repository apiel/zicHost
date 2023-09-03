#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "../helpers/range.h"
#include "midiMapping.h"

#define MAPPING_HANDLER                                                                                      \
    bool midi(std::vector<unsigned char>* message)                                                           \
    {                                                                                                        \
        return mapping.midi(message);                                                                        \
    }                                                                                                        \
    bool assignMidiMapping(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1) \
    {                                                                                                        \
        return mapping.assignMidi(key, size, valuePosition, msg0, msg1);                                     \
    }                                                                                                        \
    float getValue(int valueIndex)                                                                           \
    {                                                                                                        \
        return mapping.getValue(valueIndex);                                                                 \
    }                                                                                                        \
    void setValue(int valueIndex, float value)                                                               \
    {                                                                                                        \
        mapping.setValue(valueIndex, value);                                                                 \
    }                                                                                                        \
    int getValueCount()                                                                                      \
    {                                                                                                        \
        return mapping.getValueCount();                                                                      \
    }                                                                                                        \
    const char* getValueName(int valueIndex)                                                                 \
    {                                                                                                        \
        return mapping.getValueName(valueIndex);                                                             \
    }

template <typename T>
class MappingItem {
protected:
    T* instance;

public:
    const char* key;
    float value_f;
    uint8_t value_i;
    T& (T::*callback)(float value);
    MidiMappingItem<T> midi;

    MappingItem(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value))
        : instance(instance)
        , value_f(initValue)
        , key(_key)
        , callback(_callback)
        , midi(instance, _key, _callback)
    {
        // set getter to return float
    }

    MappingItem(T* instance, uint8_t initValue, const char* _key, T& (T::*_callback)(float value))
        : instance(instance)
        , value_i(initValue)
        , key(_key)
        , callback(_callback)
        , midi(instance, _key, _callback)
    {
        // set getter to return int / 128
    }
};

template <typename T>
class Mapping {
protected:
    T* instance;
    std::vector<MappingItem<T>> items;

public:
    Mapping(T* _instance)
        : instance(_instance)
    {
    }

    bool midi(std::vector<unsigned char>* message)
    {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].midi.handle(message)) {
                return true;
            }
        }
        return false;
    }

    float& addFloat(float initvalue, const char* _key, T& (T::*_callback)(float value))
    {
        MappingItem<T> item(instance, initvalue, _key, _callback);
        items.push_back(item);
        return items.back().value_f;
    }

    uint8_t& addInt(uint8_t initvalue, const char* _key, T& (T::*_callback)(float value))
    {
        MappingItem<T> item(instance, initvalue, _key, _callback);
        items.push_back(item);
        return items.back().value_i;
    }

    bool assignMidi(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1)
    {
        for (int i = 0; i < items.size(); i++) {
            if (strcmp(items[i].key, key) == 0) {
                items[i].midi.set(size, valuePosition, msg0, msg1);
                return true;
            }
        }
        return false;
    }

    float getValue(int valueIndex)
    {
        return items[valueIndex].value_f;
    }
    void setValue(int valueIndex, float value)
    {
        (instance->*(items[valueIndex].callback))(value);
    }
    int getValueCount()
    {
        return items.size();
    }
    const char* getValueName(int valueIndex)
    {
        return items[valueIndex].key;
    }
};

#endif