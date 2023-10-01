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
    Val<EffectFilterMultiMode>& mix = val(this, 0.5, "CUTOFF", &EffectFilterMultiMode::setCutoff, { "LPF | HPF", .type = VALUE_CENTERED });
    Val<EffectFilterMultiMode>& resonance = val(this, 0.0, "RESONANCE", &EffectFilterMultiMode::setResonance, { "Resonance", .unit = "%" });

    EffectFilterMultiMode(AudioPlugin::Props& props, char * _name)
        : Mapping(props, _name)
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

    EffectFilterMultiMode& setCutoff(float value)
    {
        mix.setFloat(value);
        hpf.setCutoff((0.20 * mix.get()) + 0.00707);
        lpf.setCutoff(0.85 * mix.get() + 0.1);

        return *this;
    }

    EffectFilterMultiMode& setResonance(float value)
    {
        resonance.setFloat(value);
        lpf.setResonance(resonance.get());
        hpf.setResonance(resonance.get());

        debug("Filter: resonance=%f\n", resonance.get());

        return *this;
    };
};

#endif
