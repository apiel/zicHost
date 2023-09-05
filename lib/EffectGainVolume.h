#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"
#include "mapping.h"

class EffectGainVolume : public Mapping<EffectGainVolume> {
public:
    Val<EffectGainVolume> volume = { this, 1.0f, "VOLUME", &EffectGainVolume::setVolume };
    Val<EffectGainVolume> gain = { this, 1.0f, "GAIN", &EffectGainVolume::setGain };
    float volumeWithGain = gain.get() * volume.get();

    EffectGainVolume(AudioPluginProps& props)
        : Mapping(props, { &volume, &gain })
    {
    }

    float sample(float in)
    {
        return in * volumeWithGain;
    }

    EffectGainVolume& setVolume(float vol)
    {
        debug("setMaster: volume %f\n", vol);
        return setVolumeWithGain(vol, gain.get());
    }

    EffectGainVolume& setVolumeWithGain(float vol, float _gain)
    {
        gain.set(_gain);
        volume.set(vol);
        volumeWithGain = gain.get() * volume.get();

        return *this;
    }

    EffectGainVolume& setGain(float _gain)
    {
        debug("setGain: gain %f\n", _gain);
        return setVolumeWithGain(volume.get(), _gain);
    }

    const char* name()
    {
        return "GainVolume";
    }
};

#endif
