#ifndef _AUDIO_HANDLER_H_
#define _AUDIO_HANDLER_H_

#include <vector>

#include "def.h"
#include "lib/audioPlugin.h"

class AudioHandler {
protected:
    struct Plugin {
        void* handle;
        AudioPlugin* instance;
    };
    std::vector<Plugin> plugins;

    struct ValueIndex {
        int pluginIndex;
        int valueIndex;
        const char* pluginName;
        const char* valueName;
    };
    std::vector<ValueIndex> valueIndices;

    static AudioHandler* instance;
    AudioHandler() { }

    void setValueIndices(Plugin& plugin)
    {
        for (std::size_t i = 0; i < plugins.size(); i++) {
            for (int j = 0; j < plugins[i].instance->getValueCount(); j++) {
                valueIndices.push_back({ i, j, plugins[i].instance->name(), plugins[i].instance->getValueName(j) });
            }
        }
    }

    float sample(float s)
    {
        for (std::size_t i = 0; i < plugins.size(); i++) {
            s = plugins[i].instance->sample(s);
        }
        return s;
    }

public:
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
            out[i] = sample(in[i]);
        }
    }

    void loadPlugin(const char* path)
    {
        AudioPluginProps pluginProps = AudioPluginProps(debug);

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

        plugin.instance = ((AudioPlugin * (*)(AudioPluginProps & props)) allocator)(pluginProps);
        APP_PRINT("audio plugin loaded\n");
        APP_PRINT("plugin: %s\n", plugin.instance->name());

        // plugin.instance->set(0, 0.1f);
        // printf("---> getParamKey: %d\n", plugin.instance->getParamKey("volume"));

        plugins.push_back(plugin);
        setValueIndices(plugin);
    }

    bool assignMidiMapping(char* key, char* value)
    {
        if (plugins.size() > 0) {
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
            if (plugins.back().instance->assignMidiMapping(key, size, valuePosition, msg0Int, msg1Int)) {
                APP_INFO("[%s] Midi mapping assigned: %s\n", plugins.back().instance->name(), key);
                return true;
            }
        }
        return false;
    }

    bool midi(std::vector<unsigned char>* message)
    {
        for (std::size_t i = 0; i < plugins.size(); i++) {
            if (plugins[i].instance->midi(message)) {
                return true;
            }
        }
        return false;
    }

    float getValue(int moduleIndex, int valueIndex)
    {
        return plugins[moduleIndex].instance->getValue(valueIndex);
    }

    float getValue(int index)
    {
        return getValue(valueIndices[index].pluginIndex, valueIndices[index].valueIndex);
    }

    int getValueIndex(const char* moduleName, const char* valueName)
    {
        for (std::size_t i = 0; i < valueIndices.size(); i++) {
            if (strcmp(valueIndices[i].pluginName, moduleName) == 0 && strcmp(valueIndices[i].valueName, valueName) == 0) {
                return i;
            }
        }
        return -1;
    }

    void setValue(int index, float value)
    {
        plugins[index].instance->setValue(valueIndices[index].valueIndex, value);
    }
};

AudioHandler* AudioHandler::instance = NULL;

#endif