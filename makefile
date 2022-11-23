CC = g++
WFLAGS = -Wpedantic -Wall -Wextra
#PFLAGS = -g -pg
PFLAGS = -g -static
#CFLAGS = -std=c++17 -g -ggdb $(WFLAGS)
CFLAGS = -std=c++17 -O3 $(PFLAGS) $(WFLAGS) -DNDEBUG -fopenmp
#CFLAGS = -std=c++17 -O3 -DNDEBUG $(WFLAGS) -fopenmp
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

gmon.out: run
gprof: gmon.out
	gprof main $^

perf.data: main
	perf record -e cycles ./$^
perf: perf.data
	perf report --stdio

clean:
	rm -rf $(PROGRAMS) *.o *.d gmon.out perf.data

.PHONY: all run clean
