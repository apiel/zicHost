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

#include "pluginState.h"

extern "C" {
int start()
{
    APP_PRINT("List of available audio interfaces:\n");
    audioApi.list();

    // TODO make config.cfg a parameter
    // make default config
    if (loadConfig() == false) {
        return 1;
    }

    startPluginInterface();

    return audioApi.open();
}
}

int main()
{
    return start();
}