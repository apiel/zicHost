#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "mapping.h"

// #include <math.h>

class EffectFilter : public Mapping<EffectFilter> {
protected:
    EffectFilterData data;

public:
    // Cutoff mix
    Val<EffectFilter> cutoff = { this, 0.5, "CUTOFF", &EffectFilter::setCutoff };
    Val<EffectFilter> resonance = { this, 0.0, "RESONANCE", &EffectFilter::setResonance };

    enum Mode {
        OFF,
        LPF,
        HPF,
        BPF,
        MODE_COUNT,
    } mode
        = OFF;
    // TODO how to handle mode in a better way?
    Val<EffectFilter> mode_value = { this, 0.0, "MODE", &EffectFilter::setMode };

    EffectFilter(AudioPluginProps& props)
        : Mapping(props, { &cutoff, &resonance, &mode_value })
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
        cutoff.set(value);

        if (mode == LPF) {
            data.setCutoff(0.85 * cutoff.get() + 0.1);
        } else if (mode == BPF) {
            data.setCutoff(0.85 * cutoff.get() + 0.1);
        } else { // HPF
            data.setCutoff((0.20 * cutoff.get()) + 0.00707);
        }

        return *this;
    }

    EffectFilter& setResonance(float value)
    {
        resonance.set(value);
        data.setResonance(resonance.get());

        return *this;
    };

    EffectFilter& setMode(EffectFilter::Mode _mode)
    {
        mode = _mode;
        setCutoff(cutoff.get());
        return *this;
    };

    EffectFilter& setMode(float value)
    {
        mode_value.set(value);
        mode = (Mode)range(mode_value.get() * 100, 0, (uint8_t)MODE_COUNT);
        return setMode(mode);
    }

    const char* name()
    {
        return "Filter";
    }
};

#endif
