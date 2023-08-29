#ifndef _EFFECT_FILTER_H_
#define _EFFECT_FILTER_H_

#include "../helpers/range.h"
#include "midiMapping.h"
#include "filter.h"

// #include <math.h>

class EffectFilter : public EffectFilterInterface {
protected:
    EffectFilterData data;
    
    const static int16_t mapCount = 3;
    MidiMapping<EffectFilter> midiMappings[mapCount] = {
        MidiMapping<EffectFilter>(this, "SET_CUTOFF", &EffectFilter::setCutoff),
        MidiMapping<EffectFilter>(this, "SET_RESONANCE", &EffectFilter::setResonance),
        MidiMapping<EffectFilter>(this, "SET_MODE", &EffectFilter::setMode),
    };

public:
    MIDI_MAPPING_HANDLER
    
    float resonance = 0.0;
    float cutoff = 0.0;

    enum Mode {
        OFF,
        LPF,
        HPF,
        BPF,
        MODE_COUNT,
    } mode
        = OFF;

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
        mode = (Mode)range(value * 128, 0, (uint8_t)MODE_COUNT);
        return setMode(mode);
    }

    EffectFilter(AudioPluginProps& props)
        : EffectFilterInterface(props)
    {
    }

    const char* name()
    {
        return "EffectFilter";
    }
};

#endif
