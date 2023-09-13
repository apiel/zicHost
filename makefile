RTMIDI=`pkg-config --cflags --libs rtmidi`

# AUDIO_API by default will use RT audio
# else uncomment one of the following to use the other audio API
RTAUDIO=`pkg-config --cflags --libs rtaudio`
# PULSEAUDIO=`pkg-config --cflags --libs libpulse-simple` -DAUDIO_API=1
# ALSA=`pkg-config --cflags --libs alsa` -DAUDIO_API=2

BUILD=-Wno-narrowing -ldl $(RTAUDIO) $(LIBSND) $(RTMIDI) $(PULSEAUDIO) $(ALSA) $(LIBLO)

host: build run
all: libs build so run

ui:
	make -C interface lib_ui
	make build run

libs:
	@echo "\n------------------ plugins ------------------\n"
	make -C plugins
	@echo "\nbuild plugins done."

so:
	@echo "\n------------------ build_so ------------------\n"
	g++ -fPIC -shared -o zicHost.so zicHost.cpp -fopenmp $(BUILD)
	@echo "\nbuild zicHost.so done."

build:
	@echo "\n------------------ build ------------------\n"
	g++ -o zicHost -Wall zicHost.cpp -fopenmp $(BUILD)
	@echo "\nbuild zicHost done."

run:
	@echo "\n------------------ run ------------------\n"
	./zicHost

try_so:
	g++ -o try_so try_so.cpp
	./try_so
