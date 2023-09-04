#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "mapping.h"

class EffectFilterMultiMode : public Mapping<EffectFilterMultiMode> {
protected:
    EffectFilterData hpf;
    EffectFilterData lpf;

public:
    // Cutoff mix
    Val<EffectFilterMultiMode> mix = { this, 0.5, "CUTOFF", &EffectFilterMultiMode::setCutoff };
    Val<EffectFilterMultiMode> resonance = { this, 0.0, "RESONANCE", &EffectFilterMultiMode::setResonance, 0.00, 0.99 };

    EffectFilterMultiMode(AudioPluginProps& props)
        : Mapping(props, { &mix, &resonance })
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

        return lpf.buf1 * (1.0 - mix.value_f) + hpf.hp * mix.value_f;
    }

    EffectFilterMultiMode& setCutoff(float value)
    {
        mix.set(value);
        hpf.setCutoff((0.20 * value) + 0.00707);
        lpf.setCutoff(0.85 * value + 0.1);

        return *this;
    }

    EffectFilterMultiMode& setResonance(float value)
    {
        resonance.set(value);
        lpf.setResonance(resonance.value_f);
        hpf.setResonance(resonance.value_f);

        debug("Filter: resonance=%f\n", resonance.value_f);

        return *this;
    };

    const char* name()
    {
        return "MultiModeFilter";
    }
};

#endif
