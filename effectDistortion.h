#ifndef _EFFECT_DISTORTION_H_
#define _EFFECT_DISTORTION_H_

#include "def.h"
#include "audioPlugin.h"

#include <math.h>

class EffectDistortion: public AudioPlugin {
protected:
    float shape;

    float (EffectDistortion::*samplePtr)(float) = &EffectDistortion::skipSample;

    float skipSample(float buf)
    {
        return buf;
    }

    float processSample(float buf)
    {
        if (buf == 0.0) {
            return buf;
        }

        return (1 + shape) * buf / (1 + shape * fabsf(buf));
    }

public:
    float drive = 0.0;

    EffectDistortion()
    {
        set(drive);
    };

    float sample(float buf)
    {
        return (this->*samplePtr)(buf);
    }

    EffectDistortion& set(float _drive)
    {
        drive = range(_drive, 0.0, 1.0);
        if (drive == 0.0) {
            samplePtr = &EffectDistortion::skipSample;
            debug("Distortion: disabled\n");
        } else {
            samplePtr = &EffectDistortion::processSample;
            shape = 2 * (drive - 0.000001) / (1 - (drive - 0.000001));
            debug("Distortion: drive=%f shape=%f\n", drive, shape);
        }
        return *this;
    }
};

#endif
