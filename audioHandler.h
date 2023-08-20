#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include "def.h"
#include "lib/audioPlugin.h"

#ifndef AUDIO_PLUGIN_COUNT
#define AUDIO_PLUGIN_COUNT 100
#endif

class AudioHandler {
protected:
    static AudioHandler* instance;

    struct Plugin {
        void* handle;
        AudioPlugin* instance;
    } plugins[AUDIO_PLUGIN_COUNT];

    uint16_t pluginCount = 0;

    AudioHandler()
    {
        printf("AudioHandler constructor\n");
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
            for (int j = 0; j < pluginCount; j++) {
                out[i] = plugins[j].instance->sample(out[i]);
            }
        }
    }

    void loadPlugin(const char* path)
    {
        AudioPluginProps pluginProps = AudioPluginProps(debug);

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

        plugins[pluginCount].instance = ((AudioPlugin * (*)(AudioPluginProps & props)) allocator)(pluginProps);
        APP_PRINT("plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugins[pluginCount].instance->name());

        plugins[pluginCount].instance->set(0, 0.5f);
        printf("---> getParamKey: %d\n", plugins[pluginCount].instance->getParamKey("volume"));

        pluginCount++;
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif