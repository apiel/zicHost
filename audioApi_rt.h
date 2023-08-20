#ifndef _AUDIO_API_INTERFACE_H_
#define _AUDIO_API_INTERFACE_H_

#include "audioApi.h"
#include "audioHandler.h"
#include "def.h"

AudioHandler& audioHandler = AudioHandler::get();

int audioCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus status, void* data)
{
    audioHandler.samples((float*)inputBuffer, (float*)outputBuffer, nBufferFrames * APP_CHANNELS);
    return 0;
}

class AudioApiRT : public AudioApi {
protected:
    RtAudio* audio = 0;

    static AudioApiRT* instance;
    AudioApiRT()
    {
        audio = new RtAudio();
    }

    const char* getApiName()
    {
        switch (audio->getCurrentApi()) {
        case RtAudio::UNSPECIFIED:
            return "UNSPECIFIED";
        case RtAudio::LINUX_ALSA:
            return "LINUX_ALSA";
        case RtAudio::LINUX_PULSE:
            return "LINUX_PULSE";
        case RtAudio::LINUX_OSS:
            return "LINUX_OSS";
        case RtAudio::UNIX_JACK:
            return "UNIX_JACK";
        case RtAudio::MACOSX_CORE:
            return "MACOSX_CORE";
        case RtAudio::WINDOWS_WASAPI:
            return "WINDOWS_WASAPI";
        case RtAudio::WINDOWS_ASIO:
            return "WINDOWS_ASIO";
        case RtAudio::WINDOWS_DS:
            return "WINDOWS_DS";
        case RtAudio::RTAUDIO_DUMMY:
            return "RTAUDIO_DUMMY";
        default:
            return "?";
        }
    }

    void showAudioDeviceInfo()
    {
        unsigned int deviceCount = audio->getDeviceCount();

        APP_PRINT("Found %d audio devices (%s):\n\n", deviceCount, getApiName());
        RtAudio::DeviceInfo rtInfo;
        for (unsigned int i = 0; i < deviceCount; i++) {
            rtInfo = audio->getDeviceInfo(i);
            if (rtInfo.probed == true) {
                APP_PRINT(" (%d) %s\n", i, rtInfo.name.c_str());
                if (rtInfo.outputChannels) {
                    APP_PRINT("   [AUDIO_OUTPUT=%s]\n", rtInfo.name.c_str());
                }
                if (rtInfo.inputChannels) {
                    APP_PRINT("   [AUDIO_INPUT=%s]\n", rtInfo.name.c_str());
                }
                APP_PRINT("\n");
            }
        }
    }

    unsigned int getAudioDeviceId(char* name)
    {
        unsigned int deviceCount = audio->getDeviceCount();

        RtAudio::DeviceInfo rtInfo;
        for (unsigned int i = 0; i < deviceCount; i++) {
            rtInfo = audio->getDeviceInfo(i);
            if (rtInfo.probed == true) {
                if (rtInfo.name.find(name) != std::string::npos) {
                    return i;
                }
            }
        }
        debug("/!\\ Warning /!\\ Audio device %s not found\n", name);
        rtInfo = audio->getDeviceInfo(0);
        debug("Using default %s device [%s]\n", getApiName(), rtInfo.name.c_str());
        return 0;
    }

public:
    static AudioApiRT& get()
    {
        if (!instance) {
            instance = new AudioApiRT();
        }
        return *instance;
    }

    int open()
    {
        APP_PRINT("RT audio::open\n");

        RtAudio::StreamParameters audioParams;
        RtAudio::StreamParameters audioInputParams;

        unsigned int bufferFrames = APP_AUDIO_CHUNK;
        audioParams.deviceId = getAudioDeviceId(audioOutputName);
        audioParams.nChannels = APP_CHANNELS;
        audioInputParams.deviceId = getAudioDeviceId(audioInputName);
        audioInputParams.nChannels = APP_CHANNELS;
        try {
            audio->openStream(&audioParams, &audioInputParams, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufferFrames, &audioCallback);
            audio->startStream();
            while (audio->isStreamRunning()) {
                usleep(100000); // 100ms
            }
        } catch (RtAudioError& e) {
            e.printMessage();
            return 1;
        }
        return 0;
    }

    void list()
    {
        showAudioDeviceInfo();
    }
};

AudioApiRT* AudioApiRT::instance = NULL;

#endif