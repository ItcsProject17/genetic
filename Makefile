CC=gcc
CFLAGS=-Wall -pedantic

default: normal

# No optimization and enable GDB debugger
debug: override CFLAGS+=-O0 -g
debug: build

# Define NDEBUG, so that assertions aren't compiled
normal: override CFLAGS+=-DNDEBUG
normal: build

build:
	$(CC) $(CFLAGS) main.c
