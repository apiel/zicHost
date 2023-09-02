#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "filter.h"
#include "midiMapping.h"

// #include <math.h>

// Paul Kellet version of the classic Stilson/Smith "Moog" filter
// https://www.kvraudio.com/forum/viewtopic.php?t=144625
class EffectFilterMultiModeMoog : public EffectFilterInterface {
protected:
    float cutoff = 0.00;
    float f, p, q = 0.00;
    float b0, b1, b2, b3, b4 = 0.0;
    float t1, t2 = 0.0;
    float mix = 0.5;

    void calculateVar(float _cutoff, float _resonance)
    {
        q = 1.0f - _cutoff;
        p = _cutoff + 0.8f * _cutoff * q;
        f = p + p - 1.0f;
        q = _resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));

        debug("mix %f cutoff %f q=%f\n", mix, _cutoff, q);
    }

    MidiMapping<EffectFilterMultiModeMoog> midiMapping;

public:
    MIDI_MAPPING_HANDLER

    float resonance = 0.0;

    EffectFilterMultiModeMoog(AudioPluginProps& props)
        : EffectFilterInterface(props)
        , midiMapping(this)
    {
        midiMapping.add("SET_CUTOFF", &EffectFilterMultiModeMoog::setCutoff);
        midiMapping.add("SET_RESONANCE", &EffectFilterMultiModeMoog::setResonance);

        setCutoff(0.5);
    };

    float sample(float inputValue)
    {
        inputValue -= q * b4; // feedback
        t1 = b1;
        b1 = (inputValue + b0) * p - b1 * f;
        t2 = b2;
        b2 = (b1 + t1) * p - b2 * f;
        t1 = b3;
        b3 = (b2 + t2) * p - b3 * f;
        b4 = (b3 + t1) * p - b4 * f;
        b4 = b4 - b4 * b4 * b4 * 0.166667f; // clipping
        b0 = inputValue;

        // Lowpass  output:  b4
        // Highpass output:  in - b4;
        // Bandpass output:  3.0f * (b3 - b4);

        return b4 * (1.0 - mix) + (inputValue - b4) * mix;
    }

    EffectFilterMultiModeMoog& setCutoff(float value)
    {
        mix = range(value, 0.00, 1.00);
        if (mix > 0.5) {
            cutoff = 1 - mix + 0.0707;
        } else {
            cutoff = mix + 0.05; // LPF should not be 0.0
        }
        calculateVar(cutoff, resonance);
        return *this;
    }

    EffectFilterMultiModeMoog& setResonance(float _res)
    {
        resonance = range(_res, 0.00, 1.00) * 0.90; // resonance should be from 0.0 to 0.90, higher values is too high
        calculateVar(cutoff, resonance);
        return *this;
    };

    const char* name()
    {
        return "EffectFilterMultiModeMoog";
    }
};

#endif
