#ifndef _FILTER_H_
#define _FILTER_H_

#include "audioPlugin.h"

class EffectFilterInterface : public AudioPlugin {
public:
    float resonance = 0.0;

    EffectFilterInterface(AudioPluginProps& props)
        : AudioPlugin(props)
    {
    }

    virtual float sample(float inputValue) = 0;
    virtual EffectFilterInterface& setCutoff(float value) = 0;
    virtual EffectFilterInterface& setResonance(float _res) = 0;
};

// https://www.martin-finke.de/articles/audio-plugins-013-filter/
// https://www.musicdsp.org/en/latest/Filters/29-resonant-filter.html

class EffectFilterData {

public:
    float cutoff = 0.0f;
    float feedback = 0.0f;
    float buf0 = 0.0f;
    float buf1 = 0.0f;
    float hp = 0.0f;
    float bp = 0.0f;
    float resonance = 0.0f;

    void setCutoff(float _cutoff)
    {
        cutoff = _cutoff;
        setResonance(resonance);
    }

    void setResonance(float _resonance)
    {
        resonance = _resonance;
        if (resonance == 0.0f) {
            feedback = 0.0f;
            return;
        }
        feedback = resonance + resonance / (1.0 - cutoff);
    }

    void setSampleData(float inputValue)
    {
        hp = inputValue - buf0;
        bp = buf0 - buf1;

        buf0 = buf0 + cutoff * (hp + feedback * bp);
        buf1 = buf1 + cutoff * (buf0 - buf1);
    }
};

#endif
