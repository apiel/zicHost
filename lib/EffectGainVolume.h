#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"

#include <cstdio> // printf to be removed

class EffectGainVolume : public AudioPlugin {
public:
    float gain = 1.0f;
    float volume = 1.0f;
    float volumeWithGain = gain * volume;

    EffectGainVolume(AudioPluginProps& props)
        : AudioPlugin(props) {};

    float sample(float in)
    {
        return in * volumeWithGain;
    }

    EffectGainVolume& set(float vol)
    {
        return set(vol, gain);
    }

    EffectGainVolume& set(float vol, float _gain)
    {
        gain = _gain;
        volume = vol;
        volumeWithGain = gain * vol;

        debug("setMaster: volume %f\n", vol);
        return *this;
    }

    EffectGainVolume& setGain(float _gain)
    {
        debug("setGain: gain %f\n", _gain);
        return set(volume, _gain);
    }

    const char* name()
    {
        return "EffectGainVolume";
    }
};

#endif
