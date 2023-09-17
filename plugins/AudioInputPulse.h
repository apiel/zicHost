#ifndef _AUDIO_INPUT_PULSE_H_
#define _AUDIO_INPUT_PULSE_H_

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#include "audioPlugin.h"
#include "mapping.h"

class AudioInputPulse : public AudioPlugin {
protected:
    static const uint32_t audioChunk = 32;
    const uint32_t bufferReadSize = (sizeof(float) / sizeof(uint8_t)) * audioChunk;

    float buffer[audioChunk];
    pa_simple* device = NULL;

    uint32_t bufferIndex = 0;

    AudioPlugin::Props& props;

public:
    const char* deviceName = NULL;
    const char* foundDeviceName = NULL;
    pa_mainloop_api* paMainLoopApi;

    AudioInputPulse(AudioPlugin::Props& props)
        : AudioPlugin(props)
        , props(props)
    {
        debug("AudioInputPulse::open\n");

        static const pa_sample_spec streamFormat = {
            .format = PA_SAMPLE_FLOAT32LE,
            .rate = (uint32_t)props.sampleRate,
            .channels = props.channels,
        };

        pa_buffer_attr bufferAttr;
        bufferAttr.fragsize = audioChunk * sizeof(float);
        bufferAttr.maxlength = -1;

        device = pa_simple_new(NULL, NULL, PA_STREAM_RECORD, "alsa_input.usb-Plantronics_Plantronics_P610_ED80689444A4FD42960C3DD9B24FFD2B-00.analog-stereo",
            "zicAudioInputPulse", &streamFormat, NULL, &bufferAttr, NULL);
    }

    float sample(float in)
    {
        if (bufferIndex >= audioChunk) {
            bufferIndex = 0;
            pa_simple_read(device, buffer, bufferReadSize, NULL);
        }
        return buffer[bufferIndex++];
    }

    const char* name()
    {
        return "AudioInputPulse";
    }
};

// static void pa_set_source_info(pa_context* c, const pa_source_info* i, int eol, void* userdata)
// {
//     AudioInputPulse* api = (AudioInputPulse*)userdata;

//     if (eol) {
//         api->paMainLoopApi->quit(api->paMainLoopApi, 0);
//         return;
//     }

//     const char* description = pa_proplist_gets(i->proplist, "device.description");
//     api->debug("- %s [AUDIO_INPUT=%s]\n", description, i->name);

//     if (strcmp(description, audioInputName) == 0) {
//         AudioInputPulse* api = (AudioInputPulse*)userdata;
//         api->foundInputName = i->name;
//     }
// }

// static void pa_context_state_callback(pa_context* context, void* userdata)
// {
//     AudioInputPulse* api = (AudioInputPulse*)userdata;

//     switch (pa_context_get_state(context)) {
//     case PA_CONTEXT_CONNECTING:
//     case PA_CONTEXT_AUTHORIZING:
//     case PA_CONTEXT_SETTING_NAME:
//         break;

//     case PA_CONTEXT_READY:
//         debug("PA_CONTEXT_READY\n");
//         pa_context_get_sink_info_list(context, pa_set_sink_info, userdata); // output info ... needs to be before input
//         pa_context_get_source_info_list(context, pa_set_source_info, userdata); // input info
//         break;

//     case PA_CONTEXT_TERMINATED:
//         api->paMainLoopApi->quit(api->paMainLoopApi, 0);
//         break;

//     case PA_CONTEXT_FAILED:
//     default:
//         debug("PA_CONTEXT_FAILED\n");
//         api->paMainLoopApi->quit(api->paMainLoopApi, 0);
//     }
// }

#endif
