# TODO

- TODO method function pointer

- TODO delay save last state of the delay config... load the last state on start
       base on this state, we can create config files...

- TODO input/output should have multiple possible values, when in[0] and out[0] are default ones, and all the others are pointer to empty audio buffer or just simple sample float...
       Is it even the job of the plugin to decide which sample goes where
       Or should it be on the routing of the plugin that decide whih plugin is apply to what....?
- TODO make plugin plugins name configurable
       default: AUDIO_PLUGIN=./plugins/build/libzic_EffectSampleRateReducer.so
       custom: AUDIO_PLUGIN=./plugins/build/libzic_EffectSampleRateReducer.so SampleReducer
       > it will only make sense when there is multiples voices/tracks...

- TODO reverb, delay
     - load/save different kind of delay and reverb from a config file
     - add lfo on time ratio

- FIXME audio plugin ALSA, output not working
    - make audio plugin following the same format so they might be compatible...

- TODO jack audio plugin

- TODO random sample file selector?

- TODO generate config file example

- TODO handle note on/off from midi keyboard

- TODO handle rotary encoder from midi... (keyboard and osc)

- TODO ? use lv2 plugin
       https://github.com/lv2/lilv/blob/master/tools/lv2apply.c
       https://github.com/jeremysalwen/lv2file

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
