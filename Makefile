
OUTPUT_DIR = $(shell pwd)/build/
OUTPUT_FNAME = $(OUTPUT_DIR)/libfmll.so

CC = gcc
STRIP = strip

FLAGS = -pipe -Wall
FLAGS_SHARED = -shared -fPIC
FLAGS_DEBUG = -g
FLAGS_RELEASE = -O2

SRC = nn/test.c # TODO
LIB = 
INCLUDE = -I. -Iinclude -Inn -Ilib

debug: clean

	mkdir -p $(OUTPUT_DIR)
	$(CC) $(FLAGS) $(FLAGS_DEBUG) $(FLAGS_SHARED) $(LIB) $(INCLUDE) $(SRC) -o $(OUTPUT_FNAME)

release: clean
	
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(FLAGS) $(FLAGS_RELEASE) $(FLAGS_SHARED) $(LIB) $(INCLUDE) $(SRC) -o $(OUTPUT_FNAME)
	$(STRIP) $(OUTPUT_FNAME)

ex:

	mkdir -p $(OUTPUT_DIR)
	make -C examples/ \
		OUTPUT_DIR="$(OUTPUT_DIR)" CC="$(CC)" FLAGS="$(FLAGS) $(FLAGS_DEBUG)" LIB_FMLL="-L$(OUTPUT_DIR) -lfmll" INCLUDE_FMLL="-I$(shell pwd)/include"

clean:

	rm -Rf $(OUTPUT_DIR)/*

distclean:

	rm -Rf $(OUTPUT_DIR)

