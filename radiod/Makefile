LD=gcc
CC=gcc

CFLAGS := -std=gnu99 -Wall -ggdb 
LDFLAGS := -ggdb 
SOURCES := $(wildcard src/*.c src/*/*.c)
HEADERS := $(wildcard src/*.h src/*/*.h)
OBJECTS := $(subst .c,.o, $(subst src,build, $(SOURCES)))
LIBS := ./vendor/cJSON/libcjson.a ./vendor/iniparser/libiniparser.a
LIB_INCLUDE := -I ./vendor/cJSON -I ./vendor/iniparser/src

.Phony: clean

default: radiod

radiod: $(OBJECTS) $(LIBS)
	$(LD) $(LDFLAGS) -o radiod $^ -lmpv

$(OBJECTS): ./build/%.o: ./src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) $(LIB_INCLUDE)

vendor/cJSON/libcjson.a:
	$(MAKE) -C ./vendor/cJSON

vendor/iniparser/libiniparser.a:
	$(MAKE) -C ./vendor/iniparser

clean:
	rm -f radiod
	rm -rf build
	$(MAKE) -C ./vendor/cJSON clean
	$(MAKE) -C ./vendor/iniparser clean
