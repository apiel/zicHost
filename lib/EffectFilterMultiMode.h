#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "midiMapping.h"
#include "filter.h"

// #include <math.h>

class EffectFilterMultiMode : public EffectFilterInterface {
protected:
    EffectFilterData hpf;
    EffectFilterData lpf;

    float mix = 0.5;

public:
    float resonance = 0.0;

    EffectFilterMultiMode(AudioPluginProps& props)
        : EffectFilterInterface(props)
    {
        setCutoff(0.5);
    };

    float sample(float inputValue)
    {
        if (inputValue == 0) {
            return inputValue;
        }

        hpf.setSampleData(inputValue);
        lpf.setSampleData(inputValue);

        return lpf.buf1 * (1.0 - mix) + hpf.hp * mix;
    }

    EffectFilterMultiMode& setCutoff(float value)
    {
        mix = range(value, 0.00, 1.00);

        hpf.setCutoff((0.20 * value) + 0.00707);
        lpf.setCutoff(0.85 * value + 0.1);

        return *this;
    }

    EffectFilterMultiMode& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 0.99);
        lpf.setResonance(resonance);
        hpf.setResonance(resonance);

        debug("Filter: resonance=%f\n", resonance);

        return *this;
    };

    const char* name()
    {
        return "EffectFilterMultiMode";
    }
};

#endif