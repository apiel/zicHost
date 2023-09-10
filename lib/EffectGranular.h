#ifndef _EFFECT_GRANULAR_H_
#define _EFFECT_GRANULAR_H_

#include <ctime>
#include <math.h>

#include "audioBuffer.h"
#include "audioPlugin.h"
#include "mapping.h"

#define GRANULER_BUFFER_SECONDS 30
#define MAX_GRAINS_PER_VOICE 24
#define MAX_GRAIN_VOICES 4
#define MIN_GRAIN_SIZE_MS 20

const uint16_t minGrainSampleCount = MIN_GRAIN_SIZE_MS * SAMPLE_RATE * 0.001f;

class EffectGranular : public Mapping<EffectGranular> {
protected:
    AudioBuffer<GRANULER_BUFFER_SECONDS> buffer;

    uint64_t voicePosition = 0;
    float attackStep = 0.0f;
    float releaseStep = 0.0f;

    uint8_t densityUint8 = 4;

    struct Grain {
        float pos;
        int64_t start;
        int64_t sampleCount;
        int64_t delay;
        float sampleStep;
    };

    struct Voice {
        int8_t note = -1;
        Grain grains[MAX_GRAINS_PER_VOICE];
        float (EffectGranular::*envelop)(Voice& voice) = &EffectGranular::envelopSustain;
        float env = 0.0f;
        uint64_t position = 0;
    } voices[MAX_GRAIN_VOICES];

    uint8_t baseNote = 60;
    float getSampleStep(uint8_t note)
    {
        // https://gist.github.com/YuxiUx/ef84328d95b10d0fcbf537de77b936cd
        // pow(2, ((note - 0) / 12.0)) = 1 for 0 semitone
        // pow(2, ((note - 1) / 12.0)) = 1.059463094 for 1 semitone
        // pow(2, ((note - 2) / 12.0)) = 1.122462048 for 2 semitone
        // ...
        return pow(2, ((note - baseNote) / 12.0));
    }

    void initGrain(Grain& grain)
    {
        initGrain(grain, grain.sampleStep);
    }

    int getRand()
    {
        // could create a lookup table ?
        srand(time(0));
        return rand();
    }

    float getRandPct()
    {
        return getRand() / (float)RAND_MAX;
    }

    void initGrain(Grain& grain, float sampleStep)
    {
        grain.sampleStep = sampleStep;
        grain.pos = 0.0f;

        // sprayToAdd is a random value between 0 and spray from starting point till end of file
        float sprayToAdd = spray.get() ? (getRandPct() * spray.get() * (1 - start.get())) : 0.0;
        grain.start = (start.get() + sprayToAdd) * buffer.size;

        // we deduct minGrainSampleCount to avoid grainSize to be too small
        grain.sampleCount = (buffer.size - (grain.start + minGrainSampleCount)) * grainSize.get() + minGrainSampleCount;

        // delayInt = delay.get() * SAMPLE_RATE * 0.001f * 1000;
        // can be simplified to:
        // delayInt = delay.get() * SAMPLE_RATE;
        grain.delay = delay.get() > 0 ? (getRand() % (int)(delay.get() * SAMPLE_RATE)) : 0;
    }

    float envelopAttack(Voice& voice)
    {
        voice.env += attackStep;
        if (voice.env >= 1.0f) {
            voice.env = 1.0f;
            voice.envelop = &EffectGranular::envelopSustain;
            // debug("envelopAttack finished, set env to %f\n", voice.env);
        }
        return voice.env;
    }

    float envelopSustain(Voice& voice)
    {
        return voice.env;
    }

    float envelopRelease(Voice& voice)
    {
        voice.env -= releaseStep;
        if (voice.env <= 0.0f) {
            voice.env = 0.0f;
            voice.note = -1;
            // debug("envelopRelease finished, set env to %f\n", voice.env);
        }
        return voice.env;
    }

    float sample(Voice& voice)
    {
        float sample = 0.0f;
        float env = (this->*voice.envelop)(voice);
        if (env <= 0.0f) {
            return sample;
        }

        for (uint8_t d = 0; d < densityUint8; d++) {
            Grain& grain = voice.grains[d];
            if (grain.delay > 0) {
                grain.delay--;
            } else {
                int64_t samplePos = (uint64_t)grain.pos + grain.start;
                // if (samplePos < buffer->size && (int64_t)grain.pos < grainSampleCount) { // is samplePos < buffer->size even necessary if start calculated properly
                if ((int64_t)grain.pos < grain.sampleCount) {
                    grain.pos += grain.sampleStep;
                    sample += buffer.samples[samplePos] * env;
                } else {
                    initGrain(grain);
                }
            }
        }
        return sample;
    }

    Voice& getNextVoice(uint8_t note)
    {
        // First, we should look if the voice is not already running, due to the envelopRelease
        for (uint8_t v = 0; v < MAX_GRAIN_VOICES; v++) {
            Voice& voice = voices[v];
            if (voice.note == note) {
                debug("getNextVoice: voice already running %d\n", note);
                return voice;
            }
        }

        // Else, we should look for a free voice
        uint8_t voiceToSteal = 0;
        for (uint8_t v = 0; v < MAX_GRAIN_VOICES; v++) {
            Voice& voice = voices[v];
            if (voice.note == -1) {
                return voice;
            }
            if (voice.position < voices[voiceToSteal].position) {
                voiceToSteal = v;
            }
        }

        debug("getNextVoice: no voice available. Steal voice %d.\n", voiceToSteal);
        return voices[voiceToSteal];
    }

public:
    Val<EffectGranular> mix = { this, 1.0f, "MIX", &EffectGranular::setMix, { "Mix" } };
    Val<EffectGranular> start = { this, 0.0f, "START", &EffectGranular::setStart, { "Start" } };
    Val<EffectGranular> spray = { this, 0.0f, "SPRAY", &EffectGranular::setSpray, { "Spray" } };
    Val<EffectGranular> grainSize = { this, 0.5f, "GRAIN_SIZE", &EffectGranular::setGrainSize, { "Grain Size" } };
    Val<EffectGranular> density = { this, 1 / MAX_GRAINS_PER_VOICE * 4, "DENSITY", &EffectGranular::setDensity, { "Density", MAX_GRAINS_PER_VOICE } };
    Val<EffectGranular> attack = { this, 1 / 5000 * 20, "ATTACK", &EffectGranular::setAttack, { "Attack", 5000 } };
    Val<EffectGranular> release = { this, 1 / 10000 * 50, "RELEASE", &EffectGranular::setRelease, { "Release", 10000 } };
    Val<EffectGranular> delay = { this, 0, "DELAY", &EffectGranular::setDelay, { "Delay", 1000 } };

    EffectGranular(AudioPluginProps& props)
        : Mapping(props, { &mix, &start, &spray, &grainSize, &density, &attack, &release })
    {
        setAttack(attack.get());
        setRelease(release.get());
    }

    EffectGranular& setMix(float value)
    {
        mix.set(value);
        return *this;
    }

    /**
     * @brief Set the Grain Size meaning the length duration of the grain.
     *
     * @param grainSize
     * @return EffectGranular&
     */
    EffectGranular& setGrainSize(float value)
    {
        grainSize.set(value);
        debug("grainSize %f\n", grainSize.get());
        return *this;
    }

    /**
     * @brief Set the Spray of the grain start position, giving random position to
     * the grain start position.
     *
     * @param spray
     * @return EffectGranular&
     */
    EffectGranular& setSpray(float value)
    {
        spray.set(value);
        debug("spray %f\n", spray.get());
        return *this;
    }

    /**
     * @brief Set the Density meaning the number of grains that are played at the same time.
     *
     * @param density
     * @return EffectGranular&
     */
    EffectGranular& setDensity(float value)
    {
        density.set(value);
        densityUint8 = value * (MAX_GRAINS_PER_VOICE - 1) + 1; // 1 to MAX_GRAINS_PER_VOICE
        debug("density %d\n", densityUint8);
        return *this;
    }

    /**
     * @brief Set the Start position of the sample to play
     *
     * @param _start position from 0.0 to 1.0, where 0.0 is the start of the sample
     * and 1.0 the end of the sample
     * @return EffectGranular&
     */
    EffectGranular& setStart(float value)
    {
        start.set(value);
        debug("setStart %f\n", start.get());
        return *this;
    }

    /**
     * @brief Set the Delay before grain start to play
     *
     * @param delay where 0 is no delay and 1 is 1000ms
     * @return EffectGranular&
     */
    EffectGranular& setDelay(float value)
    {
        delay.set(value);
        debug("delay %f\n", delay.get());
        return *this;
    }

    /**
     * @brief Set the Attack time of the voice
     *
     * @param attack where 0 is no attack and 1 is 5000ms
     * @return EffectGranular&
     */
    EffectGranular& setAttack(float value)
    {
        attack.set(value);
        // uint64_t attackSamples = attack.get() * SAMPLE_RATE * 0.001f * 5000;
        // can be simplified to:
        uint64_t attackSamples = attack.get() * SAMPLE_RATE * 5;
        attackStep = 1.0f / attackSamples;
        debug("attack %ld samples %f step\n", attackSamples, attackStep);
        return *this;
    }

    /**
     * @brief Set the Release time of the voice
     *
     * @param release where 0 is no release and 1 is 10000ms
     * @return EffectGranular&
     */
    EffectGranular& setRelease(float value)
    {
        release.set(value);
        // uint64_t releaseSamples = release.get() * SAMPLE_RATE * 0.001f * 10000;
        // can be simplified to:
        uint64_t releaseSamples = release.get() * SAMPLE_RATE * 10;
        releaseStep = 1.0f / releaseSamples;
        debug("release %ld samples %f step\n", releaseSamples, releaseStep);
        return *this;
    }

    float sample(float in)
    {
        float s = 0.0f;
        for (uint8_t v = 0; v < MAX_GRAIN_VOICES; v++) {
            Voice& voice = voices[v];
            if (voice.note != -1) {
                s += sample(voice);
            }
        }
        return s * mix.get() + in * (1 - mix.get());
    }

    EffectGranular& noteOn(uint8_t note, uint8_t velocity)
    {
        if (velocity == 0) {
            return noteOff(note, velocity);
        }

        Voice& voice = getNextVoice(note);
        voice.position = voicePosition++;
        voice.note = note;
        voice.envelop = &EffectGranular::envelopAttack;
        float sampleStep = getSampleStep(note);
        for (uint8_t g = 0; g < densityUint8; g++) {
            initGrain(voice.grains[g], sampleStep);
        }
        debug("noteOn: %d %d %f\n", note, velocity, sampleStep);

        return *this;
    }

    EffectGranular& noteOff(uint8_t note, uint8_t velocity)
    {
        for (uint8_t v = 0; v < MAX_GRAIN_VOICES; v++) {
            Voice& voice = voices[v];
            if (voice.note == note) {
                voice.envelop = &EffectGranular::envelopRelease;
                debug("noteOff set on to false: %d %d\n", note, velocity);
                return *this;
            }
        }

        debug("noteOff: note not found %d %d\n", note, velocity);
        return *this;
    }

    const char* name()
    {
        return "Granular";
    }
};

#endif