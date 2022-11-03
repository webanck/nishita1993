CC = g++
WFLAGS = -Wpedantic -Wall -Wextra
#CFLAGS = -std=c++17 -g -ggdb $(WFLAGS)
#CFLAGS = -std=c++17 -O3 -g $(WFLAGS) -DNDEBUG -fopenmp
CFLAGS = -std=c++17 -O3 -DNDEBUG -fopenmp
IFLAGS =
LFLAGS = -lm


all: run

PROGRAMS = main
OBJECTS = atmosphere.o sphere.o

# Auto dependencies (see https://skandhurkat.com/post/makefile-dependencies/)
%.d: %.cpp
	$(CC) -MM -MP $(CFLAGS) $< -o $@
# Include the required .d files in the current makefile (missing ones are generated if necessary).
include $(OBJECTS:.o=.d)
include $(addsuffix .d, $(PROGRAMS))

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $(<:.d=.cpp) $(IFLAGS)

main: main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(IFLAGS)

run: main
	./main

clean:
	rm -rf $(PROGRAMS) *.o *.d

.PHONY: all run clean
