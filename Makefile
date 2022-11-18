main.o: FORCE

FORCE:
	gcc main.c -g   -pthread -lncurses -lm -o main 


# complete:
# 	gcc main.c -g initial.c -g stack.c -g -o main
