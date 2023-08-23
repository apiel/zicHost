# used for OSC
LIBLO=`pkg-config --cflags --libs liblo`

all:
	make clean_so osc clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o interfacePlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o $(EXTRA)

osc:
	make compile LIBNAME=OscInterface EXTRA=$(LIBLO)
