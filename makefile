RTMIDI=`pkg-config --cflags --libs rtmidi`

BUILD=-Wno-narrowing -ldl $(RTMIDI)

host: build run
all: libs build so run

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
	g++ -g -o zicHost -Wall zicHost.cpp -fopenmp $(BUILD)
	@echo "\nbuild zicHost done."

run:
	@echo "\n------------------ run ------------------\n"
	./zicHost

try_so:
	g++ -o try_so try_so.cpp
	./try_so
