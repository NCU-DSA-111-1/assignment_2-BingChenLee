# Data Structure Assignment 2

**Subject**: 日本將棋對弈程式  
**Author**: 通訊二 110503518 李秉宸  
**Version**: v2.0  
**Last Updated**: 2022.11.11  

## Make File & Run

```sh
# Make File
make
gcc main.c -g   -pthread -lncurses -lm -o main 
cc    -c -o main.o main.c

# Run new game
./main -n -s NEW_GAME_FILE_NAME.txt

# Read old game
./main -l OLD_GAME_FILE_NAME.txt
```
Notice that the **NEW_GAME_FILE_NAME** must be the same as **OLD_GAME_FILE_NAME** so that the program can find the file.

## Usage 1 - Play new game
```sh
# Enter the origin column and row, goal column and row of the piece you want to move
玩家X請輸入要移動將棋的 原始座標 與 目的座標
7 7 7 6

玩家Y請輸入要移動將棋的 原始座標 與 目的座標
3 3 3 4

# If the piece can promote, the player can enter "Y" or "N" to decide the state.
玩家X請輸入要移動將棋的 原始座標 與 目的座標
8 8 2 2
可進行升變，是否進行升變[Y/N]
y
```
You can type **-1** in the terminal to stop and save the record.

## Usage 2 - Read old game
```sh
# Enter "f" to check the checkerboard
請輸入f(下一手)進行複盤(輸入s代表結束)
f 

# Enter "s" to end the game
請輸入f(下一手)進行複盤(輸入s代表結束)
s
```
If there's no more piece can forward, the program will deny your request.  
Since there still have some problem about back, I prohibited the users to enter "b" in this version.

## Usage 3 - Call back
```sh
# Enter "10" to call back the piece
玩家X請輸入要移動將棋的 原始座標 與 目的座標
10
請選擇要打入的棋子號碼(1~2)
1
請輸入要打入的位置
4 5
```
If there's no more piece can call back, the program will deny your request.

## Usage 4 - Regretion and end game
```sh
# Enter "0" to regret the piece
玩家X請輸入要移動將棋的 原始座標 與 目的座標
0

# Enter "-1" to end the game
玩家X請輸入要移動將棋的 原始座標 與 目的座標
-1
對局過程已記錄完成
```
If there's no more piece can regret, the program will deny your request.

## Promotion

```sh
升變將棋規則說明
步兵(步) -> 成步(と) 走法與金將相同
香車(香) -> 成香(杏) 走法與金將相同
桂馬(桂) -> 成桂(圭) 走法與金將相同
銀將(銀) -> 成銀(全) 走法與金將相同
飛車(飛) -> 龍王(竜) 原十字走法+自身周圍八格
角行(角) -> 龍馬(馬) 原對角走法+自身周圍八格
金將(金) 無法成變
王將(王) 無法成變
```

## Version

```sh
v1.0 以陣列型態完成基本規則判定、持駒、持駒打入、升變、悔棋、棋譜紀錄、棋譜讀取與基本計時器。
v1.1 修復攻方可移動敵方部分棋子的問題。
v2.0 將對局過程改以Linked list存取。
```

## Reference
**[1] 日本將棋走法玩法**
> https://shogi.hk/Gameplay-of-Japanese-Chess-Shogi/

**[2] C語言tips：帶顏色的輸出**
> https://www.796t.com/article.php?id=190246

**[3] Cursor Movement**
> https://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html

**[4] getopt函式範例**
> https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html