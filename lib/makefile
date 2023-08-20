# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicEffect

# used for granular synth
LIBSND=`pkg-config --cflags --libs sndfile`

# used for OSC
LIBLO=`pkg-config --cflags --libs liblo`

#g++ -c -o effectGainVolume.o effectGainVolume.cpp -fPIC
#g++ -shared -o libzic_effectGainVolume.so effectGainVolume.o

all:
	make effectGainVolume

clean:
	rm -f *.o *.so

effectGainVolume:
	g++ -c -o effectGainVolume.o audioPlugin.cpp -fPIC
	g++ -shared -o libzic_effectGainVolume.so effectGainVolume.o

