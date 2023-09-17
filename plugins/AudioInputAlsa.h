#ifndef _AUDIO_INPUT_ALSA_H_
#define _AUDIO_INPUT_ALSA_H_

#include "AudioAlsa.h"

class AudioInputAlsa : public AudioAlsa {
protected:
    snd_pcm_stream_t stream()
    {
        return SND_PCM_STREAM_CAPTURE;
    }

public:
    AudioInputAlsa(AudioPlugin::Props& props)
        : AudioAlsa(props)
    {
        open();
    }

    float sample(float in)
    {
        if (bufferIndex >= audioChunk) {
            bufferIndex = 0;
            if (handle && snd_pcm_readi(handle, buffer, audioChunk) < 0) {
                // int err;
                // debug("Audio card read error: %s\n", snd_strerror(err));
                // should throw and exit!!
                return 0.0f;
            }
        }
        return buffer[bufferIndex++] / 2147483647.0f;
    }

    const char* name()
    {
        return "AudioInputAlsa";
    }
};

#endif
