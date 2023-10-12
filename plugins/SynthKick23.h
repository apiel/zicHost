#ifndef _SYNTH_KICK23_H_
#define _SYNTH_KICK23_H_

#include <sndfile.h>

#include "audioPlugin.h"
#include "fileBrowser.h"
#include "mapping.h"
#include "utils/envelop.h"

#define ZIC_WAVETABLE_WAVEFORMS_COUNT 64

class SynthKick23 : public Mapping<SynthKick23> {
protected:
    SF_INFO sfinfo;
    SNDFILE* file = NULL;
    uint64_t sampleRate;

    uint64_t sampleCount = 2048; // should this be configurable?
    static const uint64_t bufferSize = ZIC_WAVETABLE_WAVEFORMS_COUNT * 2048;
    uint64_t bufferSampleCount = 0;
    float bufferSamples[bufferSize];

    FileBrowser fileBrowser = FileBrowser("../zicHost/wavetables");

    float pitchMult = 1.0f;

    float sampleIndex = 0.0f;
    uint64_t sampleStart = 0;

    unsigned int sampleCountDuration;
    unsigned int sampleDurationCounter = 0;

    Envelop envelopAmp = Envelop({ { 0.0f, 0.0f }, { 1.0f, 0.01f }, { 0.3f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } });
    Envelop envelopFreq = Envelop({ { 1.0f, 0.0f }, { 0.26f, 0.03f }, { 0.24f, 0.35f }, { 0.22f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } });

public:
    Val<SynthKick23>& browser = val(this, 0.0f, "BROWSER", &SynthKick23::open, { "Browser", fileBrowser.count, VALUE_STRING });
    Val<SynthKick23>& morph = val(this, 0.0f, "MORPH", &SynthKick23::setMorph, { "Morph", ZIC_WAVETABLE_WAVEFORMS_COUNT, .stepStart = 1 }); // 640, .stepStart = 10, .stepMultiplier = 0.1 });
    Val<SynthKick23>& pitch = val(this, 0.5f, "PITCH", &SynthKick23::setPitch, { "Pitch", .unit = "%", .stepStart = 50 });
    Val<SynthKick23>& duration = val(this, 0.1f, "DURATION", &SynthKick23::setDuration, { "Duration", 100, .unit = "ms", .stepStart = 50, .stepMultiplier = 50 });

    SynthKick23(AudioPlugin::Props& props, char* _name)
        : Mapping(props, _name)
        , sampleRate(props.sampleRate)
    {
        memset(&sfinfo, 0, sizeof(sfinfo));
        open(0.0, true);

        setPitch(pitch.get());
        setDuration(duration.get());
    }

    void sample(float* buf)
    {
        if (sampleDurationCounter < sampleCountDuration) {
            float time = (float)sampleDurationCounter / (float)sampleCountDuration;
            float envFreq = envelopFreq.next(time);
            float envAmp = envelopAmp.next(time);

            sampleIndex += pitchMult * envFreq;
            while (sampleIndex >= sampleCount) {
                sampleIndex -= sampleCount;
            }
            sampleDurationCounter++;
            buf[track] = bufferSamples[(uint16_t)sampleIndex + sampleStart] * envAmp;
        }
    }

    SynthKick23& setPitch(float value)
    {
        pitch.setFloat(value);
        pitchMult = pitch.get() + 0.5f;
        return *this;
    }

    SynthKick23& setMorph(float value)
    {
        morph.setFloat(value);
        sampleStart = morph.get() * bufferSampleCount;
        uint64_t max = bufferSampleCount / ZIC_WAVETABLE_WAVEFORMS_COUNT * (ZIC_WAVETABLE_WAVEFORMS_COUNT - 1); // TODO make this better :p
        if (sampleStart > max) {
            sampleStart = max;
        }
        // printf(">>>>>>>>>>>>>>.... sampleStart: %ld (%f bufferSampleCount %ld)\n", sampleStart, morph.get(), bufferSampleCount);
        return *this;
    }

    SynthKick23& setDuration(float value)
    {
        duration.setFloat(value);
        sampleCountDuration = duration.getAsInt() * (sampleRate * 0.0001f);
        // printf(">>>>>>>>>>>>>>.... sampleCountDuration: %d (%d)\n", sampleCountDuration, duration.getAsInt());
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
        sampleCount = bufferSampleCount / (float)ZIC_WAVETABLE_WAVEFORMS_COUNT;

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
        envelopAmp.reset();
        envelopFreq.reset();
    }
};

#endif
