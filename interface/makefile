all:
	make clean_so lib_osc lib_ui lib_encoder clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o interfacePlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o $(EXTRA)

lib_osc:
	make compile LIBNAME=OscInterface EXTRA=$(shell pkg-config --cflags --libs liblo)

lib_ui:
	make compile LIBNAME=UserInterface \
		EXTRA="$(shell sdl2-config --cflags --libs) $(shell pkg-config --cflags --libs SDL2_ttf)"

# FIXME need to do something with makefile depending libpigpio is available
# $(shell pkg-config --cflags --libs libpigpio)

lib_encoder:
	make compile LIBNAME=EncoderInterface
