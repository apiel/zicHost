#ifndef _ENCODER_INTERFACE_H_
#define _ENCODER_INTERFACE_H_

#ifdef PIGPIO
// sudo apt-get install libpigpio-dev
#include <pigpio.h>
#endif

#include <stdio.h>

#include "interfacePlugin.h"

#define ENCODER_COUNT 4

class RotaryEncoder {
protected:
    int levelA = 0;
    int levelB = 0;
    int lastGpio = -1;

    static void pulseEx(int gpio, int level, uint32_t tick, void* user)
    {
        RotaryEncoder* self = (RotaryEncoder*)user;
        self->pulse(gpio, level, tick);
    }

    void send(uint8_t direction)
    {
        std::vector<unsigned char> message;
        message.push_back(0xB0 + channel);
        message.push_back(cc);
        message.push_back(direction);
        midiHandler(&message);
    }

public:
    void (*midiHandler)(std::vector<unsigned char>* message) = NULL;
    int gpioA = -1;
    int gpioB = -1;
    uint8_t cc = 1;
    uint8_t channel = 1;

    void start()
    {
        if (gpioA != -1 && gpioB != -1 && midiHandler != NULL) {
            #ifdef PIGPIO
            gpioSetMode(gpioA, PI_INPUT);
            gpioSetMode(gpioB, PI_INPUT);
            gpioSetPullUpDown(gpioA, PI_PUD_UP);
            gpioSetPullUpDown(gpioB, PI_PUD_UP);
            gpioSetAlertFuncEx(gpioA, pulseEx, this);
            gpioSetAlertFuncEx(gpioB, pulseEx, this);
            #endif
        }
    }

    void pulse(int gpio, int level, uint32_t tick)
    {
        if (gpio == gpioA) {
            levelA = level;
        } else {
            levelB = level;
        }

        if (gpio != lastGpio) /* debounce */
        {
            lastGpio = gpio;

            if ((gpio == gpioA) && (level == 1)) {
                if (levelB) {
                    send(1); // 1
                }
            } else if ((gpio == gpioB) && (level == 1)) {
                if (levelA) {
                    send(127); // -1
                }
            }
        }
    }
};

class EncoderInterface : public InterfacePlugin {
protected:
    RotaryEncoder encoders[ENCODER_COUNT];
    uint8_t encodersActive = 0;

public:
    EncoderInterface(InterfacePluginProps& props)
        : InterfacePlugin(props)
    {
    }

    bool config(const char* key, const char* value)
    {
        if (strcmp(key, "ENCODER") == 0) {
            if (encodersActive >= ENCODER_COUNT) {
                return false;
            }
            // ENCODER=<gpioA> <gpioB> <channel> <cc>
            sscanf(value, "%d %d %hhd %hhd",
                &encoders[encodersActive].gpioA,
                &encoders[encodersActive].gpioB,
                &encoders[encodersActive].channel,
                &encoders[encodersActive].cc);

            encoders[encodersActive].midiHandler = midiHandler;
            
            return true;
        }
        return false;
    }

    const char* name()
    {
        return "Rotary encoder interface";
    }

    void start()
    {
        for (int i = 0; i < encodersActive; i++) {
            encoders[i].start();
        }
    };
};

#endif
