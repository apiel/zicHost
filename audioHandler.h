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

#ifdef AUDIO_FORMAT_INT32_TO_FLOAT
void samples(int32_t* in, int32_t* out32, int len)
{
    float out[len];
#else
void samples(float* in, float* out, int len)
{
#endif

        for (int i = 0; i < len; i++) {
            out[i] = in[i];
            #ifdef AUDIO_FORMAT_INT32_TO_FLOAT
            out[i] = in[i] / 2147483647.0f;
            #endif

            for (int j = 0; j < pluginCount; j++) {
                out[i] = plugins[j].instance->sample(out[i]);
            }
            
            #ifdef AUDIO_FORMAT_INT32_TO_FLOAT
            out32[i] = out[i] * 2147483647.0f;
            #endif
        }
    }

    void loadPlugin(const char* path)
    {
        AudioPluginProps pluginProps = AudioPluginProps(debug);

        if (pluginCount >= AUDIO_PLUGIN_COUNT) {
            APP_PRINT("Cannot load plugin: %s, reached max audio plugin count %d\n", path, AUDIO_PLUGIN_COUNT);
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
            APP_PRINT("Cannot load symbol: %s\n", dlsym_error);
            dlclose(plugins[pluginCount].handle);
            return;
        }

        plugins[pluginCount].instance = ((AudioPlugin * (*)(AudioPluginProps & props)) allocator)(pluginProps);
        APP_PRINT("audio plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugins[pluginCount].instance->name());

        // plugins[pluginCount].instance->set(0, 0.1f);
        // printf("---> getParamKey: %d\n", plugins[pluginCount].instance->getParamKey("volume"));

        pluginCount++;
    }

    bool assignMidiMapping(char* key, char* value)
    {
        if (pluginCount > 0) {
            // split value by space
            char* msg0 = strtok(value, " ");
            char* msg1 = strtok(NULL, " ");
            char* msg2 = strtok(NULL, " ");

            if (msg0 == NULL || msg1 == NULL) {
                APP_INFO("Invalid midi mapping\n");
                return false;
            }

            uint8_t size = msg2 == NULL ? 2 : 3;
            uint8_t valuePosition = msg1[0] == 'x' && msg1[1] == 'x' ? 2 : 3;
            uint8_t msg0Int = strtol(msg0, NULL, 16);
            uint8_t msg1Int = strtol(msg1, NULL, 16);

            if (plugins[pluginCount - 1].instance->assignMidiMapping(key, size, valuePosition, msg0Int, msg1Int)) {
                APP_INFO("[%s] Midi mapping assigned: %s\n", plugins[pluginCount - 1].instance->name(), key);
                return true;
            }
        }
        return false;
    }

    bool midi(std::vector<unsigned char>* message)
    {
        for (int i = 0; i < pluginCount; i++) {
            if (plugins[i].instance->midi(message)) {
                return true;
            }
        }
        return false;
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif