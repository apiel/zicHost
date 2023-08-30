all:
	make clean_so lib_osc clean_o

clean:
	make clean_so clean_o

clean_so:
	rm -f build/*.so

clean_o:
	rm -f *.o

compile:
	g++ -c -o $(LIBNAME).o interfacePlugin.cpp -fPIC -DPLUGIN_NAME=$(LIBNAME) -DPLUGIN_INCLUDE=\"$(LIBNAME).h\"
	g++ -shared -o build/libzic_$(LIBNAME).so $(LIBNAME).o $(EXTRA)

# sudo apt-get install liblo-dev
lib_osc:
	make compile LIBNAME=OscInterface EXTRA=$(shell pkg-config --cflags --libs liblo)
