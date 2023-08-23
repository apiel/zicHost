#ifndef _INTERFACE_PLUGIN_H_
#define _INTERFACE_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

class InterfacePluginProps {
public:
    int (*debug)(const char* format, ...);
    void (*midiHandler)(std::vector<unsigned char>* message);

    InterfacePluginProps(
        int (*debug)(const char* format, ...),
        void (*midiHandler)(std::vector<unsigned char>* message))
        : debug(debug)
        , midiHandler(midiHandler)
    {
    }
};

class InterfacePlugin {
public:
    int (*debug)(const char* format, ...);
    void (*midiHandler)(std::vector<unsigned char>* message);

    InterfacePlugin(InterfacePluginProps& props)
        : debug(props.debug)
        , midiHandler(props.midiHandler)
    {
    }

    virtual bool config(const char* key, const char* value) = 0;
    virtual const char* name() = 0;
    virtual void start() = 0;
};

#endif