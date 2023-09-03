#include "audioApi.h"

#define AUDIO_API_PULSE 1
#define AUDIO_API_ALSA 2

#ifndef AUDIO_API
// #define AUDIO_API AUDIO_API_ALSA
#endif

#if AUDIO_API == AUDIO_API_PULSE
#include "audioApi_pulse.h"
AudioApi& audioApi = AudioApiPulse::get();
#elif AUDIO_API == AUDIO_API_ALSA
#include "audioApi_alsa.h"
AudioApi& audioApi = AudioApiAlsa::get();
#else
#include "audioApi_rt.h"
AudioApi& audioApi = AudioApiRT::get();
#endif

#include "config.h"
#include "def.h"

extern "C" {
bool init()
{
    audioApi.init();
    APP_PRINT("List of available audio interfaces:\n");
    audioApi.list();

    // TODO make config.cfg a parameter
    // make default config
    return loadConfig();
}

int mainLoop()
{
    return audioApi.open();
}

void midi(std::vector<unsigned char>* message)
{
    midiHandler(message);
}

float getValue(int index)
{
    return audioHandler.getValue(index);
}

int getValueIndex(const char* moduleName, const char* valueName)
{
    return audioHandler.getValueIndex(moduleName, valueName);
}

}

int main()
{
    if (!init()) {
        return 1;
    }
    return mainLoop();
}