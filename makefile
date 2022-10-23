CC = g++
WFLAGS = -Wpedantic -Wall -Wextra
#CFLAGS = -std=c++17 -g -ggdb $(WFLAGS)
CFLAGS = -std=c++17 $(WFLAGS)
#CFLAGS = -std=c++17 -O3 -g $(WFLAGS) -DNDEBUG -fopenmp
IFLAGS =
LFLAGS = -lm


all: run

main: main.cpp  camera.h image.h ray.h sphere.h types.h vec3.h
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(IFLAGS)

run: main
	./main

clean:
	rm -rf main *.o

.PHONY: all run clean
