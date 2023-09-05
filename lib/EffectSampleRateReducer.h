#ifndef _EFFECT_SAMPLE_RATE_REDUCER_H_
#define _EFFECT_SAMPLE_RATE_REDUCER_H_

#include "audioPlugin.h"
#include "mapping.h"

#include <math.h>

class EffectSampleRateReducer : public Mapping<EffectSampleRateReducer> {
protected:
    float sampleSqueeze;
    int samplePosition = 0;
    int sampleStepInt = 0;

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

        if (samplePosition < sampleStepInt) {
            samplePosition++;
        } else {
            samplePosition = 0;
        }

        return sampleSqueeze;
    }

public:
    Val<EffectSampleRateReducer> sampleStep = { this, 0, "SAMPLE_STEP", &EffectSampleRateReducer::setSampleStep }; // the number of samples to double up.

    EffectSampleRateReducer(AudioPluginProps& props)
        : Mapping(props, { &sampleStep })
    {
        setSampleStep(0);
    };

    EffectSampleRateReducer& setSampleStep(float value)
    {
        sampleStep.set(value);
        // FIXME is this right?
        // or should we just convert float to int
        // and make min = 0.0 and max = 256.0
        // then we could also bring this max value to the frontend
        sampleStepInt = sampleStep.get() * 100;
        if (sampleStepInt == 0) {
            samplePtr = &EffectSampleRateReducer::skipSample;
            debug("SampleRateReducer: disabled\n");
        } else {
            samplePtr = &EffectSampleRateReducer::processSample;
            debug("SampleRateReducer: sampleStep=%d\n", sampleStepInt);
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
