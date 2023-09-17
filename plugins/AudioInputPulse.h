#ifndef _AUDIO_INPUT_PULSE_H_
#define _AUDIO_INPUT_PULSE_H_

#include "AudioPulse.h"

class AudioInputPulse : public AudioPulse {
protected:
    pa_simple* newDevice(const pa_sample_spec streamFormat)
    {
        pa_buffer_attr bufferAttr;
        bufferAttr.fragsize = audioChunk * sizeof(float);
        bufferAttr.maxlength = -1;

        return pa_simple_new(NULL, NULL, PA_STREAM_RECORD, deviceName, "zicAudioInputPulse", &streamFormat, NULL, &bufferAttr, NULL);
    }

public:
    AudioInputPulse(AudioPlugin::Props& props)
        : AudioPulse(props)
    {
        open();
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

    bool isSink() override
    {
        return false;
    }
};

#endif
