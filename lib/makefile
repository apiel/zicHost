# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicEffect

# used for granular synth
LIBSND=`pkg-config --cflags --libs sndfile`

# used for OSC
LIBLO=`pkg-config --cflags --libs liblo`

all:
	make clean effectGainVolume effectSampleRateReducer

clean:
	rm -f *.o *.so

compile:
	g++ -c -o $(LIBNAME).o audioPlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o libzic_$(LIBNAME).so $(LIBNAME).o

effectGainVolume:
	make compile LIBNAME=EffectGainVolume

effectSampleRateReducer:
	make compile LIBNAME=EffectSampleRateReducer
