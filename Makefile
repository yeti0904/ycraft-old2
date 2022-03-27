cc  = g++
src = src/*.cc
arg = -std=c++17 -lSDL2 -lSDL2_image -lpthread -Wall -Wextra -Werror -pedantic
out = bin/ycraft

build:
	mkdir -p bin
	$(cc) $(src) $(arg) -s -o $(out)

debug:
	mkdir -p bin
	$(cc) $(src) $(arg) -g -o $(out)

release:
	mkdir -p bin
	$(cc) $(src) $(arg) -s -O3 -o $(out)