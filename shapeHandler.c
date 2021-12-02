/*
 * shapeHandler - draws shapes and returns collision checking matrix #WIP
 *
 * For use add:
 *      #include "shapeHandler.h"
 *
 *      ...
 *
 *      collisionHandler(int shapeSelectVar, int positionXY[], int rotation);
 *
 * This will choose which shape is currently drawn and should automatically reset positionXY[] and rotation
 * when collision is detected and a new piece is spawned.
 *
 *  Created on: 11/14/22 (modified 11/30/22)
 *      Author: Kenneth J. Rudnicki
 */

#include "shapeHandler.h"

Graphics_Rectangle standardRect = {0, 0, 0 + width, 0 + width};   //Defines the standard size of each rectangle using the width parameter

//This struct may not be necessary, but it works
struct Tetromino {
    int matrix[5][5];    //All tetrominoes can fit into a 4x4 matrix, however the last two columns of row 5 contain the bounds for drawing the shape quickly to increase performance.
    int positionMap[12][22];
};


//Uses the shapeLib() function, this program draws a single rectangle for each value of 1 in the shape matrix.
//Temporarily references the shapeSelect defined in main.c
void drawShape(int shapeSelectVar, int positionXY[], int rotation)
{
    struct Tetromino shape = shapeLib(shapeSelectVar, rotation);

    Graphics_Rectangle tempRect = {0, 0, 0, 0};

    int i, j;

    for(i = 0; i < shape.matrix[4][4]; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < shape.matrix[4][3]; j++)  // uses the recorded width in shape matrix
        {
            if(shape.matrix[i][j] == 1)
            {
                //draws the graphical shape
                int xCord = ((width * j) + j) + positionXY[0];
                int yCord = ((width * i) + i) + positionXY[1];
                tempRect = (Graphics_Rectangle) {standardRect.xMin + xCord,
                                                 standardRect.yMin + yCord, //+ positionXY[1],
                                                 standardRect.xMax + xCord - 1, //+ positionXY[0],
                                                 standardRect.yMax + yCord - 1}; //+ positionXY[1]};
                Graphics_fillRectangle(&g_sContext, &tempRect);
            }
        }
    }
}

//#FIXME   There is an issue with passing the collisionMapPtr data and as a result all collisions are broken.
void positionUpdater(int shapeSelectVar, int positionXY[], int rotation, int** collisionMap)
{
    struct Tetromino shape = shapeLib(shapeSelectVar, rotation);

    int i, j;

    //Clears the positionMap so it can be updated.
    for(i = 0; i < 12; i++)
    {
        for(j = 0; j < 22; j++)
        {
            collisionMap[i][j] = 0;
        }
    }

    for(i = 0; i < shape.matrix[4][4]; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < shape.matrix[4][3]; j++)  // uses the recorded width in shape matrix
        {
            if(shape.matrix[i][j] == 1)
            {
                //maps the shape onto an invisible board to compare with the main game board
                collisionMap[i + ((positionXY[0] - 4)/(width + 1))][j + ((positionXY[1]-4)/(width + 1))] = 1;
            }
        }
    }
}


//Returns a 1 on collision.  Written by Nick
int checkCollision(int **board, int **collisionMap){
    int i, j;

     for(i = 0; i < 12; i++){
        for(j = 0; j < 21; j++){
            if((board[i][j] == 1) && (collisionMap[i][j] == 1)){
                return 1;
            }
        }
     }
     return 0;
}


//This is a library of all the shapes and their rotational possibilities
struct Tetromino shapeLib(int shapeSelectVar, int rotation)
{
    int i, j;
    struct Tetromino shape;

    //S-BLOCK
    if(shapeSelectVar == 6)
    {
        if(rotation == 0 || rotation == 2)
        {
            int sBlock[5][5] = {
                                {0, 1, 1, 0, 0},
                                {1, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = sBlock[i][j];
            }
        }
        else if(rotation == 1 || rotation == 3)
        {
            int sBlock[5][5] = {
                                {1, 0, 0, 0, 0},
                                {1, 1, 0, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = sBlock[i][j];
            }
        }
    }


    //Z-BLOCK
    else if(shapeSelectVar == 5)
    {
        if(rotation == 0 || rotation == 2)
        {
            int zBlock[5][5] = {
                                {1, 1, 0, 0, 0},
                                {0, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = zBlock[i][j];
            }
        }
        else if (rotation == 1 || rotation == 3)
        {
            int zBlock[5][5] = {
                                {0, 1, 0, 0, 0},
                                {1, 1, 0, 0, 0},
                                {1, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = zBlock[i][j];
            }
        }
    }



    //L-Block
    else if(shapeSelectVar == 4) //L-block
    {
        if(rotation == 0)
        {
            int lBlock[5][5] = {
                                {1, 0, 0, 0, 0},
                                {1, 0, 0, 0, 0},
                                {1, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = lBlock[i][j];
            }
        }
        else if(rotation == 1)
        {
            int lBlock[5][5] = {
                                {1, 1, 1, 0, 0},
                                {1, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = lBlock[i][j];
            }
        }
        else if(rotation == 2)
        {
            int lBlock[5][5] = {
                                {1, 1, 0, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = lBlock[i][j];
            }
        }
        else if(rotation == 3)
        {
            int lBlock[5][5] = {
                                {0, 0, 1, 0, 0},
                                {1, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = lBlock[i][j];
            }
        }
    }



    //J-Block
    else if(shapeSelectVar == 3) //J-Block
    {
        if(rotation == 0)
        {
            int jBlock[5][5] = {
                                {0, 1, 0, 0, 0},
                                {0, 1, 0, 0, 0},
                                {1, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = jBlock[i][j];
            }
        }
        else if(rotation == 1)
        {
            int jBlock[5][5] = {
                                {1, 0, 0, 0, 0},
                                {1, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };

            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = jBlock[i][j];
            }
        }
        else if(rotation == 2)
        {
            int jBlock[5][5] = {
                                {1, 1, 0, 0, 0},
                                {1, 0, 0, 0, 0},
                                {1, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 2, 3}
            };

            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = jBlock[i][j];
            }
        }
        else if(rotation == 3)
        {
            int jBlock[5][5] = {
                                {1, 1, 1, 0, 0},
                                {0, 0, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 2}
            };

            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = jBlock[i][j];
            }
        }
    }



    //T-Block
    else if(shapeSelectVar == 2)
    {
        if(rotation == 0)
        {
            int tBlock[5][5] = {
                                {0, 1, 0, 0, 0},
                                {1, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = tBlock[i][j];
            }
        }
        else if(rotation == 1)
        {
            int tBlock[5][5] = {
                                {0, 1, 0, 0, 0},
                                {0, 1, 1, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = tBlock[i][j];
            }
        }
        else if(rotation == 2)
        {
            int tBlock[5][5] = {
                                {0, 0, 0, 0, 0},
                                {1, 1, 1, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = tBlock[i][j];
            }
        }
        else if(rotation == 3)
        {
            int tBlock[5][5] = {
                                {0, 1, 0, 0, 0},
                                {1, 1, 0, 0, 0},
                                {0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 0, 0, 3, 3}
            };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = tBlock[i][j];
            }
        }
    }


    // Square
    else if(shapeSelectVar == 1) //square
    {
        int square[5][5] = {
                            {1, 1, 0, 0, 0},
                            {1, 1, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 2, 2}
        };

        for(i = 0; i < 5; i ++)
        {
            for(j = 0; j < 5; j ++)
                shape.matrix[i][j] = square[i][j];
        }
    }

    // Line
    else
    {
        if(rotation == 0 || rotation == 2)
        {
            int line[5][5] = {
                              {1, 0, 0, 0, 0},
                              {1, 0, 0, 0, 0},
                              {1, 0, 0, 0, 0},
                              {1, 0, 0, 0, 0},
                              {0, 0, 0, 1, 4}
                    };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = line[i][j];
            }
        }
        else if(rotation == 1 || rotation == 3)
        {
            int line[5][5] = {
                              {1, 1, 1, 1, 0},
                              {0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0},
                              {0, 0, 0, 4, 1}
                    };
            for(i = 0; i < 5; i ++)
            {
                for(j = 0; j < 5; j ++)
                    shape.matrix[i][j] = line[i][j];
            }
        }
    }
    return shape;
}
