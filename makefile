# for FTP EOF
.ONESHELL:

RTAUDIO=`pkg-config --cflags --libs rtaudio`

RTMIDI=`pkg-config --cflags --libs rtmidi`

LIBLO=`pkg-config --cflags --libs liblo`

# AUDIO_API by default will use RT audio
# else uncomment one of the following to use the other audio API
# PULSEAUDIO=`pkg-config --cflags --libs libpulse-simple` -DAUDIO_API=1
# ALSA=`pkg-config --cflags --libs alsa` -DAUDIO_API=2

BUILD=

all: libs build run
host: build run

libs:
	@echo "\n------------------ lib ------------------\n"
	make -C lib
	@echo "\nbuild lib done."

build:
	@echo "\n------------------ build ------------------\n"
	g++ -o zicHost -Wall zicHost.cpp -fopenmp -Wno-narrowing $(RTAUDIO) $(LIBSND) $(RTMIDI) $(PULSEAUDIO) $(ALSA) $(LIBLO)
	@echo "build zicHost done."

run:
	@echo "\n------------------ run ------------------\n"
	./zicHost
