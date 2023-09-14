#ifndef _AUDIO_API_INTERFACE_H_
#define _AUDIO_API_INTERFACE_H_

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

#include "audioApi.h"
#include "audioHandler.h"
#include "def.h"

static void pa_context_state_callback(pa_context* context, void* userdata);
static void pa_set_sink_info(pa_context* c, const pa_sink_info* i, int eol, void* userdata);
static void pa_set_source_info(pa_context* c, const pa_source_info* i, int eol, void* userdata);

class AudioApiPulse : public AudioApi {
protected:
    AudioHandler& audioHandler = AudioHandler::get();
    static AudioApiPulse* instance;
    AudioApiPulse() { }

    pa_mainloop* ml = NULL;
    pa_context* context = NULL;
    char* server = NULL;

    void freeListDevice()
    {
        if (context)
            pa_context_unref(context);

        if (ml)
            pa_mainloop_free(ml);

        pa_xfree(server);
    }

    void listDevices()
    {
        if (!(ml = pa_mainloop_new())) {
            debug("pa_mainloop_new() failed.\n");
            return freeListDevice();
        }

        paMainLoopApi = pa_mainloop_get_api(ml);

        if (!(context = pa_context_new_with_proplist(paMainLoopApi, NULL, NULL))) {
            debug("pa_context_new() failed.\n");
            return freeListDevice();
        }

        pa_context_set_state_callback(context, pa_context_state_callback, this);

        if (pa_context_connect(context, server, PA_CONTEXT_NOFLAGS, NULL) < 0) {
            debug("pa_context_connect() failed: %s\n", pa_strerror(pa_context_errno(context)));
            return freeListDevice();
        }

        int ret = 1;
        if (pa_mainloop_run(ml, &ret) < 0) {
            debug("pa_mainloop_run() failed.\n");
        }
        freeListDevice();
    }

public:
    const char* foundInputName = NULL;
    const char* foundOutputName = NULL;
    pa_mainloop_api* paMainLoopApi;

    static AudioApiPulse& get()
    {
        if (!instance) {
            instance = new AudioApiPulse();
        }
        return *instance;
    }

    int open()
    {
        APP_PRINT("AudioApiPulse::open\n");

        static const pa_sample_spec streamFormat = {
            .format = PA_SAMPLE_FLOAT32LE,
            .rate = SAMPLE_RATE,
            .channels = APP_CHANNELS,
        };

        pa_simple* deviceOut = pa_simple_new(NULL, NULL, PA_STREAM_PLAYBACK, foundOutputName == NULL ? audioOutputName : foundOutputName, "zicGranular", &streamFormat, NULL, NULL, NULL);
        pa_simple* deviceIn = pa_simple_new(NULL, NULL, PA_STREAM_RECORD, foundInputName == NULL ? audioInputName : foundInputName, "zicGranular", &streamFormat, NULL, NULL, NULL);

        uint8_t bufferMultiplier = sizeof(float) / sizeof(uint8_t);

        while (isRunning) {
            float inputBuffer[APP_AUDIO_CHUNK];
            pa_simple_read(deviceIn, inputBuffer, APP_AUDIO_CHUNK * bufferMultiplier, NULL);

            float outputBuffer[APP_AUDIO_CHUNK];
            audioHandler.samples((float*)inputBuffer, (float*)outputBuffer, APP_AUDIO_CHUNK);

            pa_simple_write(deviceOut, outputBuffer, APP_AUDIO_CHUNK * bufferMultiplier, NULL);
        }

        pa_simple_drain(deviceOut, NULL);

        return 0;
    }

    void list()
    {
        listDevices();
    }
};

AudioApiPulse* AudioApiPulse::instance = NULL;

static void pa_set_sink_info(pa_context* c, const pa_sink_info* i, int eol, void* userdata)
{
    if (eol) {
        // AudioApiPulse* api = (AudioApiPulse*)userdata;
        // api->paMainLoopApi->quit(api->paMainLoopApi, 0);
        return;
    }

    const char* description = pa_proplist_gets(i->proplist, "device.description");
    // printf("name: %s (outputChannels %d preferredSampleRate %d)\n", pa_proplist_gets(i->proplist, "device.description"), i->sample_spec.channels, i->sample_spec.rate);
    APP_PRINT("- %s [AUDIO_OUTPUT=%s]\n", description, i->name);

    if (strcmp(description, audioOutputName) == 0) {
        AudioApiPulse* api = (AudioApiPulse*)userdata;
        api->foundOutputName = i->name;
    }
}

static void pa_set_source_info(pa_context* c, const pa_source_info* i, int eol, void* userdata)
{
    if (eol) {
        AudioApiPulse* api = (AudioApiPulse*)userdata;
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
        return;
    }

    const char* description = pa_proplist_gets(i->proplist, "device.description");
    // printf("name: %s (inputChannels %d preferredSampleRate %d)\n", pa_proplist_gets(i->proplist, "device.description"), i->sample_spec.channels, i->sample_spec.rate);
    APP_PRINT("- %s [AUDIO_INPUT=%s]\n", description, i->name);

    if (strcmp(description, audioInputName) == 0) {
        AudioApiPulse* api = (AudioApiPulse*)userdata;
        api->foundInputName = i->name;
    }
}

static void pa_context_state_callback(pa_context* context, void* userdata)
{
    AudioApiPulse* api = (AudioApiPulse*)userdata;

    switch (pa_context_get_state(context)) {
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
        break;

    case PA_CONTEXT_READY:
        debug("PA_CONTEXT_READY\n");
        pa_context_get_sink_info_list(context, pa_set_sink_info, userdata); // output info ... needs to be before input
        pa_context_get_source_info_list(context, pa_set_source_info, userdata); // input info
        break;

    case PA_CONTEXT_TERMINATED:
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
        break;

    case PA_CONTEXT_FAILED:
    default:
        debug("PA_CONTEXT_FAILED\n");
        api->paMainLoopApi->quit(api->paMainLoopApi, 0);
    }
}

#endif