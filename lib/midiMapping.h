#ifndef _MIDI_MAPPING_H_
#define _MIDI_MAPPING_H_

#include <stdint.h>
#include <vector>

#define MIDI_MAPPING_HANDLER     bool midi(std::vector<unsigned char>* message)\
    {\
        return midiMapping.handle(message);\
    }\
    bool assignMidiMapping(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1)\
    {\
        return midiMapping.assign(key, size, valuePosition, msg0, msg1);\
    }



template <typename T>
class MidiMappingItem {
protected:
    T* instance;
    bool (MidiMappingItem::*handlePtr)(std::vector<unsigned char>* message) = &MidiMappingItem::handleNone;
    uint8_t size = 0;
    uint8_t msg[2] = { 0x00, 0x00 };

    bool handleNone(std::vector<unsigned char>* message)
    {
        return false;
    }

    bool handleUint8Position1(std::vector<unsigned char>* message)
    {
        (instance->*(callback))(message->at(1) / 128.0f);
        return true;
    }

    bool handleUint8Position2(std::vector<unsigned char>* message)
    {
        if (msg[1] == message->at(1)) {
            (instance->*(callback))(message->at(2) / 128.0f);
            return true;
        }
        return false;
    }

    bool handleUint16(std::vector<unsigned char>* message)
    {
        (instance->*(callback))(((message->at(2) << 7) + message->at(1)) / 16383.0f);
        return true;
    }

public:
    const char* key;
    T& (T::*callback)(float value);

    MidiMappingItem(T* instance, const char* _key, T& (T::*_callback)(float value))
        : instance(instance)
        , key(_key)
        , callback(_callback)
    {
    }

    bool handle(std::vector<unsigned char>* message)
    {
        if (message->size() != size) {
            return false;
        }
        if (msg[0] != message->at(0)) {
            return false;
        }
        return (this->*handlePtr)(message);
    }

    MidiMappingItem& set(uint8_t _size, uint8_t valuePosition, uint8_t _msg0, uint8_t _msg1)
    {
        msg[0] = _msg0;
        msg[1] = _msg1;
        size = _size;
        if (valuePosition == _size) {
            if (size == 2) {
                handlePtr = &MidiMappingItem::handleUint8Position1;
            } else if (size == 3) {
                handlePtr = &MidiMappingItem::handleUint8Position2;
            }
        } else {
            handlePtr = &MidiMappingItem::handleUint16;
        }
        return *this;
    }
};

template <typename T>
class MidiMapping {
protected:
    T* instance;
    std::vector<MidiMappingItem<T>> items;

public:
    MidiMapping(T* _instance)
        : instance(_instance)
    {
    }

    bool handle(std::vector<unsigned char>* message)
    {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].handle(message)) {
                return true;
            }
        }
        return false;
    }

    MidiMapping& add(const char* _key, T& (T::*_callback)(float value))
    {
        MidiMappingItem<T> item(instance, _key, _callback);
        items.push_back(item);
        return *this;
    }

    bool assign(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1)
    {
        for (int i = 0; i < items.size(); i++) {
            if (strcmp(items[i].key, key) == 0) {
                items[i].set(size, valuePosition, msg0, msg1);
                return true;
            }
        }
        return false;
    }
};

#endif