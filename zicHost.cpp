#include "audioHandler.h"
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
    return &AudioHandler::get().plugins;
}

int mainLoop()
{
    AudioHandler::get().loop();
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