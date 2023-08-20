#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "audioBuffer.h"
#include "def.h"
#include "lib/audioPlugin.h"

// #include "effectDelay.h"
// #include "effectDistortion.h"
// #include "effectFilter.h"
// #include "effectSampleRateReducer.h"
// #include "effectGainVolume.h"
// #include "synthGranular.h"

class AudioHandler {
protected:
    static AudioHandler* instance;

    // Keep buffer for echo, delay, granular, etc.
    AudioBuffer<> buffer;

    void* handle;

    AudioHandler()
    // : delay(&buffer)
    {

        handle = dlopen("./lib/build/libzic_EffectSampleRateReducer.so", RTLD_LAZY);

        if (!handle) {
            APP_PRINT("Cannot open library: %s\n", dlerror());
            return;
        }

        dlerror();
        void * allocator = (AudioPlugin*)dlsym(handle, "allocator");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            APP_PRINT("Cannot load symbol 'hello': %s\n", dlsym_error);
            dlclose(handle);
            return;
        }

        plugin = ((AudioPlugin* (*)())allocator)();
        APP_PRINT("plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugin->name());
    }

public:
    // // EffectFilterMultiMode filter;
    // EffectFilterMultiMode2 filter;
    // // EffectFilterMultiModeMoog filter;
    // EffectDistortion distortion;
    // EffectSampleRateReducer sampleRateReducer;
    // EffectDelay delay;
    // SynthGranular synthGranular;
    // EffectGainVolume gainVolume;

    AudioPlugin* plugin;

    static AudioHandler& get()
    {
        if (!instance) {
            instance = new AudioHandler();
        }
        return *instance;
    }

    void samples(float* in, float* out, int len)
    {
        for (int i = 0; i < len; i++) {
            out[i] = in[i];
            // // TODO make a list of AudioPlugin and loop through them
            // out[i] = synthGranular.sample(out[i]);
            // out[i] = filter.sample(out[i]);
            // out[i] = distortion.sample(out[i]);
            // out[i] = sampleRateReducer.sample(out[i]);
            // buffer.addSample(out[i]);

            // out[i] = delay.sample(out[i]);
            // out[i] = gainVolume.sample(out[i]);

            out[i] = plugin->sample(out[i]);
        }
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif