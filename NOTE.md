# TODO

- TODO implement pusle audio api

- TODO delay save last state of the delay config... load the last state on start
       base on this state, we can create config files...

- TODO dont make delay dependent of sample rate...
- TODO handle rotatry encoder from midi... (keyboard and osc)

- TODO input/output should have multiple possible values, when in[0] and out[0] are default ones, and all the others are pointer to empty audio buffer or just simple sample float...
       Is it even the job of the plugin to decide which sample goes where
       Or should it be on the routing of the plugin that decide whih plugin is apply to what....?
- TODO make plugin plugins name configurable
       default: AUDIO_PLUGIN=./plugins/build/libzic_EffectSampleRateReducer.so
       custom: AUDIO_PLUGIN=./plugins/build/libzic_EffectSampleRateReducer.so SampleReducer
       > it will only make sense when there is multiples voices/tracks...

- TODO give a way to retrieve all midi mapping keys
- TODO save state
       - per plugin
       - global
- TODO generate config file sample

- TODO implement input for alsa and pulse audio api
- TODO reverb, delay
     - load/save different kind of delay and reverb from a config file
     - add lfo on time ratio
- TODO Use the granular synth and combine it with random sample player features...

       Need a synth to randomly pick up a sample (giving possibilities to pick next random sample (keep order so we can come back to previous one) or let automatically the sequencer pickup and play a sample randomly every random sec) where we can then apply some granular effect on top of it...
- TODO ? use lv2 plugin
       https://github.com/lv2/lilv/blob/master/tools/lv2apply.c
       https://github.com/jeremysalwen/lv2file

# IDEA

- multiple input and output stream, instead to have only one input and one output, we could pass array of input and array of output

# method function pointer

See if I can use

```cpp
#include <functional>
#include <vector>

class MyClass1{
public:
    void hello(float value)
    {
        printf("hello %f\n", value);
    }
};

class MyClass2 {
public:
    void world(float value)
    {
        printf("world %f\n", value);
    }
};

using Callback_t = std::function<void()>;

template <typename T>
Callback_t MakeCallback(void (T::*f)(float), float val)
{
    T* p = new T;
    return [=]() { (p->*f)(val); };
}

int main()
{
    std::vector<Callback_t> vetcor = {
        MakeCallback(&MyClass1::hello, 0.123),
        MakeCallback(&MyClass2::world, 0.456)
    };

    for (std::size_t i = 0; i < vetcor.size(); i++) {
        vetcor[i]();
    }
    return 0;
}
```
