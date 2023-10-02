#ifndef _AUDIO_OUTPUT_PULSE_H_
#define _AUDIO_OUTPUT_PULSE_H_

#include "AudioPulse.h"

class AudioOutputPulse : public AudioPulse {
protected:
    pa_simple* newDevice(const pa_sample_spec streamFormat)
    {
        return pa_simple_new(NULL, NULL, PA_STREAM_PLAYBACK, deviceName, "zicAudioOutputPulse", &streamFormat, NULL, NULL, NULL);
    }

public:
    AudioOutputPulse(AudioPlugin::Props& props, char* _name)
        : AudioPulse(props, _name)
    {
        open();
    }

    float sample(float in)
    {
        if (bufferIndex >= audioChunk) {
            bufferIndex = 0;
            if (device) {
                pa_simple_write(device, buffer, bufferReadSize, NULL);
            }
        }
        buffer[bufferIndex++] = in;
        return in;
    }

    void sample(float* buf)
    {
        buf[track] = sample(buf[track]);
    }
};

#endif
