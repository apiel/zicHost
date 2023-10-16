#ifndef _SYNTH_KICK23_H_
#define _SYNTH_KICK23_H_

#include <sndfile.h>

#include "audioPlugin.h"
#include "fileBrowser.h"
#include "mapping.h"
#include "utils/envelop.h"

#define ZIC_WAVETABLE_WAVEFORMS_COUNT 64
#define ZIC_KICK_ENV_AMP_STEP 4
#define ZIC_KICK_ENV_FREQ_STEP 4
#define ZIC_KICK_UI 1000

class SynthKick23 : public Mapping<SynthKick23> {
protected:
    SF_INFO sfinfo;
    SNDFILE* file = NULL;
    uint64_t sampleRate;

    uint64_t sampleCount = 2048; // should this be configurable?
    static const uint64_t bufferSize = ZIC_WAVETABLE_WAVEFORMS_COUNT * 2048;
    uint64_t bufferSampleCount = 0;
    float bufferSamples[bufferSize];
    float bufferUi[ZIC_KICK_UI];
    int updateUiState = 0;
    std::vector<Envelop::Data> * envelopUi = NULL;

    FileBrowser fileBrowser = FileBrowser("../zicHost/wavetables");

    float pitchMult = 1.0f;

    float sampleIndex = 0.0f;
    uint64_t sampleStart = 0;

    unsigned int sampleCountDuration;
    unsigned int sampleDurationCounter = 0;

    Envelop envelopAmp = Envelop({ { 0.0f, 0.0f }, { 1.0f, 0.01f }, { 0.3f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } });
    Envelop envelopFreq = Envelop({ { 1.0f, 0.0f }, { 0.26f, 0.03f }, { 0.24f, 0.35f }, { 0.22f, 0.4f }, { 0.0f, 1.0f }, { 0.0f, 1.0f } });

    float sample(float time, float* index, float envAmp, float envFreq)
    {
        // printf("(%f) envAmp %f envFreq %f\n", time, envAmp, envFreq);

        (*index) += pitchMult * envFreq;
        while ((*index) >= sampleCount) {
            (*index) -= sampleCount;
        }
        return bufferSamples[(uint16_t)(*index) + sampleStart] * envAmp;
    }

    void updateUi(std::vector<Envelop::Data> * envData)
    {
        envelopUi = envData;
        updateUiState++;
    }

public:
    Val<SynthKick23>& browser = val(this, 0.0f, "BROWSER", &SynthKick23::open, { "Browser", fileBrowser.count, VALUE_STRING });
    Val<SynthKick23>& morph = val(this, 0.0f, "MORPH", &SynthKick23::setMorph, { "Morph", ZIC_WAVETABLE_WAVEFORMS_COUNT, .stepStart = 1 }); // 640, .stepStart = 10, .stepMultiplier = 0.1 });
    Val<SynthKick23>& pitch = val(this, 0.5f, "PITCH", &SynthKick23::setPitch, { "Pitch", .unit = "%", .stepStart = 50 });
    Val<SynthKick23>& duration = val(this, 0.1f, "DURATION", &SynthKick23::setDuration, { "Duration", 100, .unit = "ms", .stepStart = 50, .stepMultiplier = 50 });

    Val<SynthKick23> envAmpMod[ZIC_KICK_ENV_AMP_STEP] = {
        { this, 0.5f, "ENVELOP_AMP_MOD_1", &SynthKick23::setEnvAmpMod1, { "Amp.Mod.1", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_MOD_2", &SynthKick23::setEnvAmpMod2, { "Amp.Mod.2", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_MOD_3", &SynthKick23::setEnvAmpMod3, { "Amp.Mod.3", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_MOD_4", &SynthKick23::setEnvAmpMod4, { "Amp.Mod.4", .unit = "%" } },
    };
    Val<SynthKick23> envAmpTime[ZIC_KICK_ENV_AMP_STEP] = {
        { this, 0.5f, "ENVELOP_AMP_TIME_1", &SynthKick23::setEnvAmpTime1, { "Amp.Time 1", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_TIME_2", &SynthKick23::setEnvAmpTime2, { "Amp.Time 2", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_TIME_3", &SynthKick23::setEnvAmpTime3, { "Amp.Time 3", .unit = "%" } },
        { this, 0.5f, "ENVELOP_AMP_TIME_4", &SynthKick23::setEnvAmpTime4, { "Amp.Time 4", .unit = "%" } },
    };
    Val<SynthKick23> envFreqMod[ZIC_KICK_ENV_FREQ_STEP] = {
        { this, 0.5f, "ENVELOP_FREQ_MOD_1", &SynthKick23::setEnvFreqMod1, { "Freq.Mod.1", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_MOD_2", &SynthKick23::setEnvFreqMod2, { "Freq.Mod.2", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_MOD_3", &SynthKick23::setEnvFreqMod3, { "Freq.Mod.3", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_MOD_4", &SynthKick23::setEnvFreqMod4, { "Freq.Mod.4", .unit = "%" } },
    };
    Val<SynthKick23> envFreqTime[ZIC_KICK_ENV_FREQ_STEP] = {
        { this, 0.5f, "ENVELOP_FREQ_TIME_1", &SynthKick23::setEnvFreqTime1, { "Freq.Time 1", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_TIME_2", &SynthKick23::setEnvFreqTime2, { "Freq.Time 2", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_TIME_3", &SynthKick23::setEnvFreqTime3, { "Freq.Time 3", .unit = "%" } },
        { this, 0.5f, "ENVELOP_FREQ_TIME_4", &SynthKick23::setEnvFreqTime4, { "Freq.Time 4", .unit = "%" } },
    };

    SynthKick23(AudioPlugin::Props& props, char* _name)
        : Mapping(props, _name, {
                                    // clang-format off
            &envAmpMod[0], &envAmpMod[1], &envAmpMod[2], &envAmpMod[3],
            &envAmpTime[0], &envAmpTime[1], &envAmpTime[2], &envAmpTime[3],
            &envFreqMod[0], &envFreqMod[1], &envFreqMod[2], &envFreqMod[3],
            &envFreqTime[0], &envFreqTime[1], &envFreqTime[2], &envFreqTime[3],
        }) // clang-format on
        , sampleRate(props.sampleRate)
    {
        memset(&sfinfo, 0, sizeof(sfinfo));
        open(0.0, true);

        setPitch(pitch.get());
        setDuration(duration.get());

        for (int i = 0; i < ZIC_KICK_ENV_AMP_STEP; i++) {
            envAmpMod[i].setFloat(envelopAmp.data[i + 2].modulation);
            envAmpTime[i].setFloat(envelopAmp.data[i + 2].time);
            if (i > 0) {
                envAmpTime[i - 1].props().max = envAmpTime[i].get();
            } else if (i < ZIC_KICK_ENV_AMP_STEP) {
                envAmpTime[i + 1].props().min = envAmpTime[i].get();
            }
        }
        for (int i = 0; i < ZIC_KICK_ENV_FREQ_STEP; i++) {
            envFreqMod[i].setFloat(envelopFreq.data[i + 1].modulation);
            envFreqTime[i].setFloat(envelopFreq.data[i + 1].time);
            if (i > 0) {
                envFreqTime[i - 1].props().max = envFreqTime[i].get();
            } else if (i < ZIC_KICK_ENV_FREQ_STEP ) {
                envFreqTime[i + 1].props().min = envFreqTime[i].get();
            }
        }
    }

    void sample(float* buf)
    {
        if (sampleDurationCounter < sampleCountDuration) {
            float time = (float)sampleDurationCounter / (float)sampleCountDuration;
            float envAmp = envelopAmp.next(time);
            float envFreq = envelopFreq.next(time);
            buf[track] = sample(time, &sampleIndex, envAmp, envFreq);
            sampleDurationCounter++;
        }
    }

    SynthKick23& setEnvAmpMod1(float value) { return setEnvAmpMod(value, 0); }
    SynthKick23& setEnvAmpMod2(float value) { return setEnvAmpMod(value, 1); }
    SynthKick23& setEnvAmpMod3(float value) { return setEnvAmpMod(value, 2); }
    SynthKick23& setEnvAmpMod4(float value) { return setEnvAmpMod(value, 3); }
    SynthKick23& setEnvAmpMod(float value, uint8_t index)
    {
        envAmpMod[index].setFloat(value);
        envelopAmp.data[index + 2].modulation = envAmpMod[index].get();
        updateUi(&envelopAmp.data);
        return *this;
    }

    SynthKick23& setEnvAmpTime1(float value) { return setEnvAmpTime(value, 0); }
    SynthKick23& setEnvAmpTime2(float value) { return setEnvAmpTime(value, 1); }
    SynthKick23& setEnvAmpTime3(float value) { return setEnvAmpTime(value, 2); }
    SynthKick23& setEnvAmpTime4(float value) { return setEnvAmpTime(value, 3); }
    SynthKick23& setEnvAmpTime(float value, uint8_t index)
    {
        envAmpTime[index].setFloat(value);
        envelopAmp.data[index + 2].time = envAmpTime[index].get();
        if (index > 0) {
            envAmpTime[index - 1].props().max = envAmpTime[index].get();
        } else if (index < ZIC_KICK_ENV_AMP_STEP) {
            envAmpTime[index + 1].props().min = envAmpTime[index].get();
        }
        updateUi(&envelopAmp.data);
        return *this;
    }

    SynthKick23& setEnvFreqMod1(float value) { return setEnvFreqMod(value, 0); }
    SynthKick23& setEnvFreqMod2(float value) { return setEnvFreqMod(value, 1); }
    SynthKick23& setEnvFreqMod3(float value) { return setEnvFreqMod(value, 2); }
    SynthKick23& setEnvFreqMod4(float value) { return setEnvFreqMod(value, 3); }
    SynthKick23& setEnvFreqMod(float value, uint8_t index)
    {
        envFreqMod[index].setFloat(value);
        envelopFreq.data[index + 1].modulation = envFreqMod[index].get();
        updateUi(&envelopFreq.data);
        return *this;
    }

    SynthKick23& setEnvFreqTime1(float value) { return setEnvFreqTime(value, 0); }
    SynthKick23& setEnvFreqTime2(float value) { return setEnvFreqTime(value, 1); }
    SynthKick23& setEnvFreqTime3(float value) { return setEnvFreqTime(value, 2); }
    SynthKick23& setEnvFreqTime4(float value) { return setEnvFreqTime(value, 3); }
    SynthKick23& setEnvFreqTime(float value, uint8_t index)
    {
        envFreqTime[index].setFloat(value);
        envelopFreq.data[index + 1].time = envFreqTime[index].get();
        if (index > 0) {
            envFreqTime[index - 1].props().max = envFreqTime[index].get();
        } else if (index < ZIC_KICK_ENV_FREQ_STEP) {
            envFreqTime[index + 1].props().min = envFreqTime[index].get();
        }
        updateUi(&envelopFreq.data);
        return *this;
    }

    SynthKick23& setPitch(float value)
    {
        pitch.setFloat(value);
        pitchMult = pitch.get() + 0.5f;
        updateUi(NULL);
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
        updateUi(NULL);
        // printf(">>>>>>>>>>>>>>.... sampleStart: %ld (%f bufferSampleCount %ld)\n", sampleStart, morph.get(), bufferSampleCount);
        return *this;
    }

    SynthKick23& setDuration(float value)
    {
        duration.setFloat(value);
        sampleCountDuration = duration.getAsInt() * (sampleRate * 0.0001f);
        updateUi(NULL);
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

        updateUi(NULL);

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
        // Could change the frequency base on the note...
        // Could change the amplitude base on the velocity...
        sampleIndex = 0;
        sampleDurationCounter = 0;
        envelopAmp.reset();
        envelopFreq.reset();
    }

    void* data(int id)
    {
        switch (id) {
        case 0:
            return &updateUiState;

        case 1: {
            unsigned int ampIndex = 0;
            unsigned int freqIndex = 0;
            float index = 0;
            for (int i = 0; i < ZIC_KICK_UI; i++) {
                float time = i / (float)ZIC_KICK_UI;
                float envAmp = envelopAmp.next(time, &ampIndex);
                float envFreq = envelopFreq.next(time, &freqIndex);
                bufferUi[i] = sample(time, &index, envAmp, envFreq);
            }
            return (void*)&bufferUi;
        }
        case 2:
            return envelopUi;
        }
        return NULL;
    }
};

#endif
