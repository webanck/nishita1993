CC = g++
WFLAGS = -Wpedantic -Wall -Wextra
CFLAGS = -std=c++17 -g -ggdb $(WFLAGS)
#CFLAGS = -std=c++17 -O3 -g $(WFLAGS) -DNDEBUG -fopenmp
IFLAGS =
LFLAGS = -lm


all: run

main: main.cpp image.h sphere.h types.h vec3.h
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(IFLAGS)

run: main
	./main

clean:
	rm -rf $(PROGRAMS) *.o

.PHONY: all run clean
