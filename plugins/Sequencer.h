#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include <math.h>
#include <time.h>

#include "audioPlugin.h"
#include "mapping.h"

int randCounter = 0;
int getRand()
{
    srand(time(NULL) + randCounter++);
    return rand();
}

struct StepCondition {
    const char* name;
    bool (*conditionMet)(uint8_t loopCounter);
} stepConditions[] = {
    { "---", [](uint8_t loopCounter) { return true; } },
    { "Pair", [](uint8_t loopCounter) { return loopCounter % 2 == 0; } },
    { "4th", [](uint8_t loopCounter) { return loopCounter % 4 == 0; } },
    { "6th", [](uint8_t loopCounter) { return loopCounter % 6 == 0; } },
    { "8th", [](uint8_t loopCounter) { return loopCounter % 8 == 0; } },
    { "Impair", [](uint8_t loopCounter) { return loopCounter % 2 == 1; } },
    { "1%", [](uint8_t loopCounter) { return (getRand() % 100) == 0; } },
    { "2%", [](uint8_t loopCounter) { return (getRand() % 100) < 2; } },
    { "5%", [](uint8_t loopCounter) { return (getRand() % 100) < 5; } },
    { "10%", [](uint8_t loopCounter) { return (getRand() % 100) < 10; } },
    { "20%", [](uint8_t loopCounter) { return (getRand() % 100) < 20; } },
    { "30%", [](uint8_t loopCounter) { return (getRand() % 100) < 30; } },
    { "40%", [](uint8_t loopCounter) { return (getRand() % 100) < 40; } },
    { "50%", [](uint8_t loopCounter) { return (getRand() % 100) < 50; } },
    { "60%", [](uint8_t loopCounter) { return (getRand() % 100) < 60; } },
    { "70%", [](uint8_t loopCounter) { return (getRand() % 100) < 70; } },
    { "80%", [](uint8_t loopCounter) { return (getRand() % 100) < 80; } },
    { "90%", [](uint8_t loopCounter) { return (getRand() % 100) < 90; } },
    { "95%", [](uint8_t loopCounter) { return (getRand() % 100) < 95; } },
    { "98%", [](uint8_t loopCounter) { return (getRand() % 100) < 98; } },
    { "99%", [](uint8_t loopCounter) { return (getRand() % 100) < 99; } },
};

uint8_t STEP_CONDITIONS_COUNT = sizeof(stepConditions) / sizeof(stepConditions[0]);

class Step {
public:
    bool enabled = false;
    float velocity = 0;
    uint8_t condition = 0;

    Step& toggle()
    {
        enabled = !enabled;
        return *this;
    }

    Step& setVelocity(float _velocity)
    {
        velocity = range(_velocity, 0.0f, 1.0f);
        return *this;
    }

    Step& setCondition(int8_t _condition)
    {
        condition = range(_condition, 0, STEP_CONDITIONS_COUNT - 1);
        return *this;
    }

    bool conditionMet(uint8_t loopCounter)
    {
        return stepConditions[condition].conditionMet(loopCounter);
    }
};

class Sequencer : public Mapping<Sequencer> {
protected:
    const static uint8_t MAX_STEPS = 32;

    Step steps[MAX_STEPS];
    Step* activeStep = &steps[0];

    uint8_t clockCounter = 0;
    uint8_t stepCounter = 0;
    uint8_t loopCounter = 0;

    bool active = false;

    void onStep()
    {
        stepCounter++;
        if (stepCounter >= MAX_STEPS) {
            stepCounter = 0;
            loopCounter++;
        }
        if (activeStep) {
            // noteOff
        }
        if (active) {
            Step* step = &steps[stepCounter];
            if (step->enabled && step->conditionMet(loopCounter)) {
                // note on
                activeStep = step;
                // debug("Step %d on\n", stepCounter);
            }
        }
    }

public:
    Val<Sequencer> detune = { this, 1.0f, "DETUNE", &Sequencer::setDetune, { "Detune", 48, VALUE_CENTERED_ONE_SIDED, .stepStart = -24 } };

    Sequencer(AudioPlugin::Props& props)
        : Mapping(props, { &detune })
    {
        steps[0].setVelocity(1.0).enabled = true;
        steps[8].setVelocity(1.0).enabled = true;
        steps[16].setVelocity(1.0).enabled = true;
        steps[24].setVelocity(1.0).enabled = true;
    }

    void onClockTick()
    {
        clockCounter++;
        // Clock events are sent at a rate of 24 pulses per quarter note
        // (24/4 = 6)
        if (clockCounter >= 6) {
            clockCounter = 0;
            onStep();
        }
    }

    void onStart()
    {
        clockCounter = 0;
        stepCounter = 0;
        loopCounter = 0;
        active = true;
    }

    void onStop()
    {
        active = false;
    }

    float sample(float in)
    {
        return in;
    }

    Sequencer& setDetune(float value)
    {
        detune.set(value);
        return *this;
    }

    const char* name()
    {
        return "Sequencer";
    }
};

#endif
