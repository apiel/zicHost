#ifndef _MAPPING_INTERFACE_H_
#define _MAPPING_INTERFACE_H_

#include <stdint.h>

enum ValueType {
    VALUE_BASIC,
    VALUE_CENTERED,
    VALUE_CENTERED_ONE_SIDED,
    VALUE_STRING,
};

class ValueInterface {
public:
    struct Props {
        const char* label = nullptr;
        int stepCount = 100;
        ValueType type = VALUE_BASIC;
        const char* unit = nullptr;
        int stepStart = 0;
    };

    virtual const char* key() = 0;
    virtual Props& props() = 0;
    virtual const char* label() = 0;
    virtual inline float get() = 0;
    virtual char* string() = 0;
    virtual void set(float value) = 0;
    virtual void onUpdate(void (*callback)(float, void* data), void* data) = 0;
};

#endif