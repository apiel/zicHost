#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "midiMapping.h"

// #include <math.h>

// another version of the same filter but with a small clicking at 0.5

class EffectFilterMultiMode2 : public EffectFilterInterface {
protected:
    EffectFilterData hpf;
    EffectFilterData lpf;

    float mix = 0.5;

    MidiMapping<EffectFilterMultiMode2> midiMapping;

public:
    MIDI_MAPPING_HANDLER

    float resonance = 0.0;

    EffectFilterMultiMode2(AudioPluginProps& props)
        : EffectFilterInterface(props)
        , midiMapping(this)
    {
        midiMapping.add("SET_CUTOFF", &EffectFilterMultiMode2::setCutoff);
        midiMapping.add("SET_RESONANCE", &EffectFilterMultiMode2::setResonance);

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

    EffectFilterMultiMode2& setCutoff(float value)
    {
        mix = range(value, 0.00, 1.00);

        if (value > 0.5) {
            // 0 to 0.10
            float cutoff = (0.10 * ((value - 0.5) * 2)) + 0.00707;
            hpf.setCutoff(cutoff);
            lpf.setCutoff(0.99);
        } else {
            // From 0.95 to 0.1
            float cutoff = 0.85 * (value * 2) + 0.1;
            hpf.setCutoff(0.00707);
            lpf.setCutoff(cutoff);
        }

        debug("Filter: cutoff=%f\n", value);

        return *this;
    }

    EffectFilterMultiMode2& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 0.99);
        lpf.setResonance(resonance);
        hpf.setResonance(resonance);

        debug("Filter: resonance=%f\n", resonance);

        return *this;
    };

    const char* name()
    {
        return "EffectFilterMultiMode2";
    }
};

#endif
