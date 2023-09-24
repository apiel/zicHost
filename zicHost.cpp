#include "audioPluginHandler.h"
#include "config.h"
#include "def.h"

extern "C" {
std::vector<Plugin>* init()
{
    // TODO make config.cfg a parameter
    // make default config
    if (!loadConfig()) {
        return NULL;
    }
    return &AudioPluginHandler::get().plugins;
}

int mainLoop()
{
    AudioPluginHandler::get().loop();
    return 0;
}

void midi(std::vector<unsigned char>* message)
{
    midiHandler(message);
}
}

int main()
{
    if (!init()) {
        return 1;
    }
    return mainLoop();
}