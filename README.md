## Installation

```sh
sudo apt install librtaudio-dev librtmidi-dev libsndfile1-dev
```

## Hint

Get list of audio card:

```sh
cat /proc/asound/cards
```

or 

```sh
arecord -l
```

or

```sh
aplay -l
```

## Build lib

g++ -c -o effectGainVolume.o effectGainVolume.cpp -fPIC
g++ -shared -o libzic_effectGainVolume.so effectGainVolume.o

https://iq.opengenus.org/create-shared-library-in-cpp/
https://theo-penavaire.medium.com/loading-of-a-c-class-from-a-shared-library-modern-c-722d6a830a2b
https://domiyanyue.medium.com/c-development-tutorial-4-static-and-dynamic-libraries-7b537656163e

dlopen

