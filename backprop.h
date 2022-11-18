#ifndef BACKPROP_H
#define BACKPROP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <ncurses.h>

#define TRUE 1
#define FALSE 0
#define BOARDROW 10
#define BOARDCOLUMN 10
#define STACKSIZE 500
#define TOTAL_CHESS_NUM 20

int state = 0; //0表未開始, 1表開始
int turn = 1; //第幾手
bool review_mode = 1; //0表示上一步模式，1表示下一步模式
int MAX_turn = 0;
int checkerboard[BOARDROW][BOARDCOLUMN]={0};

int checkerboard_memory[STACKSIZE][BOARDROW][BOARDCOLUMN];
int initial_row_memory[STACKSIZE];
int initial_column_memory[STACKSIZE];
int initial_chess_memory[STACKSIZE];
int goal_row_memory[STACKSIZE];
int goal_column_memory[STACKSIZE];
int goal_chess_memory[STACKSIZE];
int promotion_memory[STACKSIZE];


int X_chess_memory[STACKSIZE][TOTAL_CHESS_NUM];
int Y_chess_memory[STACKSIZE][TOTAL_CHESS_NUM];
int game_mode = 0;
int top = -1;
int X_chess_top = -1;
int Y_chess_top = -1;

struct node{
    int initial_row_memory;
    int initial_column_memory;
    int initial_chess_memory;
    int goal_row_memory;
    int goal_column_memory;
    int goal_chess_memory;
    int promotion_memory;
    struct node *right;
    struct node *left;
};

typedef struct node NODE;

void initial_checkerboard(void);
void move_chess(void);
void call_back(void);
void check_promotion(int goal_row, int goal_column);
int  check_move_chess(int dir, int initial_row, int initial_column, int goal_row, int goal_column, int chess_index, int checkerboard[BOARDROW][BOARDCOLUMN]);
int  check_direction(int initial_row, int initial_column, int goal_row, int goal_column, int chess_index);
void print_checkerboard(void);
void color(int color_code, char word[3]);

void regret(void);
int  isEmpty(void);
void push(int initial_row, int initial_column, int initial_chess, int goal_row, int goal_column, int goal_chess, int X_chess[20], int Y_chess[20]);
int  pop(void);
void file_print_chess(int initial_row, int initial_column, int goal_row, int goal_column, int chess_index);
void print_time(void);
void print_clk(void);
// int  kbhit(void);

void replay(void);
void read_old_game(void);
void review_old_game(void);
int  review_pop_forward(void);
int  review_pop_back(void);

#endif