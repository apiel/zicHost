#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "mapping.h"

// #include <math.h>

// another version of the same filter but with a small clicking at 0.5

class EffectFilterMultiMode2 : public Mapping<EffectFilterMultiMode2> {
protected:
    EffectFilterData hpf;
    EffectFilterData lpf;

public:
    // Cutoff mix
    Val<EffectFilterMultiMode2> mix = { this, 0.5, "CUTOFF", &EffectFilterMultiMode2::setCutoff, { "LPF | HPF", .type = VALUE_CENTERED } };
    Val<EffectFilterMultiMode2> resonance = { this, 0.0, "RESONANCE", &EffectFilterMultiMode2::setResonance, { "Resonance" } };

    EffectFilterMultiMode2(AudioPlugin::Props& props)
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

        return lpf.buf1 * (1.0 - mix.get()) + hpf.hp * mix.get();
    }

    EffectFilterMultiMode2& setCutoff(float value)
    {
        mix.setFloat(value);

        if (value > 0.5) {
            // 0 to 0.10
            float cutoff = (0.10 * ((mix.get() - 0.5) * 2)) + 0.00707;
            hpf.setCutoff(cutoff);
            lpf.setCutoff(0.99);
        } else {
            // From 0.95 to 0.1
            float cutoff = 0.85 * (mix.get() * 2) + 0.1;
            hpf.setCutoff(0.00707);
            lpf.setCutoff(cutoff);
        }

        debug("Filter: cutoff=%f\n", mix.get());

        return *this;
    }

    EffectFilterMultiMode2& setResonance(float value)
    {
        resonance.setFloat(value);
        lpf.setResonance(resonance.get());
        hpf.setResonance(resonance.get());

        debug("Filter: resonance=%f\n", resonance.get());

        return *this;
    };

    const char* name()
    {
        return "MultiModeFilter2";
    }
};

#endif
