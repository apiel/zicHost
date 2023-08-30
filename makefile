RTMIDI=`pkg-config --cflags --libs rtmidi`

# AUDIO_API by default will use RT audio
# else uncomment one of the following to use the other audio API
# RTAUDIO=`pkg-config --cflags --libs rtaudio`
# PULSEAUDIO=`pkg-config --cflags --libs libpulse-simple` -DAUDIO_API=1
ALSA=`pkg-config --cflags --libs alsa` -DAUDIO_API=2

all: interfaces libs build run
host: build run

ui:
	make -C interface lib_ui
	make build run

interfaces:
	@echo "\n------------------ interface ------------------\n"
	make -C interface
	@echo "\nbuild interface done."

libs:
	@echo "\n------------------ lib ------------------\n"
	make -C lib
	@echo "\nbuild lib done."

build:
	@echo "\n------------------ build ------------------\n"
	g++ -o zicHost -Wall zicHost.cpp -fopenmp -Wno-narrowing -ldl $(RTAUDIO) $(LIBSND) $(RTMIDI) $(PULSEAUDIO) $(ALSA) $(LIBLO)
	@echo "build zicHost done."

run:
	@echo "\n------------------ run ------------------\n"
	./zicHost
