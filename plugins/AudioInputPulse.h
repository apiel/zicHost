#ifndef _AUDIO_INPUT_PULSE_H_
#define _AUDIO_INPUT_PULSE_H_

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#include "audioPlugin.h"
#include "mapping.h"

static void pa_context_state_callback(pa_context* context, void* userdata);

class AudioInputPulse : public AudioPlugin {
protected:
    static const uint32_t audioChunk = 32;
    const uint32_t bufferReadSize = (sizeof(float) / sizeof(uint8_t)) * audioChunk;

    float buffer[audioChunk];
    pa_simple* device = NULL;

    uint32_t bufferIndex = 0;

    AudioPlugin::Props& props;

    void open()
    {
        debug("AudioInputPulse::open\n");

        if (device) {
            pa_simple_free(device);
            device = NULL;
        }

        static const pa_sample_spec streamFormat = {
            .format = PA_SAMPLE_FLOAT32LE,
            .rate = (uint32_t)props.sampleRate,
            .channels = props.channels,
        };

        pa_buffer_attr bufferAttr;
        bufferAttr.fragsize = audioChunk * sizeof(float);
        bufferAttr.maxlength = -1;

        device = pa_simple_new(NULL, NULL, PA_STREAM_RECORD, deviceName, "zicAudioInputPulse", &streamFormat, NULL, &bufferAttr, NULL);

        if (!device) {
            debug("ERROR: pa_simple_new() failed.\n");
            return;
        }
    }

    void search()
    {
        pa_mainloop* ml = NULL;
        pa_context* context = NULL;
        char* server = NULL;

        if (!(ml = pa_mainloop_new())) {
            debug("pa_mainloop_new() failed.\n");
            return freeListDevice(ml, context, server);
        }

        paMainLoopApi = pa_mainloop_get_api(ml);

        if (!(context = pa_context_new_with_proplist(paMainLoopApi, NULL, NULL))) {
            debug("pa_context_new() failed.\n");
            return freeListDevice(ml, context, server);
        }

        pa_context_set_state_callback(context, pa_context_state_callback, this);

        if (pa_context_connect(context, server, PA_CONTEXT_NOFLAGS, NULL) < 0) {
            debug("pa_context_connect() failed: %s\n", pa_strerror(pa_context_errno(context)));
            return freeListDevice(ml, context, server);
        }

        int ret = 1;
        if (pa_mainloop_run(ml, &ret) < 0) {
            debug("pa_mainloop_run() failed.\n");
        }
        freeListDevice(ml, context, server);
    }

    void freeListDevice(pa_mainloop* ml, pa_context* context, char* server = NULL)
    {
        if (context) {
            pa_context_unref(context);
        }

        if (ml) {
            pa_mainloop_free(ml);
        }

        pa_xfree(server);
    }

public:
    const char* deviceName = NULL;
    pa_mainloop_api* paMainLoopApi;

    AudioInputPulse(AudioPlugin::Props& props)
        : AudioPlugin(props)
        , props(props)
    {
        for (uint32_t i = 0; i < audioChunk; i++) {
            buffer[i] = 0.0f;
        }
        open();
    }

    bool config(char* key, char* value) override
    {
        if (strcmp(key, "DEVICE") == 0) {
            debug("Load input device: %s\n", value);
            deviceName = value;
            search();
            open();
            return true;
        }
        return false;
    }

    float sample(float in)
    {
        if (bufferIndex >= audioChunk) {
            bufferIndex = 0;
            if (device) {
                pa_simple_read(device, buffer, bufferReadSize, NULL);
            }
        }
        return buffer[bufferIndex++];
    }

    const char* name()
    {
        return "AudioInputPulse";
    }
};

static void pa_set_source_info(pa_context* c, const pa_source_info* i, int eol, void* userdata)
{
    AudioInputPulse* api = (AudioInputPulse*)userdata;

    if (eol) {
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
        return;
    }

    const char* description = pa_proplist_gets(i->proplist, "device.description");
    api->debug("- %s [DEVICE=%s] or [DEVICE=%s]\n", description, description, i->name);

    if (strcmp(description, api->deviceName) == 0) {
        AudioInputPulse* api = (AudioInputPulse*)userdata;
        api->deviceName = i->name;
    }
}

static void pa_context_state_callback(pa_context* context, void* userdata)
{
    AudioInputPulse* api = (AudioInputPulse*)userdata;

    switch (pa_context_get_state(context)) {
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
        break;

    case PA_CONTEXT_READY:
        pa_context_get_source_info_list(context, pa_set_source_info, userdata);
        break;

    case PA_CONTEXT_TERMINATED:
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
        break;

    case PA_CONTEXT_FAILED:
    default:
        api->debug("PA_CONTEXT_FAILED\n");
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
    }
}

#endif
