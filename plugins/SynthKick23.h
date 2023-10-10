#ifndef _SYNTH_KICK23_H_
#define _SYNTH_KICK23_H_

#include <sndfile.h>

#include "audioPlugin.h"
#include "fileBrowser.h"
#include "mapping.h"

#define ZIC_KICK_ENVELOP_STEPS 7

class SynthKick23 : public Mapping<SynthKick23> {
protected:
    SF_INFO sfinfo;
    SNDFILE* file = NULL;
    uint64_t sampleRate;

    uint64_t sampleCount = 2048; // should this be configurable?
    static const uint64_t bufferSize = 64 * 2048;
    uint64_t bufferSampleCount = 0;
    float bufferSamples[bufferSize];

    FileBrowser fileBrowser = FileBrowser("../zicHost/wavetables");

    float freqMult = 0.0f;
    // float frequency = 600.0f;
    // unsigned int duration = 300; // in ms

    float sampleIndex = 0.0f;

    unsigned int sampleCountDuration; // = duration * SAMPLE_PER_MS;
    unsigned int sampleDurationCounter = 0;

    unsigned int envelopAmpIndex = 0;
    unsigned int envelopFreqIndex = 0;

    // The first 2 steps are readonly, so for amp env there is very short ramp up to avoid clicking noize
    // The last step is also readonly, so the amp and freq end to 0.0f
    float envelopAmp[ZIC_KICK_ENVELOP_STEPS][2] = { { 0.0f, 0.0f }, { 1.0f, 0.01f }, { 0.3f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } };
    float envelopFreq[ZIC_KICK_ENVELOP_STEPS][2] = { { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.26f, 0.03f }, { 0.24f, 0.35f }, { 0.22f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } };

    float envelop(float (*envelop)[2], unsigned int* envelopIndex)
    {
        if (envelopFreqIndex > ZIC_KICK_ENVELOP_STEPS - 1) {
            return 0.0f;
        }

        float time = (float)sampleDurationCounter / (float)sampleCountDuration;
        if (time >= envelop[*envelopIndex + 1][1]) {
            (*envelopIndex)++;
        }
        float timeOffset = envelop[*envelopIndex + 1][1] - envelop[*envelopIndex][1];
        float timeRatio = (time - envelop[*envelopIndex][1]) / timeOffset;
        return (envelop[*envelopIndex + 1][0] - envelop[*envelopIndex][0]) * timeRatio + envelop[*envelopIndex][0];
    }

public:
    Val<SynthKick23>& browser = val(this, 0.0f, "BROWSER", &SynthKick23::open, { "Browser", fileBrowser.count, VALUE_STRING });
    Val<SynthKick23>& frequency = val(this, 1.0f, "FREQUENCY", &SynthKick23::setFrequency, { "Frequency", 2000, .unit = "Hz", .stepStart = 50 });
    Val<SynthKick23>& duration = val(this, 0.1f, "DURATION", &SynthKick23::setFrequency, { "Duration", 4990, .unit = "ms", .stepStart = 10 });

    SynthKick23(AudioPlugin::Props& props, char* _name)
        : Mapping(props, _name)
        , sampleRate(props.sampleRate)
    {
        sampleCountDuration = sampleCount * props.sampleRate * 0.0001f;
    }

    void sample(float* buf)
    {
        if (sampleDurationCounter < sampleCountDuration) {
            float envFreq = envelop(envelopFreq, &envelopFreqIndex);
            float envAmp = envelop(envelopAmp, &envelopAmpIndex);

            sampleIndex += freqMult * envFreq;
            while (sampleIndex >= sampleCount) {
                sampleIndex -= sampleCount;
            }
            sampleDurationCounter++;
            buf[track] = bufferSamples[(uint16_t)sampleIndex] * envAmp;
        }
    }

    SynthKick23& setFrequency(float value)
    {
        frequency.setFloat(value);
        freqMult = sampleCount * frequency.getAsInt() / (float)sampleRate;
        return *this;
    }

    SynthKick23& setDuration(float value)
    {
        return *this;
    }

    SynthKick23& close()
    {
        if (file) {
            sf_close(file);
        }
        return *this;
    }

    SynthKick23& open(const char* filename)
    {
        close();

        if (!(file = sf_open(filename, SFM_READ, &sfinfo))) {
            debug("Error: could not open file %s\n", filename);
            return *this;
        }
        debug("Audio file %s sampleCount %ld sampleRate %d\n", filename, (long)sfinfo.frames, sfinfo.samplerate);

        bufferSampleCount = sf_read_float(file, bufferSamples, bufferSize);

        return *this;
    }

    SynthKick23& open(float value, bool force)
    {
        browser.setFloat(value);
        int position = browser.get() * fileBrowser.count;
        if (force || position != fileBrowser.position) {
            char* filepath = fileBrowser.getFilePath(position);
            browser.setString(fileBrowser.getFile(position));
            debug("KICK23_SAMPLE_SELECTOR: %f %s\n", value, filepath);
            open(filepath);
        }
        return *this;
    }

    SynthKick23& open(float value)
    {
        open(value, false);
        return *this;
    }

    void noteOn(uint8_t note, uint8_t velocity) override
    {
        printf("kick noteOn\n");

        // Could change the frequency base on the note...
        // Could change the amplitude base on the velocity...
        sampleIndex = 0;
        sampleDurationCounter = 0;
        envelopAmpIndex = 0;
        envelopFreqIndex = 0;
    }
};

#endif
