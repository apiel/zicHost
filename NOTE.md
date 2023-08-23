# TODO

- TODO give a way to retrieve all midi mapping keys
- TODO save state
       - per plugin
       - global
- TODO plugin interface
       - UI
       - rotary ecnoder
       - how to share state between interface?
- TODO dont make delay dependent of sample rate...


- TODO implement input for alsa and pulse audio api
- TODO reverb, delay
     - load/save different kind of delay and reverb from a config file
     - add lfo on time ratio
- TODO Use the granular synth and combine it with random sample player features...

       Need a synth to randomly pick up a sample (giving possibilities to pick next random sample (keep order so we can come back to previous one) or let automatically the sequencer pickup and play a sample randomly every random sec) where we can then apply some granular effect on top of it...
- TODO ? use lv2 plugin
       https://github.com/lv2/lilv/blob/master/tools/lv2apply.c
       https://github.com/jeremysalwen/lv2file

# NOTE

pipe stream into app https://gist.github.com/ghedo/963382?permalink_comment_id=3509255

# IDEA

- multiple input and output stream, instead to have only one input and one output, we could pass array of input and array of output

# Build lib

g++ -c -o effectGainVolume.o effectGainVolume.cpp -fPIC
g++ -shared -o libzic_effectGainVolume.so effectGainVolume.o

https://iq.opengenus.org/create-shared-library-in-cpp/
https://theo-penavaire.medium.com/loading-of-a-c-class-from-a-shared-library-modern-c-722d6a830a2b
https://domiyanyue.medium.com/c-development-tutorial-4-static-and-dynamic-libraries-7b537656163e

dlopen