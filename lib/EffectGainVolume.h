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
    float gain = 1.0f;
    float volume = 1.0f;
    float volumeWithGain = gain * volume;

    EffectGainVolume(AudioPluginProps& props)
        : AudioPlugin(props) {};

    float sample(float in)
    {
        return in * volumeWithGain;
    }

    void setVolume(float vol)
    {
        debug("setMaster: volume %f\n", vol);
        setVolumeWithGain(vol, gain);
    }

    EffectGainVolume& setVolumeWithGain(float vol, float _gain)
    {
        gain = _gain;
        volume = vol;
        volumeWithGain = gain * vol;

        return *this;
    }

    void setGain(float _gain)
    {
        debug("setGain: gain %f\n", _gain);
        setVolumeWithGain(volume, _gain);
    }

    const char* name()
    {
        return "EffectGainVolume";
    }

    bool set(uint16_t param, float value)
    {
        return setters.set(param, value);
    }

    int16_t getParamKey(const char* name)
    {
        return setters.getParamKey(name);
    }
};

#endif
