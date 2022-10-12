CC = g++
WFLAGS = -Wpedantic -Wall -Wextra
CFLAGS = -std=c++17 -g -ggdb $(WFLAGS)
#CFLAGS = -std=c++17 -O3 -g $(WFLAGS) -DNDEBUG -fopenmp
IFLAGS =
LFLAGS = -lm


OBJECTS =

all: run

main: main.o $(OBJECTS) image.h types.h vec3.h
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(IFLAGS)

run: main
	./main

clean:
	rm -rf $(PROGRAMS) *.o *.d *.json

.PHONY: all run clean
