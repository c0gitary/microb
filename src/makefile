CC ?= gcc
CFLAGS ?= -Wall -O3
override CFLAGS += $(CDEFS)
LDFLAGS ?= -s

SOURCES = \
	config.c \
	fileman.c \
	logger.c \
	utils.c \
	main.c 

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all: microb

.PHONY: microb
microb: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf microb *.o

config.o: config.c config.h logger.h utils.h
fileman.o: fileman.c fileman.h utils.h config.h
logger.o: logger.c logger.h 
utils.o: utils.c utils.h 
main.o: main.c fileman.h config.h