#ifndef _EFFECT_DISTORTION_H_
#define _EFFECT_DISTORTION_H_

#include "../helpers/range.h"
#include "audioPlugin.h"
#include "mapping.h"

#include <math.h>

class EffectDistortion : public Mapping<EffectDistortion> {
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
    Val<EffectDistortion> drive = { this, 0.0, "DRIVE", &EffectDistortion::setDrive, { "Distortion" } };

    EffectDistortion(AudioPlugin::Props& props)
        : Mapping(props, { &drive })
    {
        setDrive(drive.get());
    };

    float sample(float buf)
    {
        return (this->*samplePtr)(buf);
    }

    EffectDistortion& setDrive(float value)
    {
        drive.setFloat(value);
        if (drive.get() == 0.0) {
            samplePtr = &EffectDistortion::skipSample;
            debug("Distortion: disabled\n");
        } else {
            samplePtr = &EffectDistortion::processSample;
            shape = 2 * (drive.get() - 0.000001) / (1 - (drive.get() - 0.000001));
            debug("Distortion: drive=%f shape=%f\n", drive.get(), shape);
        }
        return *this;
    }

    const char* name()
    {
        return "Distortion";
    }
};

#endif
