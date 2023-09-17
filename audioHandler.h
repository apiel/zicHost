#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include <vector>

#include "def.h"
#include "midiMapping.h"
#include "plugin.h"

class AudioHandler {
protected:
    std::vector<MidiMapping> midiMapping;

    static AudioHandler* instance;
    AudioHandler() { }

    float sample(float s)
    {
        for (Plugin& plugin : plugins) {
            s = plugin.instance->sample(s);
        }
        return s;
    }

    bool assignMidiMapping(char* key, char* value)
    {
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

        // try to assign value to last plugin
        int valueIndex = plugins.back().instance->getValueIndex(key);
        if (valueIndex != -1) {
            midiMapping.push_back({ plugins.back().instance, valueIndex, size, valuePosition, msg0Int, msg1Int });
            APP_INFO("[%s] Midi mapping assigned: %s\n", plugins.back().instance->name(), key);
            return true;
        }
        return false;
    }

public:
    std::vector<Plugin> plugins;

    static AudioHandler& get()
    {
        if (!instance) {
            instance = new AudioHandler();
        }
        return *instance;
    }

    void samples(int32_t* in32, int32_t* out32, int len)
    {
        for (int i = 0; i < len; i++) {
            out32[i] = sample(in32[i] / 2147483647.0f) * 2147483647.0f;
        }
    }

    void samples(float* in, float* out, int len)
    {
        for (int i = 0; i < len; i++) {
            // out[i] = sample(in[i]);
            out[i] = sample(0);
        }
    }

    void loadPlugin(const char* path)
    {
        AudioPlugin::Props pluginProps = { debug, SAMPLE_RATE, APP_CHANNELS };

        Plugin plugin;

        plugin.handle = dlopen(path, RTLD_LAZY);

        if (!plugin.handle) {
            APP_PRINT("Cannot open library: %s\n", dlerror());
            return;
        }

        dlerror();
        void* allocator = (AudioPlugin*)dlsym(plugin.handle, "allocator");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            APP_PRINT("Cannot load symbol: %s\n", dlsym_error);
            dlclose(plugin.handle);
            return;
        }

        plugin.instance = ((AudioPlugin * (*)(AudioPlugin::Props & props)) allocator)(pluginProps);
        APP_PRINT("audio plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugin.instance->name());

        // plugin.instance->set(0, 0.1f);
        // printf("---> getParamKey: %d\n", plugin.instance->getParamKey("volume"));

        plugins.push_back(plugin);
    }

    bool config(char* key, char* value)
    {
        if (plugins.size() > 0) {
            if (plugins.back().instance->config(key, value)) {
                return true;
            } else {
                return assignMidiMapping(key, value);
            }
        }
        return false;
    }

    bool midi(std::vector<unsigned char>* message)
    {
        for (MidiMapping& mapping : midiMapping) {
            if (mapping.handle(message)) {
                return true;
            }
        }
        return false;
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif