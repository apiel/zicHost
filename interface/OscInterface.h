#ifndef _OSC_INTERFACE_H_
#define _OSC_INTERFACE_H_

#include <lo/lo.h>
#include <stdio.h>
#include <stdlib.h>

#include "interfacePlugin.h"

void error(int num, const char* msg, const char* path);
int midiOscHandler(const char* path, const char* types, lo_arg** argv, int argc, lo_message data, void* user_data);
int exitOscHandler(const char* path, const char* types, lo_arg** argv, int argc, lo_message data, void* user_data);

class OscInterface : public InterfacePlugin {
protected:
    char port[6] = "8000";

public:
    lo_server_thread serverThread;

    OscInterface(InterfacePluginProps& props)
    : InterfacePlugin(props)
    {}

    bool config(const char* key, const char* value)
    {
        if (strcmp(key, "PORT") == 0) {
            strcpy(port, value);    
            return true;
        }
        return false;
    }

    const char* name()
    {
        return "OSC server";
    }

    void start()
    {
        serverThread = lo_server_thread_new(port, error);

        debug("OSC server listening on port %s\n", port);

        lo_server_thread_add_method(serverThread, "/midi", NULL, midiOscHandler, this);
        lo_server_thread_add_method(serverThread, "/exit", "", exitOscHandler, this);

        lo_server_thread_start(serverThread);
    };
};


void error(int num, const char* msg, const char* path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}

int midiOscHandler(const char* path, const char* types, lo_arg** argv, int argc, lo_message data, void* user_data)
{
    OscInterface* oscInterface = (OscInterface*)user_data;

    std::vector<unsigned char> msg;
    for (int i = 0; i < argc; i++) {
        msg.push_back(argv[i]->c);
    }
    oscInterface->midiHandler(&msg);

    return 0;
}

int exitOscHandler(const char* path, const char* types, lo_arg** argv, int argc, lo_message data, void* user_data)
{
    OscInterface* oscInterface = (OscInterface*)user_data;

    lo_server_thread_free(oscInterface->serverThread);
    exit(0);

    return 0;
}

#endif
