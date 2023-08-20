#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "audioBuffer.h"
#include "def.h"
#include "lib/audioPlugin.h"

#ifndef AUDIO_PLUGIN_COUNT
#define AUDIO_PLUGIN_COUNT 4
#endif

class AudioHandler {
protected:
    static AudioHandler* instance;

    // Keep buffer for echo, delay, granular, etc.
    AudioBuffer<> buffer;

    struct Plugin {
        void* handle;
        AudioPlugin* instance;
    } plugins[AUDIO_PLUGIN_COUNT];

    uint16_t pluginCount = 0;

    AudioHandler()
    // : delay(&buffer)
    {
    }

public:
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

            // TODO this must be a plugin as well!! or should there even a shared audio buffer between plugins?...
            // buffer.addSample(out[i]);

            // out[i] = delay.sample(out[i]);
            // out[i] = gainVolume.sample(out[i]);

            for (int j = 0; j < pluginCount; j++) {
                out[i] = plugins[j].instance->sample(out[i]);
            }
        }
    }

    void loadPlugin(const char* path)
    {
        if (pluginCount >= AUDIO_PLUGIN_COUNT) {
            APP_PRINT("Cannot load plugin: %s, reached max plugin count %d\n", path, AUDIO_PLUGIN_COUNT);
            return;
        }

        plugins[pluginCount].handle = dlopen(path, RTLD_LAZY);

        if (!plugins[pluginCount].handle) {
            APP_PRINT("Cannot open library: %s\n", dlerror());
            return;
        }

        dlerror();
        void* allocator = (AudioPlugin*)dlsym(plugins[pluginCount].handle, "allocator");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            APP_PRINT("Cannot load symbol 'hello': %s\n", dlsym_error);
            dlclose(plugins[pluginCount].handle);
            return;
        }

        plugins[pluginCount].instance = ((AudioPlugin * (*)()) allocator)();
        APP_PRINT("plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugins[pluginCount].instance->name());

        pluginCount++;
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif