// #ifndef _MAIN_H_
// #define _MAIN_H_

// #include "audioApi.h"

// #define AUDIO_API_PULSE 1
// #define AUDIO_API_ALSA 2

// #ifndef AUDIO_API
// // #define AUDIO_API AUDIO_API_ALSA
// #endif

// #if AUDIO_API == AUDIO_API_PULSE
// #include "audioApi_pulse.h"
// AudioApi& audioApi = AudioApiPulse::get();
// #elif AUDIO_API == AUDIO_API_ALSA
// #include "audioApi_alsa.h"
// AudioApi& audioApi = AudioApiAlsa::get();
// #else
// #include "audioApi_rt.h"
// AudioApi& audioApi = AudioApiRT::get();
// #endif

// #include "config.h"
// #include "def.h"

// #include "pluginState.h"


// #endif