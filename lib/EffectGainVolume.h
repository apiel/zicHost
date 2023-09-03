#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"
#include "mapping.h"

class EffectGainVolume : public AudioPlugin {
protected:
    Mapping<EffectGainVolume> mapping;

public:
    MAPPING_HANDLER

    float& volume = mapping.addFloat(1.0f, "VOLUME", &EffectGainVolume::setVolume);
    float& gain = mapping.addFloat(1.0f, "GAIN", &EffectGainVolume::setGain);
    float volumeWithGain = gain * volume;

    EffectGainVolume(AudioPluginProps& props)
        : AudioPlugin(props)
        , mapping(this)
    {
    }

    float sample(float in)
    {
        return in * volumeWithGain;
    }

    EffectGainVolume& setVolume(float vol)
    {
        debug("setMaster: volume %f\n", vol);
        return setVolumeWithGain(vol, gain);
    }

    EffectGainVolume& setVolumeWithGain(float vol, float _gain)
    {
        gain = _gain;
        volume = vol;
        volumeWithGain = gain * vol;

        return *this;
    }

    EffectGainVolume& setGain(float _gain)
    {
        debug("setGain: gain %f\n", _gain);
        return setVolumeWithGain(volume, _gain);
    }

    const char* name()
    {
        return "GainVolume";
    }
};

#endif
