#ifndef _AUDIO_INPUT_ALSA_H_
#define _AUDIO_INPUT_ALSA_H_

#include "AudioAlsa.h"

class AudioOutputAlsa : public AudioAlsa {
protected:
    snd_pcm_stream_t stream()
    {
        return SND_PCM_STREAM_CAPTURE;
    }

public:
    AudioOutputAlsa(AudioPlugin::Props& props)
        : AudioAlsa(props)
    {
        open();
    }

    float sample(float in)
    {
        if (bufferIndex >= audioChunk) {
            bufferIndex = 0;
            if (handle) {
                snd_pcm_sframes_t count = snd_pcm_writei(handle, buffer, audioChunk);
                // debug("write %ld (%d)(%d)\n", count, buffer[0], audioChunk);
            }
        }
        buffer[bufferIndex++] = in * 2147483647.0f;
        return in;
    }

    const char* name()
    {
        return "AudioOutputAlsa";
    }
};

#endif
