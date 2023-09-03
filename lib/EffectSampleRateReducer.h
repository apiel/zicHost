#ifndef _EFFECT_SAMPLE_RATE_REDUCER_H_
#define _EFFECT_SAMPLE_RATE_REDUCER_H_

#include "audioPlugin.h"
#include "mapping.h"

#include <math.h>

class EffectSampleRateReducer : public AudioPlugin {
protected:
    float sampleSqueeze;
    int samplePosition = 0;

    float (EffectSampleRateReducer::*samplePtr)(float) = &EffectSampleRateReducer::skipSample;

    float skipSample(float buf)
    {
        return buf;
    }

    float processSample(float buf)
    {
        if (samplePosition == 0) {
            sampleSqueeze = buf;
        }

        if (samplePosition < sampleStep) {
            samplePosition++;
        } else {
            samplePosition = 0;
        }

        return sampleSqueeze;
    }

    Mapping<EffectSampleRateReducer> mapping;

public:
    MAPPING_HANDLER

    float& f_sampleStep = mapping.addFloat(0.0, "SAMPLE_STEP", &EffectSampleRateReducer::setSampleStep);
    uint8_t sampleStep = 0; // the number of samples to double up.

    EffectSampleRateReducer(AudioPluginProps& props)
        : AudioPlugin(props)
        , mapping(this)
    {
        setSampleStep(0);
        // set(0.5);
    };

    EffectSampleRateReducer& setSampleStep(float value)
    {
        f_sampleStep = value;
        // TODO find better solution...
        sampleStep = value * 128.0;
        if (sampleStep == 0) {
            samplePtr = &EffectSampleRateReducer::skipSample;
            debug("SampleRateReducer: disabled\n");
        } else {
            samplePtr = &EffectSampleRateReducer::processSample;
            debug("SampleRateReducer: sampleStep=%d\n", sampleStep);
        }
        return *this;
    }

    float sample(float buf)
    {
        return (this->*samplePtr)(buf);
    }

    const char* name()
    {
        return "SampleRateReducer";
    }
};

#endif
