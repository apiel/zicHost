#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <stdint.h>
#include <vector>

#include "midiMapping.h"

#define MAPPING_HANDLER                                                                                      \
    bool midi(std::vector<unsigned char>* message)                                                           \
    {                                                                                                        \
        return mapping.midi(message);                                                                  \
    }                                                                                                        \
    bool assignMidiMapping(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1) \
    {                                                                                                        \
        return mapping.assignMidi(key, size, valuePosition, msg0, msg1);                                     \
    }

template <typename T>
class MappingItem {
protected:
    T* instance;

public:
    const char* key;
    float value;
    T& (T::*callback)(float value);
    MidiMappingItem<T> midi;

    MappingItem(T* instance, float initValue, const char* _key, T& (T::*_callback)(float value))
        : instance(instance)
        , value(initValue)
        , key(_key)
        , callback(_callback)
        , midi(instance, _key, _callback)
    {
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
        // return items[items.size() - 1].value;
        return items.back().value;
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
};

#endif