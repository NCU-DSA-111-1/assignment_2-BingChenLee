#include "backprop.h"
// #include "timer.h"

struct playerchess{
    int chess[20];
};

struct time {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
};

struct time initial = {0,0,0,0,0,0};
struct time x_begin = {0,0,0,0,0,0};
struct time y_begin = {0,0,0,0,0,0};
struct time x_end = {0,0,0,0,0,0};
struct time y_end = {0,0,0,0,0,0};
struct time current = {0,0,0,0,0,0};

struct playerchess X = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
struct playerchess Y = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

NODE *stack = NULL;

FILE *new_shogi_game = NULL;
FILE *old_shogi_game = NULL;

int main(int argc, char **argv){

    system("clear");
    int cmd_opt =0 ;
    extern char *optarg;

    while((cmd_opt = getopt(argc, argv, "ns:l:")) != -1){

        switch(cmd_opt){
            case 'n':
                game_mode = 1;
                break;

            case 's':
                printf("%s\n",optarg);
                
                if((new_shogi_game = fopen(optarg,"w+"))!=NULL){
                    printf("%s\n",optarg);
                    fprintf(new_shogi_game, "%3s%8s%15s\n","手","走法","升變");
                    initial_checkerboard();
                    print_checkerboard();
                }
                break;

            case 'l':
                game_mode = 2;
                if((old_shogi_game = fopen(optarg,"r"))!=NULL){
                    initial_checkerboard();
                    read_old_game();
                }
                break;
        }
    }
        
    return 0;
}

void initial_checkerboard(){

    checkerboard[2][0] = checkerboard[2][8] =checkerboard[8][0] = checkerboard[8][8] = 0;
    for(int i=2; i<=6; i++){
        checkerboard[2][i]=0;
        checkerboard[8][i]=0;
    }

    for(int i=4; i<=6; i++){
        for(int j=0; j<=8; j++){
            checkerboard[i][j]=0;
        }
    }

    //-1表示紅方香、-2表示紅方桂、-3表示紅方銀、-4表示紅方金、-5表示紅方王、-6表示紅方飛、-7表示紅方角、-8表示紅方步、-9表示紅方成香、-10表示紅方成桂、-11表示紅方成銀、-12表示紅方成步、-13表示紅方龍王、-14表示紅方龍馬
    checkerboard[1][0] = checkerboard[1][8] = -1;
    checkerboard[1][1] = checkerboard[1][7] = -2;
    checkerboard[1][2] = checkerboard[1][6] = -3;
    checkerboard[1][3] = checkerboard[1][5] = -4;
    checkerboard[1][4] = -5;
    checkerboard[2][1] = -6;
    checkerboard[2][7] = -7;
    checkerboard[3][0] = checkerboard[3][1] = checkerboard[3][2] = checkerboard[3][3] = checkerboard[3][4] = checkerboard[3][5] = checkerboard[3][6] = checkerboard[3][7] = checkerboard[3][8] = -8; 
    

    //1表示藍方香、2表示藍方桂、3表示藍方銀、4表示藍方金、5表示藍方王、6表示藍方飛、7表示藍方角、8表示藍方步、9表示藍方成香、10表示藍方成桂、11表示藍方成銀、12表示藍方成步、13表示藍方龍王、14表示藍方龍馬
    checkerboard[9][0] = checkerboard[9][8] = 1;
    checkerboard[9][1] = checkerboard[9][7] = 2;
    checkerboard[9][2] = checkerboard[9][6] = 3;
    checkerboard[9][3] = checkerboard[9][5] = 4;
    checkerboard[9][4] = 5;
    checkerboard[8][7] = 6;
    checkerboard[8][1] = 7;
    checkerboard[7][0] = checkerboard[7][1] = checkerboard[7][2] = checkerboard[7][3] = checkerboard[7][4] = checkerboard[7][5] = checkerboard[7][6] = checkerboard[7][7] = checkerboard[7][8] = 8; 
    
}

void move_chess(){

    // time_t t = time(NULL);
    // struct tm tm = *localtime(&t);

    int initial_row = -2, initial_column = -2, goal_row = -2, goal_column = -2, chess_index = -2;
    int actual_column_1, actual_column_2;
    int i=0, j=0, dir;

    state = 1;
    
    if(turn%2 == 1){

        printf("\033[24;0f");
        printf("\033[34m玩家X\033[m請輸入要移動將棋的 原始座標 與 目的座標\n");

        print_clk();

        scanf("%d", &actual_column_1);
        initial_column = 9-actual_column_1;
        
        if(actual_column_1 == -1){
            fclose(new_shogi_game);
            printf("對局過程已記錄完成\n");
            exit(0);
        }
        else if(actual_column_1 == 0){
            regret();
        }
        else if(actual_column_1 == 10){
            call_back();
        }

        scanf("%d %d %d", &initial_row, &actual_column_2, &goal_row);
        goal_column = 9-actual_column_2;
        
        if(checkerboard[initial_row][initial_column] > 0){
            if(checkerboard[goal_row][goal_column] <= 0){
                dir = check_direction(initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column]);
                if(check_move_chess(dir, initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column], checkerboard) == TRUE){
                    push(initial_row, initial_column, checkerboard[initial_row][initial_column], goal_row, goal_column, checkerboard[goal_row][goal_column], X.chess, Y.chess);
                    file_print_chess(initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column]); 

                    if(checkerboard[goal_row][goal_column] < 0){
                        if(checkerboard[goal_row][goal_column] < -8){
                            if(checkerboard[goal_row][goal_column] == -9){
                                checkerboard[goal_row][goal_column] = -1;
                            }

                            else if(checkerboard[goal_row][goal_column] == -10){
                                checkerboard[goal_row][goal_column] = -2;
                            }

                            else if(checkerboard[goal_row][goal_column] == -11){
                                checkerboard[goal_row][goal_column] = -3;
                            }

                            else if(checkerboard[goal_row][goal_column] == -12){
                                checkerboard[goal_row][goal_column] = -8;
                            }

                            else if(checkerboard[goal_row][goal_column] == -13){
                                checkerboard[goal_row][goal_column] = -6;
                            }

                            else if(checkerboard[goal_row][goal_column] == -14){
                                checkerboard[goal_row][goal_column] = -7;
                            }
                        }
                        X_chess_top ++;
                        X.chess[X_chess_top] = -(checkerboard[goal_row][goal_column]);
                    }

                    if(checkerboard[goal_row][goal_column] == -5){ //獲勝條件
                        checkerboard[goal_row][goal_column]=checkerboard[initial_row][initial_column];
                        checkerboard[initial_row][initial_column]=0;
                        game_mode = 3; //防止回到move_chess
                        turn++; 
                        print_checkerboard();
                        printf("\n先方(X)獲勝！\n");
                        fclose(new_shogi_game);
                        exit(0);
                    }
                    else{
                        checkerboard[goal_row][goal_column]=checkerboard[initial_row][initial_column];
                        checkerboard[initial_row][initial_column]=0;
                        check_promotion(goal_row, goal_column);
                    }
                    
                }
                else{
                    printf("移動方式不合基礎規則或中途路徑有其他將棋，請重新輸入\n");
                    move_chess();
                }
            }
            else{
                printf("輸入的位置有其他將棋，不可移動，請重新輸入\n");
                move_chess();
            }
        }

        else{
            printf("無法找到指定將棋，請重新輸入\n");
            move_chess();
        }

        turn++; 
        print_checkerboard();
        
    }

    else{

        printf("\033[24;0f");
        printf("\033[31m玩家Y\033[m請輸入要移動將棋的 原始座標 與 目的座標\n");

        print_clk();

        scanf("%d", &actual_column_1);
        initial_column = 9-actual_column_1;
        
        if(actual_column_1 == -1){
            fclose(new_shogi_game);
            printf("對局過程已記錄完成\n");
            exit(0);
        }
        else if(actual_column_1 == 0){
            regret();
        }
        else if(actual_column_1 == 10){
            call_back();
        }

        scanf("%d %d %d", &initial_row, &actual_column_2, &goal_row);
        goal_column = 9-actual_column_2;
        
        if(checkerboard[initial_row][initial_column] < 0){
            if(checkerboard[goal_row][goal_column] >= 0){
                dir = check_direction(initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column]);
                if(check_move_chess(dir, initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column], checkerboard) == TRUE){
                    push(initial_row, initial_column, checkerboard[initial_row][initial_column], goal_row, goal_column, checkerboard[goal_row][goal_column], X.chess, Y.chess);
                    file_print_chess(initial_row, initial_column, goal_row, goal_column, checkerboard[initial_row][initial_column]); 

                    if(checkerboard[goal_row][goal_column] > 0){
                        if(checkerboard[goal_row][goal_column] > 8){
                            if(checkerboard[goal_row][goal_column] == 9){
                                checkerboard[goal_row][goal_column] = 1;
                            }

                            else if(checkerboard[goal_row][goal_column] == 10){
                                checkerboard[goal_row][goal_column] = 2;
                            }

                            else if(checkerboard[goal_row][goal_column] == 11){
                                checkerboard[goal_row][goal_column] = 3;
                            }

                            else if(checkerboard[goal_row][goal_column] == 12){
                                checkerboard[goal_row][goal_column] = 8;
                            }

                            else if(checkerboard[goal_row][goal_column] == 13){
                                checkerboard[goal_row][goal_column] = 6;
                            }

                            else if(checkerboard[goal_row][goal_column] == 14){
                                checkerboard[goal_row][goal_column] = 7;
                            }
                        }
                        Y_chess_top ++;
                        Y.chess[Y_chess_top] = -(checkerboard[goal_row][goal_column]);
                    }

                    if(checkerboard[goal_row][goal_column] == 5){ //獲勝條件
                        checkerboard[goal_row][goal_column]=checkerboard[initial_row][initial_column];
                        checkerboard[initial_row][initial_column]=0;
                        game_mode = 3; //防止回到move_chess
                        turn++; 
                        print_checkerboard();
                        printf("\n後方(Y)獲勝！\n");
                        fclose(new_shogi_game);
                        exit(0);
                    }
                    else{
                        checkerboard[goal_row][goal_column]=checkerboard[initial_row][initial_column];
                        checkerboard[initial_row][initial_column]=0;
                        check_promotion(goal_row, goal_column);
                    }
                    
                }
                else{
                    printf("移動方式不合基礎規則或中途路徑有其他將棋，請重新輸入\n");
                    move_chess();
                }
            }
            else{
                printf("輸入的位置有其他將棋，不可移動，請重新輸入\n");
                move_chess();
            }
        }

        else{
            printf("無法找到指定將棋，請重新輸入\n");
            move_chess();
        }

        turn++; 
        print_checkerboard();
    }
}

void call_back(){
    int i;
    int call_back_index;
    int call_back_row, call_back_column;

    if(turn%2 == 1){
        if(X_chess_top != -1){
            printf("請選擇要打入的棋子號碼(1~%d)\n",X_chess_top+1);
            scanf("%d",&call_back_index);
            printf("請輸入要打入的位置\n");
            scanf("%d %d",&call_back_column, &call_back_row);

            //步兵
            if(X.chess[call_back_index-1] == 8){
                if(call_back_row==1){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }

                for(i=1; i<=9; i++){
                    if(checkerboard[i][BOARDCOLUMN-call_back_column-1] == 8){
                        printf("一筋二步，該位置無法放置棋子，請重新輸入\n");
                        move_chess();
                    }
                }
            }

            //桂馬
            else if(X.chess[call_back_index-1] == 1){
                if(call_back_row<=2){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }
            }

            //香車
            else if(X.chess[call_back_index-1] == 2){
                if(call_back_row==1){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }
            }

            if(checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1] == 0){
                push(0, 0, 0, call_back_row, BOARDCOLUMN-call_back_column-1, checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1], X.chess, Y.chess);
                checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1] = X.chess[call_back_index-1];
                file_print_chess(X_chess_top, 0, call_back_row, 9-call_back_column, X.chess[call_back_index-1]);
                fprintf(new_shogi_game, " N\n");
                X.chess[call_back_index-1] = 0;
                for(i=0; i< X_chess_top; i++){
                    if(X.chess[i] == 0){
                        X.chess[i] = X.chess[i+1];
                        X.chess[i+1] = 0;
                    }
                }
                X_chess_top--;
                turn++; 
                print_checkerboard();
            }
            else{
                printf("該位置無法放置棋子，請重新輸入\n");
                move_chess();
            }
        }
        else{
            printf("你還沒有持駒，請重新輸入\n");
            move_chess();
        }
        
    }

    else if(turn%2 == 0){
        if(Y_chess_top != -1){
            printf("請選擇要打入的棋子號碼(1~%d)\n",Y_chess_top+1);
            scanf("%d",&call_back_index);
            printf("請輸入要打入的位置\n");
            scanf("%d %d",&call_back_column, &call_back_row);

            //步兵
            if(Y.chess[call_back_index-1] == -8){
                if(call_back_row==9){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }

                for(i=1; i<=9; i++){
                    if(checkerboard[i][BOARDCOLUMN-call_back_column-1] == -8){
                        printf("一筋二步，該位置無法放置棋子，請重新輸入\n");
                        move_chess();
                    }
                }
            }

            //桂馬
            else if(Y.chess[call_back_index-1] == -1){
                if(call_back_row>=8){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }
            }

            //香車
            else if(Y.chess[call_back_index-1] == -2){
                if(call_back_row==9){
                    printf("下一步無法移動，該位置無法放置棋子，請重新輸入\n");
                    move_chess();
                }
            }

            if(checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1] == 0){
                push(0, 0, 0, call_back_row, BOARDCOLUMN-call_back_column-1, checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1], X.chess, Y.chess);
                checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1] = Y.chess[call_back_index-1];
                file_print_chess(Y_chess_top, 0, call_back_row, 9-call_back_column, Y.chess[call_back_index-1]);
                fprintf(new_shogi_game, " N\n");
                Y.chess[call_back_index-1] = 0;
                for(i=0; i< Y_chess_top; i++){
                    if(Y.chess[i] == 0){
                        Y.chess[i] = Y.chess[i+1];
                        Y.chess[i+1] = 0;
                    }
                }
                Y_chess_top--; 
                turn++; 
                print_checkerboard();
            }
            else{
                printf("該位置無法放置棋子，請重新輸入\n");
                move_chess();
            }
        }
        else{
            printf("你還沒有持駒，請重新輸入\n");
            move_chess();
        }
    }
}

void check_promotion(int goal_row, int goal_column){

    char check;

    //一般升變
    //香變成香
    if(abs(checkerboard[goal_row][goal_column]) == 1){
        if((turn%2 == 1 && goal_row == 1) || (turn%2 == 0 && goal_row == 9)){
            if(checkerboard[goal_row][goal_column]>0){
                checkerboard[goal_row][goal_column] = 9;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }

            else if(checkerboard[goal_row][goal_column]<0){
                checkerboard[goal_row][goal_column] = -9;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }
        }
        else if((turn%2 == 1 && goal_row <= 3) || (turn%2 == 0 && goal_row >= 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = 9;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -9;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n');
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    //桂變成桂
    else if(abs(checkerboard[goal_row][goal_column]) == 2){
        if((turn%2 == 1 && goal_row <= 2) || (turn%2 == 0 && goal_row >= 8)){
            if(checkerboard[goal_row][goal_column]>0){
                checkerboard[goal_row][goal_column] = 10;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }

            else if(checkerboard[goal_row][goal_column]<0){
                checkerboard[goal_row][goal_column] = -10;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }
        }
        else if((turn%2 == 1 && goal_row == 3) || (turn%2 == 0 && goal_row == 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = 10;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -10;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n'); 
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    //銀變成銀
    else if(abs(checkerboard[goal_row][goal_column]) == 3){
        if((turn%2 == 1 && goal_row <= 3) || (turn%2 == 0 && goal_row >= 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] =11;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -11;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n');
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    //飛變龍王
    else if(abs(checkerboard[goal_row][goal_column]) == 6){
        if((turn%2 == 1 && goal_row <= 3) || (turn%2 == 0 && goal_row >= 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = 13;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -13;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n');
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    //角變龍馬
    else if(abs(checkerboard[goal_row][goal_column]) == 7){
        if((turn%2 == 1 && goal_row <= 3) || (turn%2 == 0 && goal_row >= 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = 14;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -14;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n');
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    //步變成步
    else if(abs(checkerboard[goal_row][goal_column]) == 8){
        if((turn%2 == 1 && goal_row == 1) || (turn%2 == 0 && goal_row == 9)){
            if(checkerboard[goal_row][goal_column]>0){
                checkerboard[goal_row][goal_column] = 12;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }

            else if(checkerboard[goal_row][goal_column]<0){
                checkerboard[goal_row][goal_column] = -12;
                fprintf(new_shogi_game, " Y\n");
                promotion_memory[top-1] = 1;
            }
        }
        else if((turn%2 == 1 && goal_row <= 3) || (turn%2 == 0 && goal_row >= 7)){
            do{
                printf("可進行升變，是否進行升變[Y/N]\n");
                getc(stdin);
                scanf("%c",&check);

                if(checkerboard[goal_row][goal_column]>0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = 12;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(checkerboard[goal_row][goal_column]<0 && (check == 'Y' || check == 'y')){
                    checkerboard[goal_row][goal_column] = -12;
                    fprintf(new_shogi_game, " Y\n");
                    promotion_memory[top-1] = 1;
                }

                else if(check == 'N' || check == 'n'){
                    fprintf(new_shogi_game, " N\n");
                    promotion_memory[top-1] = 0;
                }

            }while(check != 'Y' && check != 'y' && check != 'N' && check != 'n');
        }
        else{
            fprintf(new_shogi_game, " N\n");
            promotion_memory[top-1] = 0;
        }
    }

    else{
        fprintf(new_shogi_game, " N\n");
        promotion_memory[top-1] = 0;
    }
}

int check_direction(int initial_row, int initial_column, int goal_row, int goal_column, int chess_index){
    if(goal_column-initial_column == 0 && goal_row-initial_row < 0){
        return 1;
    }
    
    else if(goal_column-initial_column > 0 && goal_column-initial_column == -(goal_row-initial_row)){
        return 2;
    }

    else if(goal_column-initial_column > 0 && goal_row-initial_row == 0){
        return 3;
    }

    else if(goal_column-initial_column > 0 && goal_column-initial_column == goal_row-initial_row){
        return 4;
    }

    else if(goal_column-initial_column == 0 && goal_row-initial_row > 0){
        return 5;
    }

    else if(goal_column-initial_column < 0 && goal_column-initial_column == -(goal_row-initial_row)){
        return 6;
    }

    else if(goal_column-initial_column < 0 && goal_row-initial_row == 0){
        return 7;
    }

    else if(goal_column-initial_column < 0 && goal_column-initial_column == goal_row-initial_row){
        return 8;
    }

    else if(goal_column-initial_column == 1 && goal_row-initial_row == -2){
        return 9;
    }

    else if(goal_column-initial_column == -1 && goal_row-initial_row == -2){
        return 10;
    }

    else if(goal_column-initial_column == -1 && goal_row-initial_row == 2){
        return 11;
    }

    else if(goal_column-initial_column == 1 && goal_row-initial_row == 2){
        return 12;
    }
}

int check_move_chess(int dir, int initial_row, int initial_column, int goal_row, int goal_column, int chess_index, int checkerboard[BOARDROW][BOARDCOLUMN]){
    double distance = sqrt(pow((double)(initial_column-goal_column),2)+pow((double)(goal_row-initial_row),2));
    //printf("turn %d buffer= %d %d %d %d %d, dir= %d, distance= %f\n",turn, initial_column, initial_row, goal_column, goal_row, chess_index, dir, distance);

    int i;
    int check_point = 0;

    // 王將走八方各一格
    if((abs(chess_index) == 5) && dir <= 8 && distance < 2){
        return TRUE;
    }

    // 金將、成香、成桂、成銀、成步走上下左右、左前、右前
    else if((abs(chess_index) == 4 || abs(chess_index) == 9 || abs(chess_index) == 10 || abs(chess_index) == 11 || abs(chess_index) == 12) && ((turn%2 == 1 && (dir == 2 || dir == 8)) || ((turn%2 == 0) && (dir == 4 || dir == 6)) || (dir == 1 || dir == 3 || dir == 5 || dir == 7)) && distance < 2){
        return TRUE;
    }

    // 銀將走上跟四個對角線
    else if(abs(chess_index) == 3 && ((turn%2 == 1 && dir == 1) || (turn%2 == 0 && dir == 5) || (dir == 2 || dir == 4 || dir == 6 || dir == 8)) && distance < 2){
        return TRUE;
    }

    // 桂馬走左上跟右上兩格
    else if(abs(chess_index) == 2 && ((turn%2 == 1 && (dir == 9 || dir == 10)) || (turn%2 == 0 && (dir == 11 || dir == 12)))){
        return TRUE;
    }

    // 香車走一直線
    else if(abs(chess_index) == 1){

        if(turn%2 == 1 && dir == 1){

            for(i = initial_row-1; i > goal_row; i--){ 
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(turn%2 == 0 && dir == 5){

            for(i = initial_row+1; i < goal_row; i++){ 
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }
    }

    // 飛車走十字
    else if(abs(chess_index) == 6){
        if(dir == 1){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 3){
            for(i = initial_column+1; i < goal_column; i++){
                if(checkerboard[initial_row][i] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE; 
        }

        else if(dir == 5){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 7){
            for(i = initial_column-1; i > goal_column; i--){
                if(checkerboard[initial_row][i] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }
    }

    // 角行走對角
    else if(abs(chess_index) == 7){
        if(dir == 2){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column-(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 4){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column+(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 6){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column-(i-initial_row)] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 8){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column+(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }
    }

    // 步兵往前一格
    else if(abs(chess_index) == 8 &&  ( (turn%2 == 1 && dir == 1 && distance == 1) || (turn%2 == 0 && dir == 5 && distance == 1))) {
        return TRUE;    
    }

    // 龍王走十字+四個對角
    else if(abs(chess_index) == 13){
        if(dir == 1){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 3){
            for(i = initial_column+1; i < goal_column; i++){
                if(checkerboard[initial_row][i] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE; 
        }

        else if(dir == 5){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 7){
            for(i = initial_column-1; i > goal_column; i--){
                if(checkerboard[initial_row][i] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir%2 == 0 && distance < 2){
            return TRUE;
        }
    }

    // 龍馬走對角+四個直線
    else if(abs(chess_index) == 14){
        if(dir == 2){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column-(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 4){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column+(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 6){
            for(i = initial_row+1; i < goal_row; i++){
                if(checkerboard[i][initial_column-(i-initial_row)] != 0){
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir == 8){
            for(i = initial_row-1; i > goal_row; i--){
                if(checkerboard[i][initial_column+(i-initial_row)] != 0){ 
                    return FALSE;
                    break;
                }
            }
            return TRUE;
        }

        else if(dir%2 == 1 && distance == 1){
            return TRUE;
        }
    }

    return FALSE;
}

void print_checkerboard(){

    system("clear"); //Clear the screen

    int i=0, j=0;
    int color_code = 0;
    char chess;

    print_time();
    printf("\n\033[33m第%d手 玩家%s的回合\033[m\n",turn, turn%2==1 ? "X":"Y");
    
    printf("\n\033[31mY方持駒: \033[m");
    for(i=0; i<20; i++){
        if(Y.chess[i] != 0){
            if(Y.chess[i] == -1){
                printf("香");
            }

            else if(Y.chess[i] == -2){
                printf("桂");
            }

            else if(Y.chess[i] == -3){
                printf("銀");
            }

            else if(Y.chess[i] == -4){
                printf("金");
            }

            else if(Y.chess[i] == -6){
                printf("飛");
            }

            else if(Y.chess[i] == -7){
                printf("角");
            }

            else if(Y.chess[i] == -8){
                printf("步");
            }
        }
    }
    printf("\n--------------------------\n");
    for(j=BOARDCOLUMN-1; j>0; j--){
        printf("%d ",j);
    }
    
    printf("\n");

    for(i=1; i<BOARDROW; i++){
        for(j=0; j<BOARDCOLUMN-1; j++){
            if(checkerboard[i][j] > 0){
                color_code = 34;
            }
            else if(checkerboard[i][j] < 0){
                color_code = 31;
            }

            if(abs(checkerboard[i][j]) == 1){
                color(color_code,"香");
            }

            else if(abs(checkerboard[i][j]) == 2){
                color(color_code,"桂");
            }

            else if(abs(checkerboard[i][j]) == 3){
                color(color_code,"銀");
            }

            else if(abs(checkerboard[i][j]) == 4){
                color(color_code,"金");
            }

            else if(abs(checkerboard[i][j]) == 5){
                color(color_code,"王");
            }

            else if(abs(checkerboard[i][j]) == 6){
                color(color_code,"飛");
            }

            else if(abs(checkerboard[i][j]) == 7){
                color(color_code,"角");
            }

            else if(abs(checkerboard[i][j]) == 8){
                color(color_code,"步");
            }

            else if(abs(checkerboard[i][j]) == 9){
                color(color_code,"杏");
            }

            else if(abs(checkerboard[i][j]) == 10){
                color(color_code,"圭");
            }

            else if(abs(checkerboard[i][j]) == 11){
                color(color_code,"全");
            }

            else if(abs(checkerboard[i][j]) == 12){
                color(color_code,"と");
            }

            else if(abs(checkerboard[i][j]) == 13){
                color(color_code,"竜");
            }

            else if(abs(checkerboard[i][j]) == 14){
                color(color_code,"馬");
            }

            else{
                printf("　");
            }
        }
        printf("%d\n", i);
    }

    printf("--------------------------\n");
    printf("\033[34mX方持駒: \033[m");
    for(i=0; i<20; i++){
        if(X.chess[i] != 0){
            if(X.chess[i] == 1){
                printf("香");
            }

            else if(X.chess[i] == 2){
                printf("桂");
            }

            else if(X.chess[i] == 3){
                printf("銀");
            }

            else if(X.chess[i] == 4){
                printf("金");
            }

            else if(X.chess[i] == 6){
                printf("飛");
            }

            else if(X.chess[i] == 7){
                printf("角");
            }

            else if(X.chess[i] == 8){
                printf("步");
            }
        }
    }
    printf("\n");

    if(game_mode == 1){
        move_chess();
    }
}

void color(int color_code, char word[3]){
    printf("\033[%dm%s\033[m", color_code, word);
}

void regret(){

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    if(pop()){
        if(turn%2 == 1){
            x_begin.hour = tm.tm_hour;
            x_begin.min = tm.tm_min;
            x_begin.sec = tm.tm_sec;
        }

        else if(turn%2 == 0){
            y_begin.hour = tm.tm_hour;
            y_begin.min = tm.tm_min;
            y_begin.sec = tm.tm_sec;
        }

        turn--;
        print_checkerboard();
    } 
}

int isEmpty(){
    if(top == -2){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void push(int initial_row, int initial_column, int initial_chess, int goal_row, int goal_column, int goal_chess, int X_chess[20], int Y_chess[20]){
    int i,j;
    NODE *tmp;

    if(top >= STACKSIZE){
        printf("儲存空間已滿\n");
    }
    else{
        top++;

        if((tmp=(NODE*)malloc(sizeof(NODE))) != NULL){
            tmp->initial_row_memory = initial_row;
            tmp->initial_column_memory = initial_column;
            tmp->initial_chess_memory = initial_chess;
            tmp->goal_row_memory = goal_row;
            tmp->goal_column_memory = goal_column;
            tmp->goal_chess_memory = goal_chess;
            tmp->left = NULL;
            if(stack != NULL){
                tmp->right = stack;
                stack->left = tmp;
            }
            stack = tmp;
        }

        for(i=0; i<20; i++){
            X_chess_memory[top][i] = X_chess[i];
            Y_chess_memory[top][i] = Y_chess[i];
        }
       
    }
}

int pop(){
    int i,j;
    int Xmax = -1, Ymax = -1;
    NODE *tmp = NULL;

    if(stack == NULL){
        printf("無法再悔棋\n");
        move_chess();
    }
    else{
        fseek(new_shogi_game, -18, SEEK_CUR);

        tmp = stack;
        stack = stack->right;
        checkerboard[tmp->initial_row_memory][tmp->initial_column_memory] = tmp->initial_chess_memory;
        checkerboard[tmp->goal_row_memory][tmp->goal_column_memory] = tmp->goal_chess_memory;
        free(tmp);

        for(i=0; i<20; i++){
            X.chess[i] = X_chess_memory[top][i];
            Y.chess[i] = Y_chess_memory[top][i];
            if(X.chess[i] != 0){
                Xmax++;
            }
            if(Y.chess[i] != 0){
                Ymax++;
            }
        }
        X_chess_top = Xmax;
        Y_chess_top = Ymax;
        top--;
        return TRUE;
    }

    // //Todo: 悔棋顯示問題
    // if(isEmpty()){
    //     printf("無法再悔棋\n");
    //     move_chess();
    // }
    // else{
    //     fseek(new_shogi_game, -18, SEEK_CUR);

    //     // for(i=0; i<BOARDROW; i++){
    //     //     for(j=0; j<BOARDCOLUMN; j++){
    //     //         checkerboard[i][j] = checkerboard_memory[top][i][j];
    //     //     }
    //     // }

    //     checkerboard[initial_row_memory[top]][initial_column_memory[top]] = initial_chess_memory[top];
    //     checkerboard[goal_row_memory[top]][goal_column_memory[top]] = goal_chess_memory[top];

    //     for(i=0; i<20; i++){
    //         X.chess[i] = X_chess_memory[top][i];
    //         Y.chess[i] = Y_chess_memory[top][i];
    //         if(X.chess[i] != 0){
    //             Xmax++;
    //         }
    //         if(Y.chess[i] != 0){
    //             Ymax++;
    //         }
    //     }
    //     X_chess_top = Xmax;
    //     Y_chess_top = Ymax;
    //     top--;
    //     return TRUE;
    // }
}

void file_print_chess(int initial_row, int initial_column, int goal_row, int goal_column, int chess_index){
    fprintf(new_shogi_game, "%3d %d %d",turn, 9-goal_column, goal_row);

    if(abs(chess_index)==1){
        fprintf(new_shogi_game, " 香 ");
    }

    else if(abs(chess_index)==2){
        fprintf(new_shogi_game, " 桂 ");
    }

    else if(abs(chess_index)==3){
        fprintf(new_shogi_game, " 銀 ");
    }

    else if(abs(chess_index)==4){
        fprintf(new_shogi_game, " 金 ");
    }

    else if(abs(chess_index)==5){
        fprintf(new_shogi_game, " 王 ");
    }

    else if(abs(chess_index)==6){
        fprintf(new_shogi_game, " 飛 ");
    }

    else if(abs(chess_index)==7){
        fprintf(new_shogi_game, " 角 ");
    }

    else if(abs(chess_index)==8){
        fprintf(new_shogi_game, " 步 ");
    }

    else if(abs(chess_index)==9){
        fprintf(new_shogi_game, " 杏 ");
    }

    else if(abs(chess_index)==10){
        fprintf(new_shogi_game, " 圭 ");
    }

    else if(abs(chess_index)==11){
        fprintf(new_shogi_game, " 全 ");
    }

    else if(abs(chess_index)==12){
        fprintf(new_shogi_game, " と ");
    }

    else if(abs(chess_index)==13){
        fprintf(new_shogi_game, " 竜 ");
    }

    else if(abs(chess_index)==14){
        fprintf(new_shogi_game, " 馬 ");
    }

    fprintf(new_shogi_game, "%d %d", 9-initial_column, initial_row);
}

void print_time(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(state == 0){
        initial.year = tm.tm_year+1900;
        initial.month = tm.tm_mon + 1;
        initial.day = tm.tm_mday;
        initial.hour = tm.tm_hour;
        initial.min = tm.tm_min;
        initial.sec = tm.tm_sec;
        state = 1;
        //fprintf(new_shogi_game, "\033[33m開始時間: %d.%02d.%02d %02d:%02d\033[m\n", initial.year, initial.month, initial.day, initial.hour, initial.min);
    }

    printf("\033[33mShogi 日本將棋%s\033[m\n\n", game_mode == 2 ? " (上局複盤)" : " ");
    printf("\033[33m開始時間: %d.%02d.%02d %02d:%02d\033[m\n", initial.year, initial.month, initial.day, initial.hour, initial.min);
    printf("\033[33m%s\033[m\n", turn%2 == 1 ? "*先手: X" : " 先手: X");
    printf("\033[33m%s\033[m\n", turn%2 == 0 ? "*後手: Y" : " 後手: Y");
}

void print_clk(){

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(turn%2 == 0){
        if(turn >= 2){
            if(tm.tm_min-x_begin.min < 0 && tm.tm_sec-x_begin.sec < 0){
                x_end.hour += tm.tm_hour-x_begin.hour-1;
                x_end.min += tm.tm_min-x_begin.min+59;
                x_end.sec += tm.tm_sec-x_begin.sec+60;
            }

            else if(tm.tm_min-x_begin.min < 0 && tm.tm_sec-x_begin.sec >= 0){
                x_end.hour += tm.tm_hour-x_begin.hour-1;
                x_end.min += tm.tm_min-x_begin.min+60;
                x_end.sec += tm.tm_sec-x_begin.sec;
            }

            else if(tm.tm_min-x_begin.min >= 0 && tm.tm_sec-x_begin.sec < 0){
                x_end.hour += tm.tm_hour-x_begin.hour;
                x_end.min += tm.tm_min-x_begin.min-1;
                x_end.sec += tm.tm_sec-x_begin.sec+60;
            }

            else{
                x_end.hour += tm.tm_hour-x_begin.hour;
                x_end.min += tm.tm_min-x_begin.min;
                x_end.sec += tm.tm_sec-x_begin.sec;
            }
        }
        if(x_end.sec >= 60){
            x_end.sec -= 60;
            x_end.min += 1;
        }
        if(x_end.min >= 60){
            x_end.min -= 60;
            x_end.hour += 1;
        }
        y_begin.hour = tm.tm_hour;
        y_begin.min = tm.tm_min;
        y_begin.sec = tm.tm_sec;
    }

    else if(turn%2 == 1){
        if(turn >= 3){
            if(tm.tm_min-y_begin.min < 0 && tm.tm_sec-y_begin.sec < 0){
                y_end.hour += tm.tm_hour-y_begin.hour-1;
                y_end.min += tm.tm_min-y_begin.min+59;
                y_end.sec += tm.tm_sec-y_begin.sec+60;
            }

            else if(tm.tm_min-y_begin.min < 0 && tm.tm_sec-y_begin.sec >= 0){
                y_end.hour += tm.tm_hour-y_begin.hour-1;
                y_end.min += tm.tm_min-y_begin.min+60;
                y_end.sec += tm.tm_sec-y_begin.sec;
            }

            else if(tm.tm_min-y_begin.min >= 0 && tm.tm_sec-y_begin.sec < 0){
                y_end.hour += tm.tm_hour-y_begin.hour;
                y_end.min += tm.tm_min-y_begin.min-1;
                y_end.sec += tm.tm_sec-y_begin.sec+60;
            }

            else{
                y_end.hour += tm.tm_hour-y_begin.hour;
                y_end.min += tm.tm_min-y_begin.min;
                y_end.sec += tm.tm_sec-y_begin.sec;
            }
        }
        if(y_end.sec >= 60){
            y_end.sec -= 60;
            y_end.min += 1;
        }
        if(y_end.min >= 60){
            y_end.min -= 60;
            y_end.hour += 1;
        }
        x_begin.hour = tm.tm_hour;
        x_begin.min = tm.tm_min;
        x_begin.sec = tm.tm_sec;
    }
    
    printf("\033[4;11f");

    if(x_end.sec<0 && x_end.min<10){
        printf("\033[33m0%d:\033[m", x_end.min-1);
    }
    else if(x_end.min<10){
        printf("\033[33m0%d:\033[m", x_end.min);
    }
    else{
        printf("\033[33m%d:\033[m", x_end.min);
    }

    if(x_end.sec<0){
        printf("\033[33m%d\033[m", 60+x_end.sec);
    }
    else if(x_end.sec<10){
        printf("\033[33m0%d\033[m", x_end.sec);
    }
    else{
        printf("\033[33m%d\033[m", x_end.sec);
    }

    printf("\033[5;11f");

    if(turn > 1){
        if(y_end.sec<0 && y_end.min<10){
            printf("\033[33m0%d:\033[m", y_end.min-1);
        }
        else if(y_end.min<10){
            printf("\033[33m0%d:\033[m", y_end.min);
        }
        else{
            printf("\033[33m%d:\033[m", y_end.min);
        }

        if(y_end.sec<0){
            printf("\033[33m%d\033[m", 60+y_end.sec);
        }
        else if(y_end.sec<10){
            printf("\033[33m0%d\033[m", y_end.sec);
        }
        else{
            printf("\033[33m%d\033[m", y_end.sec);
        }
    }
    else{
        printf("\033[33m00:00\033[m");   
    }

    printf("\033[25;0f");
    
}

// NODE *inverse(){
//     NODE *front, *fnext, *list, *tmp, *save_front;
//     list = stack;
//     front = NULL;

//     while(list){
//         fnext = front;
//         front = list;
//         list = list->right;
//         front->right = fnext;
//     }

//     return(front);
// }

// void inverse(){
//     NODE *tmp = NULL;
//     NODE *current = stack;

//     while(current){
//         tmp = current->left;
//         current->left = current->right;
//         current->right = tmp;
//         current = current->left;
//         stack = tmp;
//     }
    
//     // if(tmp != NULL){
//     //     stack = tmp->left;
//     // }

//     // return(stack);
// }

// void replay() /*印出開始後的所有步伐*/
// {
//     NODE *tmp;
//     int t = 0;
    
//     initial_checkerboard();

//     tmp = inverse();

//     do{
//         checkerboard[tmp->goal_row_memory][tmp->goal_column_memory] = checkerboard[tmp->initial_row_memory][tmp->initial_column_memory];
//         checkerboard[tmp->initial_row_memory][tmp->initial_column_memory] = 0;
//         printf("*****%d*****\n", t+1);
//         print_checkerboard();
//         tmp = tmp->right;
//         t++;
//     }while(tmp);
// }

void read_old_game(){
    int i;
    int old_year, old_month, old_day, old_hour, old_min;
    char chess[5];
    char str1[8], str2[8], str3[15];
    int initial_row = 0, initial_column = 0, goal_row = 0, goal_column = 0, chess_index = 0;
    char promotion;

    //fscanf(old_shogi_game, "開始時間: %d.%02d.%02d %02d:%02d\n", &old_year, &old_month, &old_day, &old_hour, &old_min);
    fscanf(old_shogi_game, "%3s%8s%15s\n",str1,str2,str3);
    
    initial_checkerboard();

    while(!feof(old_shogi_game)){

        fscanf(old_shogi_game, "%3d %d %d %s %d %d %c\n", &turn, &goal_column, &goal_row, chess, &initial_column, &initial_row, &promotion);

        //printf("%3d %d %d %d %d\n",turn, initial_column, initial_row, goal_column, goal_row);

        if(turn%2 == 1 && checkerboard[goal_row][BOARDCOLUMN-goal_column-1] < 0){
            X_chess_top ++;
            if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-9){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-1;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-10){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-2;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-11){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-3;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-12){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-8;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-13){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-6;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-14){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-7;
            }
            X.chess[X_chess_top] = -(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]);
        }

        else if(turn%2 == 0 && checkerboard[goal_row][BOARDCOLUMN-goal_column-1] > 0){
            Y_chess_top ++;
            if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==9){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=1;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==10){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=2;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==11){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=3;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==12){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=8;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==13){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=6;
            }

            else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==14){
                checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=7;
            }
            Y.chess[Y_chess_top] = -(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]);
        }

        if(initial_column != 9){
            checkerboard[goal_row][BOARDCOLUMN-goal_column-1] = checkerboard[initial_row][BOARDCOLUMN-initial_column-1];
            checkerboard[initial_row][BOARDCOLUMN-initial_column-1] = 0;

            if(promotion == 'Y'){
                if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==1){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=9;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==2){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=10;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==3){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=11;
                }
                
                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==6){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=13;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==7){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=14;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==8){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=12;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-1){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-9;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-2){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-10;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-3){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-11;
                }
                
                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-6){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-13;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-7){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-14;
                }

                else if(checkerboard[goal_row][BOARDCOLUMN-goal_column-1]==-8){
                    checkerboard[goal_row][BOARDCOLUMN-goal_column-1]=-12;
                }
            }
        }

        else if(turn%2 == 1){
            checkerboard[goal_row][BOARDCOLUMN-goal_column-1] = X.chess[initial_row];
            X.chess[initial_row] = 0;
            for(i=0; i< X_chess_top; i++){
                if(X.chess[i] == 0){
                    X.chess[i] = X.chess[i+1];
                    X.chess[i+1] = 0;
                }
            }
            X_chess_top--;
        }
        else if(turn%2 == 0){
            checkerboard[goal_row][BOARDCOLUMN-goal_column-1] = Y.chess[initial_row];
            Y.chess[initial_row] = 0;
            for(i=0; i< Y_chess_top; i++){
                if(Y.chess[i] == 0){
                    Y.chess[i] = Y.chess[i+1];
                    Y.chess[i+1] = 0;
                }
            }
            Y_chess_top--;
        }
        
        // push(0, 0, 0, call_back_row, BOARDCOLUMN-call_back_column-1, checkerboard[call_back_row][BOARDCOLUMN-call_back_column-1], X.chess, Y.chess);
        push(initial_row, BOARDCOLUMN-initial_column-1, checkerboard[initial_row][BOARDCOLUMN-initial_column-1], goal_row, BOARDCOLUMN-goal_column-1, checkerboard[goal_row][BOARDCOLUMN-goal_column-1], X.chess, Y.chess);
        
    }

    MAX_turn = top;
    fclose(old_shogi_game);

    review_old_game();

}

void review_old_game(){
    char step;
    int i;

    top = -1;
    turn = 1;

    // inverse();

    while(stack->right != NULL){
        // printf("%d %d %d\n",stack->right->initial_row_memory,stack->right->initial_column_memory,stack->right->goal_chess_memory);
        stack = stack->right;
    }
    
    review_mode = 1;
    
    initial_checkerboard();
    for(i=0; i<20; i++){
        X.chess[i] = X_chess_memory[top][i];
        Y.chess[i] = Y_chess_memory[top][i];
    }
    print_checkerboard();

    do{
        printf("\033[24;0f");
        printf("請輸入f(下一手)進行複盤(輸入s代表結束)\n");
        scanf("%c", &step);

        if(step == 'f' || step == 'F'){
            if(review_pop_forward()){
                turn++;
                print_checkerboard();
            }
        }

        // else if(step == 'b' || step == 'B'){
        //     if(review_pop_back()){
        //         turn--;
        //         print_checkerboard();
        //     }
        // }

    } while(step != 's' && step != 'S');

    printf("\033[24;0f");
    printf("\033[K");
    printf("複盤結束  \n");
    exit(0);
}

int review_pop_forward(){
    int i,j;
    int Xmax = -1, Ymax = -1;
    NODE *tmp;
    tmp=(NODE*)malloc(sizeof(NODE));

    if(top >= MAX_turn){
        printf("已到最後一手\n");
        top = MAX_turn;
        return FALSE;
    }
    else{
        top++;

        if(review_mode == 1){
            tmp = stack;
            if(stack->left != NULL){
                stack = stack->left;
            }
        }
        
        else if(review_mode == 0){
            tmp = stack->left;
            if(stack->left->left != NULL){
                stack = stack->left->left;
            }
            review_mode = 1;
        }

        printf("%d %d %d\n",tmp->initial_row_memory,tmp->initial_column_memory,tmp->goal_chess_memory);
        checkerboard[tmp->initial_row_memory][tmp->initial_column_memory] = tmp->initial_chess_memory;
        checkerboard[tmp->goal_row_memory][tmp->goal_column_memory] = tmp->goal_chess_memory;
        free(tmp);

        for(i=0; i<20; i++){
            X.chess[i] = X_chess_memory[top][i];
            Y.chess[i] = Y_chess_memory[top][i];
            if(X.chess[i] != 0){
                Xmax++;
            }
            if(Y.chess[i] != 0){
                Ymax++;
            }
        }
        X_chess_top = Xmax;
        Y_chess_top = Ymax;
        return TRUE;
    }
}

int review_pop_back(){
    int i,j;
    int Xmax = -1, Ymax = -1;
    NODE *tmp;
    tmp=(NODE*)malloc(sizeof(NODE));

    top--;

    if(isEmpty()){
        printf("已到第一手\n");
        top = -1;
        return FALSE;
    }
    else{
        if(review_mode == 1){
            printf("%d %d %d\n",stack->right->initial_row_memory,stack->right->initial_column_memory,stack->right->goal_chess_memory);
            tmp = stack->right;
            if(stack->right->right != NULL){
                stack = stack->right->right;
            }
            review_mode = 0;
        }

        else if(review_mode == 0){
            printf("%d %d %d\n",stack->initial_row_memory,stack->initial_column_memory,stack->goal_chess_memory);
            tmp = stack;
            if(stack->right != NULL){
                stack = stack->right;
            }
        }

        // printf("%d %d %d\n",tmp->initial_row_memory,tmp->initial_column_memory,tmp->goal_chess_memory);
        checkerboard[tmp->initial_row_memory][tmp->initial_column_memory] = tmp->goal_chess_memory;
        checkerboard[tmp->goal_row_memory][tmp->goal_column_memory] = tmp->initial_chess_memory;
        free(tmp);

        for(i=0; i<20; i++){
            X.chess[i] = X_chess_memory[top][i];
            Y.chess[i] = Y_chess_memory[top][i];
            if(X.chess[i] != 0){
                Xmax++;
            }
            if(Y.chess[i] != 0){
                Ymax++;
            }
        }
        X_chess_top = Xmax;
        Y_chess_top = Ymax;

        return TRUE;
    }
}