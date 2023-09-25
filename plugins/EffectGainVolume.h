#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"
#include "mapping.h"

class EffectGainVolume : public Mapping<EffectGainVolume> {
public:
    Val<EffectGainVolume>& volume = val(this, 1.0f, "VOLUME", &EffectGainVolume::setVolume, { "Volume" });
    Val<EffectGainVolume>& gain = val(this, 0.0f, "GAIN", &EffectGainVolume::setGain, { "Gain" });
    float volumeWithGain = volume.get();

    EffectGainVolume(AudioPlugin::Props& props)
        : Mapping(props)
    {
        setVolumeWithGain(volume.get(), gain.get());
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
        gain.setFloat(_gain);
        volume.setFloat(vol);
        volumeWithGain = (1.0 + (gain.get() * 4.0)) * volume.get();

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
