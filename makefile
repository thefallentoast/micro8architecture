# Makefile because yes
CXX    := g++
WINCXX := x86_64-w64-mingw32-g++
FLAGS := -Wall -Wextra -Ofast -march=native
WINFLAGS := -static-libgcc -static-libstdc++


SRCS := $(wildcard src/*.cpp)               # src/main.cpp src/otherfile.cpp
OUTPUT := ./bin/emu
WINOUT := ./bin/emu.exe

.PHONY: all

lr: linux
	$(OUTPUT) ./sample.bin
wr: windows
	$(OUTPUT) ./sample.bin
all: linux windows

linux:
	$(CXX) $(SRCS) -o $(OUTPUT) $(FLAGS)
	
windows:
	$(WINCXX) -c $(SRCS) -o $(OBJS) $(WINFLAGS)
	$(WINCXX) $(OBJS) -o $(WINOUT) $(WINFLAGS)

clean:
	rm -rf ./bin/*