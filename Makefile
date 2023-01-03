SOURCES := $(wildcard src/*.c)

SHOGI := $(wildcard src/*.c)

all: arth main
arth: FORCE

FORCE:
main:
	gcc $(SHOGI) -g -pthread -lncurses -lm -lev -o ./build/main.o -o ./bin/main
