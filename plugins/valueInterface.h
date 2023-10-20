#ifndef _MAPPING_INTERFACE_H_
#define _MAPPING_INTERFACE_H_

#include <stdint.h>

enum ValueType {
    VALUE_BASIC,
    VALUE_CENTERED,
    VALUE_STRING,
};

class ValueInterface {
public:
    struct Props {
        const char* label = nullptr;
        int stepCount = 101; // 100 + 1 for 0
        ValueType type = VALUE_BASIC;
        const char* unit = nullptr;
        int (*asInt)(int value) = [](int value) { return value; };
    };

    virtual const char* key() = 0;
    virtual Props& props() = 0;
    virtual const char* label() = 0;
    virtual inline float get() = 0;
    virtual void increment(int8_t steps) = 0;
    virtual inline int getAsInt() = 0;
    virtual char* string() = 0;
    virtual void set(float value) = 0;
    virtual void onUpdate(void (*callback)(float, void* data), void* data) = 0;
};

#endif