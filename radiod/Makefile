LD=gcc
CC=gcc

CFLAGS := -std=gnu99 -Wall -ggdb
LDFLAGS := -ggdb
SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))

.Phony: clean

default: radiod

radiod: $(OBJECTS)
	$(LD) $(LDFLAGS) -o radiod $^

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

$(ASM): ./build/%.s: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -S

clean:
	rm -f radiod
	rm -rf build
