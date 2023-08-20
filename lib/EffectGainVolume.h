#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"
#include "audioPluginSetter.h"

class EffectGainVolume : public AudioPlugin {
protected:
    const static int16_t setterCount = 2;
    AudioPluginSetter<EffectGainVolume> setterList[setterCount] = {
        { &EffectGainVolume::setVolume, "VOLUME" },
        { &EffectGainVolume::setGain, "GAIN" },
    };
    AudioPluginSetters<EffectGainVolume> setters = AudioPluginSetters<EffectGainVolume>(this, setterList, setterCount);

public:
    SETTER_METHODS

    float gain = 1.0f;
    float volume = 1.0f;
    float volumeWithGain = gain * volume;

    EffectGainVolume(AudioPluginProps& props)
        : AudioPlugin(props) {};

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
        return "EffectGainVolume";
    }
};

#endif
