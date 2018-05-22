# These compilation/linker flags will cause the program to halt and
# display a nice warning if memory is improperly used.
#
# This Makefile will *NOT* work on the link machines, unless Nolan
# decides to install libasan and libasan-static.  You can always
# remove -fsanitize=address, but I don't guarantee that'll work
# because that's undefined behavior :)
cflags = -ggdb -std=c++11 -Wall -fsanitize=address
ldflags = $(cflags)

ifdef DEBUG
cflags := $(cflags) -DDEBUG
endif

executables = main

all: $(executables)

%.o : %.cc
	g++ $(cflags) -c -o $@ $<

main: main.o
	g++ $(ldflags) -o $@ $^

.PHONY: all clean test
clean:
	rm -fv $(executables) *.o

test: $(executables)
	echo "No tests given."
