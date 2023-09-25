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
    Val<EffectSampleRateReducer>& sampleStep = val(this, 0, "SAMPLE_STEP", &EffectSampleRateReducer::setSampleStep, { "Step Reducer", .stepCount = 256, .unit = "steps" });

    EffectSampleRateReducer(AudioPlugin::Props& props)
        : Mapping(props)
    {
        setSampleStep(0);
    };

    EffectSampleRateReducer& setSampleStep(float value)
    {
        sampleStep.setFloat(value);
        sampleStepInt = sampleStep.get() * sampleStep.props().stepCount;
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
