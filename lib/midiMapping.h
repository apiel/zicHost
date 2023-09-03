#ifndef _MIDI_MAPPING_H_
#define _MIDI_MAPPING_H_

#include <vector>

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

#endif