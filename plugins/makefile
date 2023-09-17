# used for granular synth
LIBSND=`pkg-config --cflags --libs sndfile`

all:
	make clean_so EffectGainVolume EffectSampleRateReducer EffectFilter EffectDistortion EffectDelay\
		EffectFilter EffectFilterMultiMode EffectFilterMultiMode2 EffectFilterMultiModeMoog SynthGranular\
		AudioInputPulse\
		clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o audioPlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o $(EXTRA)

EffectGainVolume:
	make compile LIBNAME=EffectGainVolume

EffectSampleRateReducer:
	make compile LIBNAME=EffectSampleRateReducer

EffectDistortion:
	make compile LIBNAME=EffectDistortion

EffectDelay:
	make compile LIBNAME=EffectDelay

EffectFilter:
	make compile LIBNAME=EffectFilter

EffectFilterMultiMode:
	make compile LIBNAME=EffectFilterMultiMode

EffectFilterMultiMode2:
	make compile LIBNAME=EffectFilterMultiMode2

EffectFilterMultiModeMoog:
	make compile LIBNAME=EffectFilterMultiModeMoog

SynthGranular:
	make compile LIBNAME=SynthGranular

AudioInputPulse:
	make compile LIBNAME=AudioInputPulse EXTRA="$(shell pkg-config --cflags --libs libpulse-simple)"
