#ifndef _AUDIO_PLUGIN_H_
#define _AUDIO_PLUGIN_H_

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
    // TODO add midi mapping: cc, noteon, noteoff...
    // TODO add osc mapping
    // TODO add config, e.g.: gainOutput

    AudioPlugin(AudioPluginProps& props)
        : debug(props.debug)
    {
    }

    virtual float sample(float in) = 0;
    virtual const char* name() = 0;

    int (*debug)(const char* format, ...);
};

#endif