# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicEffect

# used for granular synth
LIBSND=`pkg-config --cflags --libs sndfile`

RTAUDIO=`pkg-config --cflags --libs rtaudio`

RTMIDI=`pkg-config --cflags --libs rtmidi`

LIBLO=`pkg-config --cflags --libs liblo`

# AUDIO_API by default will use RT audio
# else uncomment one of the following to use the other audio API
# PULSEAUDIO=`pkg-config --cflags --libs libpulse-simple` -DAUDIO_API=1
# ALSA=`pkg-config --cflags --libs alsa` -DAUDIO_API=2

BUILD=-Wall zicEffect.cpp -fopenmp -Wno-narrowing $(RTAUDIO) $(LIBSND) $(RTMIDI) $(PULSEAUDIO) $(ALSA) $(LIBLO)

linux: build run

build:
	$(CC) $(BUILD)

run:
	echo start zicEffect
	LD_LIBRARY_PATH=. ./zicEffect
