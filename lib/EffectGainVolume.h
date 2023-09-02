#ifndef _EFFECT_GAIN_VOLUME_H_
#define _EFFECT_GAIN_VOLUME_H_

#include "audioPlugin.h"
#include "midiMapping.h"

class EffectGainVolume : public AudioPlugin {
protected:
    MidiMapping<EffectGainVolume> midiMapping;

public:
    MIDI_MAPPING_HANDLER

    float gain
        = 1.0f;
    float volume = 1.0f;
    float volumeWithGain = gain * volume;

    EffectGainVolume(AudioPluginProps& props)
        : AudioPlugin(props)
        , midiMapping(this)
    {
        midiMapping.add("VOLUME", &EffectGainVolume::setVolume);
        midiMapping.add("GAIN", &EffectGainVolume::setGain);
    };

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
