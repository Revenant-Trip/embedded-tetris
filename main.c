#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROW 21
#define COL 12



bool gameOver;

int **boardInit();

int linesCleared = 0;

int main()
{

//Random tests I did
    bool gameOver = false;



   int **board= boardInit();


        for(int j = 0; j < COL-2; j++){
           board[ROW-3][j] = 1;
        }

        for(int j = 0; j < COL-1; j++){
           board[ROW-2][j] = 1;
        }




   printBoard(board);

   checkLine(board);

      printBoard(board);


    gameState(board);

    board[0][5] = 1;

      gameState(board);

    free(board);

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
        board[i][0] = 1;
        board[i][11] = 1;
    }
    board[0][0] = 0;
    board[0][11] = 0;

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
    for(int j=1; j<COL-1; j++){
        if(board[0][j] == 1){ //checks if there is a block in the first row, if so game is over
              gameOver = true;
        printf("Game Over \n "); //for testing purposes
        freeBoard(board); //frees board when game over
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
      for(int j=1;j<COL-1;j++) {
         if(board[i][j] == 1){
            numberOfOnes++;
       //     printf("number of 1's = %d\n",numberOfOnes); //for testing purposes
         }
      }
      if(numberOfOnes == COL){
            printf("line needs to be cleared\n"); //for testing purposes
        clearLine(board, i);
        linesCleared++;
      }
   }

}

/*
Frees allocated board from memory
*/
void freeBoard(int **board){
 for(int i = 0; i < ROW; i++){
        free(board[i]);
    }

 free(board);
}

