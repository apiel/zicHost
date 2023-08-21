# for FTP EOF
.ONESHELL:

# default
CC=g++ -o zicEffect

# used for granular synth
LIBSND=`pkg-config --cflags --libs sndfile`

# used for OSC
LIBLO=`pkg-config --cflags --libs liblo`

all:
	make clean_so effectGainVolume effectSampleRateReducer effectFilter effectDistortion effectDelay effectFilter effectFilterMultiMode effectFilterMultiMode2 effectFilterMultiModeMoog clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o audioPlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o

effectGainVolume:
	make compile LIBNAME=EffectGainVolume

effectSampleRateReducer:
	make compile LIBNAME=EffectSampleRateReducer

effectFilter:
	make compile LIBNAME=EffectFilter

effectDistortion:
	make compile LIBNAME=EffectDistortion

effectDelay:
	make compile LIBNAME=EffectDelay

effectFilter:
	make compile LIBNAME=EffectFilter

effectFilterMultiMode:
	make compile LIBNAME=EffectFilterMultiMode

effectFilterMultiMode2:
	make compile LIBNAME=EffectFilterMultiMode2
effectFilterMultiModeMoog:
	make compile LIBNAME=EffectFilterMultiModeMoog
