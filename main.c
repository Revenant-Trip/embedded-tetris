#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROW 10
#define COL 21

bool gameOver;

int **boardInit();

int main()
{

//Random tests I did
    bool gameOver = false;



   int **board= boardInit();
   printBoard(board);

   for(int j=0; j<COL; j++){
       board[9][j] = 1;
   }
     for(int j=0; j<COL; j++){
       board[8][j] = 1;
   }

       for(int j=0; j<COL-2; j++){
       board[7][j] = 1;
   }


    board[5][20] = 1 ;
    printBoard(board);
    checkLine(board);

    printBoard(board);

    board[0][5] = 1 ;

    gameState(board);









}
/*
Initializes board by allocating memory for it
*/
int **boardInit(){
    int i, j;


    //allocates memory for board
    int **board;
    board = malloc(sizeof(int*)* ROW);

    for(i = 0; i < ROW; i++){
        board[i] = malloc(sizeof(int*) * COL);
    }

    //assigns all board values to 0
    for(i = 0; i < ROW; i++){
        for(j = 0; j < COL; j++){
            board[i][j] = 0;
        }
    }

    gameOver = false;

    return board;
}

/*
Prints board

*/
void printBoard(int **board){

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            printf("%d ", board[i][j]);

        }
        printf("\n");
    }
    printf("\n");
}

/*
Checks if the game over condition is met, whether the top is filled up
*/
void gameState(int **board){
    for(int j=0; j<COL; j++){
        if(board[0][j] == 1){
              gameOver = true;
        printf("Game Over \n ");
        break;
        }
    }

}

/*
Clears line and shifts all blocks above down. Line to clear is given by checkLine function
*/
void clearLine(int **board, int lineClear){
    for(int j=0; j<COL; j++){
        board[lineClear][j] = 0;
    }
    for(int i = lineClear-1; i > 0; i--){
        for(int j = 0; j<COL; j++){
           if(board[i][j] == 1){
            board[i+1][j] = 1;
            board[i][j] = 0;
           }
        }
    }

}

/*

Checks if any line needs to be cleared and if so calls clearLine on them
*/
void checkLine(int **board){
 //  int numberOfOnes = 0;
   for(int i=ROW-1; i>0; i--) {
          int numberOfOnes = 0;
      for(int j=0;j<COL;j++) {
         if(board[i][j] == 1){
            numberOfOnes++;
            printf("number of 1's = %d\n",numberOfOnes);
         }
      }
      if(numberOfOnes == COL){
            printf("line needs to be cleared\n");
        clearLine(board, i);
      }
   }

}

