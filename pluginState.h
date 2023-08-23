#ifndef _PLUGIN_STATE_H
#define _PLUGIN_STATE_H

#include "def.h"
#include "interface/interfacePlugin.h"
#include "midi.h"

#ifndef INTERFACE_PLUGIN_COUNT
#define INTERFACE_PLUGIN_COUNT 10
#endif

struct StateInterfacePlugin {
    void* handle;
    InterfacePlugin* instance;
} interfacePlugins[INTERFACE_PLUGIN_COUNT];

uint16_t interfacePluginCount = 0;

void loadPluginInterface(const char* path)
{
    InterfacePluginProps pluginProps = InterfacePluginProps(debug, midiHandler);

    if (interfacePluginCount >= INTERFACE_PLUGIN_COUNT) {
        APP_PRINT("Cannot load plugin: %s, reached max interface plugin count %d\n", path, INTERFACE_PLUGIN_COUNT);
        return;
    }

    interfacePlugins[interfacePluginCount].handle = dlopen(path, RTLD_LAZY);

    if (!interfacePlugins[interfacePluginCount].handle) {
        APP_PRINT("Cannot open library: %s\n", dlerror());
        return;
    }

    dlerror();
    void* allocator = (InterfacePlugin*)dlsym(interfacePlugins[interfacePluginCount].handle, "allocator");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        APP_PRINT("Cannot load symbol: %s\n", dlsym_error);
        dlclose(interfacePlugins[interfacePluginCount].handle);
        return;
    }

    interfacePlugins[interfacePluginCount].instance = ((InterfacePlugin * (*)(InterfacePluginProps & props)) allocator)(pluginProps);
    APP_PRINT("interface plugin loaded\n");
    APP_PRINT("plugin: %s\n", interfacePlugins[interfacePluginCount].instance->name());

    interfacePluginCount++;
}

void startPluginInterface()
{
    for (uint16_t i = 0; i < interfacePluginCount; i++) {
        interfacePlugins[i].instance->start();
        APP_PRINT("Interface plugin started: %s\n", interfacePlugins[i].instance->name());
    }
}

bool configPluginStart(char* key, char* value)
{
    if (interfacePluginCount > 0) {
        if (interfacePlugins[interfacePluginCount - 1].instance->config(key, value)) {
            APP_INFO("[%s] Config started: %s\n", interfacePlugins[interfacePluginCount - 1].instance->name(), key);
            return true;
        }
    }
    return false;
}

#endif