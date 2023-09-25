#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include <math.h>
#include <stdio.h> // file
#include <sys/stat.h> // file exist
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
const uint8_t MAX_STEPS = 32;

class Step {
public:
    bool enabled = false;
    float velocity = 0;
    uint8_t condition = 0;
    uint8_t len = 1; // len 0 is infinite?
    uint8_t counter = 0;
    uint8_t note = 60;

    // step could also have random pitch

    Step& reset()
    {
        enabled = false;
        velocity = 0;
        condition = 0;
        len = 1;
        counter = 0;
        note = 60;
        return *this;
    }

    Step& toggle()
    {
        enabled = !enabled;
        return *this;
    }

    Step& setNote(float _note)
    {
        note = range(_note, 0, 127);
        return *this;
    }

    Step& setLen(uint8_t _len)
    {
        len = range(_len, 0, MAX_STEPS);
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
    const char* folder = "../zicHost/patterns/";
    char patternFilename[255];
    Step steps[MAX_STEPS];

    uint8_t clockCounter = 0;
    uint8_t stepCounter = 0;
    uint8_t loopCounter = 0;

    bool active = false;

    AudioPlugin& targetPlugin;

    void onStep()
    {
        stepCounter++;
        if (stepCounter >= MAX_STEPS) {
            stepCounter = 0;
            loopCounter++;
        }
        for (int i = 0; i < MAX_STEPS; i++) {
            Step* step = &steps[i];
            if (step->counter) {
                step->counter--;
                if (step->counter == 0) {
                    targetPlugin.noteOff(step->note, 0);
                }
            }
        }
        if (active) {
            Step* step = &steps[stepCounter];
            if (step->enabled && step->conditionMet(loopCounter)) {
                step->counter = step->len;
                targetPlugin.noteOn(step->note, step->velocity);
            }
        }
    }

    bool fileExists(const char* filename)
    {
        struct stat buffer;
        return (stat(filename, &buffer) == 0);
    }

public:
    Val<Sequencer> detune = { this, 1.0f, "DETUNE", &Sequencer::setDetune, { "Detune", 48, VALUE_CENTERED_ONE_SIDED, .stepStart = -24 } };
    Val<Sequencer> pattern = { this, 0.0f, "PATTERN", &Sequencer::setPattern, { "Pattern" } };
    Val<Sequencer> selectedStep = { this, 0.0f, "SELECTED_STEP", &Sequencer::setSelectedStep, { "Step", MAX_STEPS - 1, .stepStart = 1 } };

    Sequencer(AudioPlugin::Props& props)
        : Mapping(props, { &detune, &pattern })
        , targetPlugin(props.audioPluginHandler->getPlugin("Granular"))
    {
        steps[0].setVelocity(1.0).setLen(8).enabled = true;
        steps[16].setVelocity(1.0).setNote(52).setLen(16).enabled = true;
        // save();
        // load can be done using setPattern
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

    Sequencer& setPattern(float value)
    {
        pattern.set(value);
        sprintf(patternFilename, "%s/%d.bin", folder, (uint)(pattern.get() * pattern.props().stepCount));
        if (fileExists(patternFilename)) {
            FILE* file = fopen(patternFilename, "rb");
            fread(steps, sizeof(Step), MAX_STEPS, file);
            fclose(file);
        } else {
            for (int i = 0; i < MAX_STEPS; i++) {
                steps[i].reset();
            }
        }
        return *this;
    }

    Sequencer& setSelectedStep(float value)
    {
        selectedStep.set(value);
        return *this;
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

    void save()
    {
        FILE* file = fopen(patternFilename, "wb");
        fwrite(steps, sizeof(Step), MAX_STEPS, file);
        fclose(file);
    }
};

#endif
