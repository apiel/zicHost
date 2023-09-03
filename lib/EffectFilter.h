#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "mapping.h"

// #include <math.h>

class EffectFilter : public EffectFilterInterface {
protected:
    EffectFilterData data;
    Mapping<EffectFilter> mapping;

public:
    MAPPING_HANDLER

    float& resonance = mapping.addFloat(0.0f, "RESONANCE", &EffectFilter::setResonance);
    float& cutoff = mapping.addFloat(0.0f, "CUTOFF", &EffectFilter::setCutoff);

    enum Mode {
        OFF,
        LPF,
        HPF,
        BPF,
        MODE_COUNT,
    } mode
        = OFF;
    // TODO how to handle mode in a better way?
    float& f_mode = mapping.addFloat(0.0f, "MODE", &EffectFilter::setMode);

    EffectFilter(AudioPluginProps& props)
        : EffectFilterInterface(props)
        , mapping(this)
    {
    }

    float sample(float inputValue)
    {
        if (inputValue == 0 || mode == OFF) {
            return inputValue;
        }

        data.setSampleData(inputValue);

        if (mode == HPF) {
            return data.hp;
        } else if (mode == BPF) {
            return data.bp;
        }
        // LPF
        return data.buf0;
    }

    EffectFilter& setCutoff(float value)
    {
        cutoff = range(value, 0.00, 1.00);

        if (mode == LPF) {
            data.setCutoff(0.85 * value + 0.1);
        } else if (mode == BPF) {
            data.setCutoff(0.85 * value + 0.1);
        } else { // HPF
            data.setCutoff((0.20 * value) + 0.00707);
        }

        return *this;
    }

    EffectFilter& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 0.99);
        data.setResonance(resonance);

        return *this;
    };

    EffectFilter& setMode(EffectFilter::Mode _mode)
    {
        mode = _mode;
        setCutoff(cutoff);
        return *this;
    };

    EffectFilter& setMode(float value)
    {
        f_mode = value;
        mode = (Mode)range(f_mode * 128, 0, (uint8_t)MODE_COUNT);
        return setMode(mode);
    }

    const char* name()
    {
        return "Filter";
    }
};

#endif
