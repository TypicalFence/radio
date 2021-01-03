LD=gcc
CC=gcc

CFLAGS := -std=gnu99 -Wall -ggdb -I ./vendor/cJSON
LDFLAGS := -ggdb 
SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))
LIBS := ./vendor/cJSON/libcjson.a

.Phony: clean

default: radiod

radiod: $(OBJECTS) $(LIBS)
	$(LD) $(LDFLAGS) -o radiod $^

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

vendor/cJSON/libcjson.a:
	$(MAKE) -C ./vendor/cJSON

clean:
	rm -f radiod
	rm -rf build
	$(MAKE) -C ./vendor/cJSON clean
