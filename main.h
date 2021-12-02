#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;

#define width 5    //This is defined again in shapeHandler. effectively defeating the purpose.

// Same as standardRect
Graphics_Rectangle gridRect = {0, 0, 0 + width, 0 + width};   //Defines the standard size of each rectangle using the width parameter


void boardInit(int ***boards);
void gameState(int **board);
void clearLine(int **board, int lineClear);
void checkLine(int **board);
void freeBoard(int **board);
