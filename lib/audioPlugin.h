#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

#include "midiMapping.h"

class AudioPluginProps {
public:
    int (*debug)(const char* format, ...);

    AudioPluginProps(int (*debug)(const char* format, ...))
        : debug(debug)
    {
    }
};

class AudioPlugin {
public:
    AudioPlugin(AudioPluginProps& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;
    virtual bool midi(std::vector<unsigned char>* message) = 0;
    virtual bool assignMidiMapping(const char* key, uint8_t size, uint8_t valuePosition, uint8_t msg0, uint8_t msg1) = 0;

    int (*debug)(const char* format, ...);
};

#endif