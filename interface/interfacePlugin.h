#ifndef _INTERFACE_PLUGIN_H_
#define _INTERFACE_PLUGIN_H_

#include <stdint.h>
#include <string.h>
#include <vector>

class InterfacePluginProps {
public:
    int (*debug)(const char* format, ...);
    void (*midiHandler)(std::vector<unsigned char>* message);
    void (*quit)();

    InterfacePluginProps(
        int (*debug)(const char* format, ...),
        void (*midiHandler)(std::vector<unsigned char>* message),
        void (*quit)())
        : debug(debug)
        , midiHandler(midiHandler)
        , quit(quit)
    {
    }
};

class InterfacePlugin {
public:
    int (*debug)(const char* format, ...);
    void (*midiHandler)(std::vector<unsigned char>* message);
    void (*quit)();

    InterfacePlugin(InterfacePluginProps& props)
        : debug(props.debug)
        , midiHandler(props.midiHandler)
        , quit(props.quit)
    {
    }

    virtual bool config(const char* key, const char* value) = 0;
    virtual const char* name() = 0;
    virtual void start() = 0;
};

#endif