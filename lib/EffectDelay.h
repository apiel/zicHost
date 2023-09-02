#ifndef _EFFECT_DELAY_H_
#define _EFFECT_DELAY_H_

#include "EffectFilter.h"
#include "audioBuffer.h"
#include "audioPlugin.h"
#include "midiMapping.h"

#define MAX_DELAY_VOICES 8

// TODO load/save different kind of delay and reverb from a config file
// TODO add lfo on time ratio

class EffectDelay : public AudioPlugin {
protected:
    AudioBuffer<> buffer;

    struct DelayVoice {
        uint32_t index;
        float amplitude;
        float feedback;
        float sec;
    } voices[MAX_DELAY_VOICES] = {
        { 0, 0.0f, 0.0f, 0.1f },
    };

    // FIXME
    // TODO handle voices values
    MidiMapping<EffectDelay> midiMapping;

public:
    MIDI_MAPPING_HANDLER

    // From 0.0 to 1.0 to apply time ratio to voice in seconds
    float timeRatio = 1.0f;
    float masterAmplitude = 1.0f;

    EffectFilter filter;

    EffectDelay(AudioPluginProps& props)
        : AudioPlugin(props)
        , filter(props)
        , midiMapping(this)
    {
        midiMapping.add("TIME_RATIO", &EffectDelay::setTimeRatio);
        midiMapping.add("MASTER_AMPLITUDE", &EffectDelay::setMasterAmplitude);
        midiMapping.add("FILTER_CUTOFF", &EffectDelay::setCutoff);
        midiMapping.add("FILTER_RESONANCE", &EffectDelay::setResonance);
        midiMapping.add("FILTER_MODE", &EffectDelay::setMode);
        midiMapping.add("SEC_0", &EffectDelay::setSec0);
        midiMapping.add("SEC_1", &EffectDelay::setSec1);
        midiMapping.add("SEC_2", &EffectDelay::setSec2);
        midiMapping.add("SEC_3", &EffectDelay::setSec3);
        midiMapping.add("SEC_4", &EffectDelay::setSec4);
        midiMapping.add("SEC_5", &EffectDelay::setSec5);
        midiMapping.add("SEC_6", &EffectDelay::setSec6);
        midiMapping.add("SEC_7", &EffectDelay::setSec7);
        midiMapping.add("AMPLITUDE_0", &EffectDelay::setAmplitude0);
        midiMapping.add("AMPLITUDE_1", &EffectDelay::setAmplitude1);
        midiMapping.add("AMPLITUDE_2", &EffectDelay::setAmplitude2);
        midiMapping.add("AMPLITUDE_3", &EffectDelay::setAmplitude3);
        midiMapping.add("AMPLITUDE_4", &EffectDelay::setAmplitude4);
        midiMapping.add("AMPLITUDE_5", &EffectDelay::setAmplitude5);
        midiMapping.add("AMPLITUDE_6", &EffectDelay::setAmplitude6);
        midiMapping.add("AMPLITUDE_7", &EffectDelay::setAmplitude7);
        midiMapping.add("FEEDBACK_0", &EffectDelay::setFeedback0);
        midiMapping.add("FEEDBACK_1", &EffectDelay::setFeedback1);
        midiMapping.add("FEEDBACK_2", &EffectDelay::setFeedback2);
        midiMapping.add("FEEDBACK_3", &EffectDelay::setFeedback3);
        midiMapping.add("FEEDBACK_4", &EffectDelay::setFeedback4);
        midiMapping.add("FEEDBACK_5", &EffectDelay::setFeedback5);
        midiMapping.add("FEEDBACK_6", &EffectDelay::setFeedback6);
        midiMapping.add("FEEDBACK_7", &EffectDelay::setFeedback7);
        
        // setVoice(0, 0.1f, 0.6f, 0.0f);
        // setVoice(1, 0.2f, 0.5f, 0.0f);
        // setVoice(2, 0.3f, 0.4f, 0.0f);
        // setVoice(3, 0.4f, 0.3f, 0.0f);
        // setVoice(4, 0.5f, 0.2f, 0.0f);

        // filter.setCutoff(0.5f).setResonance(0.95f).setMode(EffectFilter::Mode::HPF);

        // // make reverb
        // setVoice(0, 0.05f, 0.9f, 0.0f);
        // setVoice(1, 0.10f, 0.85f, 0.0f);
        // setVoice(2, 0.15f, 0.8f, 0.0f);
        // setVoice(3, 0.20f, 0.75f, 0.0f);
        // setVoice(4, 0.25f, 0.7f, 0.0f);
        // setVoice(5, 0.30f, 0.65f, 0.0f);
        // setVoice(6, 0.35f, 0.6f, 0.0f);
        // setVoice(7, 0.40f, 0.55f, 0.0f);
    }

    EffectDelay& setSec0(float sec) { return setSec(0, sec); }
    EffectDelay& setSec1(float sec) { return setSec(1, sec); }
    EffectDelay& setSec2(float sec) { return setSec(2, sec); }
    EffectDelay& setSec3(float sec) { return setSec(3, sec); }
    EffectDelay& setSec4(float sec) { return setSec(4, sec); }
    EffectDelay& setSec5(float sec) { return setSec(5, sec); }
    EffectDelay& setSec6(float sec) { return setSec(6, sec); }
    EffectDelay& setSec7(float sec) { return setSec(7, sec); }

    EffectDelay& setSec(uint8_t voiceIndex, float sec)
    {
        voices[voiceIndex].sec = sec;
        voices[voiceIndex].index = (buffer.index + buffer.size - (uint32_t)(SAMPLE_RATE * sec * timeRatio)) % buffer.size;
        return *this;
    }

    EffectDelay& setAmplitude0(float amplitude) { return setAmplitude(0, amplitude); }
    EffectDelay& setAmplitude1(float amplitude) { return setAmplitude(1, amplitude); }
    EffectDelay& setAmplitude2(float amplitude) { return setAmplitude(2, amplitude); }
    EffectDelay& setAmplitude3(float amplitude) { return setAmplitude(3, amplitude); }
    EffectDelay& setAmplitude4(float amplitude) { return setAmplitude(4, amplitude); }
    EffectDelay& setAmplitude5(float amplitude) { return setAmplitude(5, amplitude); }
    EffectDelay& setAmplitude6(float amplitude) { return setAmplitude(6, amplitude); }
    EffectDelay& setAmplitude7(float amplitude) { return setAmplitude(7, amplitude); }

    EffectDelay& setAmplitude(uint8_t voiceIndex, float amplitude)
    {
        voices[voiceIndex].amplitude = range(amplitude, 0.0f, 1.0f);
        return *this;
    }

    EffectDelay& setFeedback0(float feedback) { return setFeedback(0, feedback); }
    EffectDelay& setFeedback1(float feedback) { return setFeedback(1, feedback); }
    EffectDelay& setFeedback2(float feedback) { return setFeedback(2, feedback); }
    EffectDelay& setFeedback3(float feedback) { return setFeedback(3, feedback); }
    EffectDelay& setFeedback4(float feedback) { return setFeedback(4, feedback); }
    EffectDelay& setFeedback5(float feedback) { return setFeedback(5, feedback); }
    EffectDelay& setFeedback6(float feedback) { return setFeedback(6, feedback); }
    EffectDelay& setFeedback7(float feedback) { return setFeedback(7, feedback); }

    EffectDelay& setFeedback(uint8_t voiceIndex, float feedback)
    {
        voices[voiceIndex].feedback = range(feedback, 0.0f, 1.0f);
        return *this;
    }

    EffectDelay& setVoice(uint8_t voiceIndex, float sec, float amplitude, float feedback)
    {
        setSec(voiceIndex, sec);
        setAmplitude(voiceIndex, amplitude);
        setFeedback(voiceIndex, feedback);
        return *this;
    }

    EffectDelay& setFeedback(float feedback)
    {
        for (uint8_t i = 0; i < MAX_DELAY_VOICES; i++) {
            setFeedback(i, feedback);
        }
        return *this;
    }

    EffectDelay& setMasterAmplitude(float amplitude)
    {
        masterAmplitude = range(amplitude, 0.0f, 1.0f);
        return *this;
    }

    EffectDelay& setTimeRatio(float ratio)
    {
        timeRatio = range(ratio, 0.0f, 1.0f);
        for (uint8_t i = 0; i < MAX_DELAY_VOICES; i++) {
            setSec(i, voices[i].sec);
        }
        return *this;
    }

    EffectDelay& setCutoff(float cutoff)
    {
        filter.setCutoff(cutoff);
        return *this;
    }

    EffectDelay& setResonance(float resonance)
    {
        filter.setResonance(resonance);
        return *this;
    }

    EffectDelay& setMode(float mode)
    {
        filter.setMode(mode);
        return *this;
    }

    float sample(float in)
    {
        buffer.addSample(in);

        float delay = 0.0f;
        for (uint8_t i = 0; i < MAX_DELAY_VOICES; i++) {
            DelayVoice& voice = voices[i];
            if (voice.index++ >= buffer.size) {
                voice.index = 0;
            }
            if (masterAmplitude && voice.amplitude > 0.0f) {
                delay += buffer.samples[voice.index] * voice.amplitude * masterAmplitude;
                if (voice.feedback > 0.0f) {
                    buffer.samples[buffer.index] += delay * voice.feedback;
                }
            }
        }

        return in + filter.sample(delay);
    }

    const char* name()
    {
        return "EffectDelay";
    }
};

#endif