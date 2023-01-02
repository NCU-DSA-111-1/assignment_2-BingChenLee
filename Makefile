SOURCES := $(wildcard src/*.c)

SHOGI := $(wildcard src/*.c)

all: arth main
arth: FORCE

FORCE:
main:
	gcc $(SHOGI) -g -pthread -lncurses -lm -lev -o ./build/main.o -o ./bin/main

# 	gcc ./src/main.c -g -pthread -lncurses -lm -lev -o ./bin/main~‵

# main.o: src/main
# 	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

# complete:
# 	gcc main.c -g initial.c -g stack.c -g -o main
