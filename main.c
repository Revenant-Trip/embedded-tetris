/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII - Joystick
//
//          Displays raw 14-bit ADC measurements for X/Y axis of Joystick
//
//****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "shapeHandler.h"
#include <stdbool.h>

#define ROW 12
#define COL 21

/* ADC results buffer */
static uint16_t resultsBuffer[2];
int start_counter = 5*3*1000000;    // 5 seconds

int shapeSelect = 0;                //temporary variable that will handled by random number generator range from 0 to 6.
int positionXY[2] = {28, 4};        //Initial position of spawned pieces
int rotation = 0;                   //range from 0 to 3.

bool gameOver;
int collisionFlag;


//This game clock is used to continuously force the piece down.
void gameClockInit()
{
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_disableInterrupt(TIMER32_0_BASE);
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_disableInterrupt(TIMER32_1_BASE);
}

/*
 * Main function
 */
int main(void)
{

    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

    /* Configures Pin 6.0 and 4.4 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);


    gameClockInit();
    bool gameOver = false;
    int** boards[2];
    boardInit(boards);

    //Making a simple grid.

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_Rectangle backgroundSquare = {3, 3, 63, 123};
    Graphics_fillRectangle(&g_sContext, &backgroundSquare);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle tempRect;
    int i,j;
    for(i = 0; i < 20; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < 10; j++)  // uses the recorded width in shape matrix
        {
            int xCord = ((width * j) + j) + 4;
            int yCord = ((width * i) + i) + 4;
            tempRect = (Graphics_Rectangle) {gridRect.xMin + xCord,
                gridRect.yMin + yCord, //+ positionXY[1],
                gridRect.xMax + xCord - 1, //+ positionXY[0],
                gridRect.yMax + yCord - 1}; //+ positionXY[1]};
            Graphics_fillRectangle(&g_sContext, &tempRect);
        }
    }



    //Draws the first shape
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    drawShape(shapeSelect, positionXY, rotation);
    positionUpdater(shapeSelect, positionXY, rotation, boards[1]);


    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    MAP_PCM_gotoLPM0();

    while(gameOver == false)
    {
        //Hopefully this checks for collision inbetween every interrupt. Definetly cursed   #FIXME
        positionUpdater(shapeSelect, positionXY, rotation, boards[1]);
        collisionFlag = checkCollision(boards[1], boards[0]);

        static int gameClockTracker = 0;
        if(gameClockTracker == 0)
        {
            Timer32_haltTimer(TIMER32_0_BASE);
            Timer32_setCount(TIMER32_0_BASE, start_counter);
            Timer32_startTimer(TIMER32_0_BASE, true);

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            positionXY[1] += width + 1;

            positionUpdater(shapeSelect, positionXY, rotation, boards[1]);
            collisionFlag = checkCollision(boards[1], boards[0]);

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                positionXY[1] -= width + 1;

                // NEED TO UPDATE THE GAME BOARD AS THE SUM OF THE COLLISION MAP AND THE GAME BOARD   // #FIXME

                positionXY[0] = 28;
                positionXY[1] = 4;
                shapeSelect++;
                if(shapeSelect > 6)
                    shapeSelect = 0;
            }

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);

        }
        gameClockTracker = Timer32_getValue(TIMER32_0_BASE);
    }
}


//FIXME   Needs to have the ability to check for collision. Problem with passing boards[] into interrupt.
/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);


    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {

        static int xJoystickLock = 0;
        static int yJoystickLock = 0;
        static int buttonDebounce = 0;

        /* Store ADC14 conversion results */
    	resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
    	resultsBuffer[1] = ADC14_getResult(ADC_MEM1);


    	//Attempt to move piece to the left, check collision and draw result.
        if(resultsBuffer[0] < 6000 && xJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            positionXY[0] -= width + 1;
            xJoystickLock = 1;

            //updatePosition
            //checkCollision

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                positionXY[0] += width + 1;
            }

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);
        }

        //Attempt to move piece to right, check collision and draw result;
        else if(resultsBuffer[0] > 10000 && xJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            positionXY[0] += width + 1;
            xJoystickLock = 1;

            //updatePosition
            //checkCollision

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                positionXY[0] -= width + 1;
            }

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);
        }


        //Attempt to move piece down, check collision and draw result
        //Extended hold also increases clock update speed.
        if(resultsBuffer[1] < 6000 && yJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            positionXY[1] += width + 1;
            yJoystickLock = 1;

            //updatePosition
            //checkCollision

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                positionXY[1] -= width + 1;

                // NEED TO UPDATE THE GAME BOARD AS THE SUM OF THE COLLISION MAP AND THE GAME BOARD   // #FIXME

                positionXY[0] = 28;
                positionXY[1] = 4;
                shapeSelect++;
                if(shapeSelect > 6)
                    shapeSelect = 0;
            }


            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);
            start_counter = 1 * 3 * 1000000;      //Effectively speeds up the game timer while holding the joystick down.
        }

        //Joystick DeBounce
        if(resultsBuffer[0] > 6000 && resultsBuffer[0] < 10000)
        {
            xJoystickLock = 0;
        }

        if(resultsBuffer[1] > 6000 && resultsBuffer[1] < 10000)
        {
            yJoystickLock = 0;
            start_counter = 5 * 3 * 1000000;      //Sets the game speed back to the original time.
        }

        //Rotation handler using buttons.
        if(~P3IN & 0x20 && buttonDebounce == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            rotation ++;
            if(rotation > 3)
                rotation = 0;
            buttonDebounce = 1;

            //updatePosition
            //checkCollision

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                rotation --;
                if(rotation < 0)
                    rotation = 3;
            }

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);
        }
        else if(~P5IN & 0x02 && buttonDebounce == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            drawShape(shapeSelect, positionXY, rotation);

            rotation --;
            if(rotation < 0)
                rotation = 3;
            buttonDebounce = 1;

            //updatePosition
            //checkCollision

            if(collisionFlag == 1)
            {
                collisionFlag = 0;
                rotation ++;
                if(rotation > 3)
                    rotation = 0;
            }

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            drawShape(shapeSelect, positionXY, rotation);
        }

        if(P5IN & 0X02 && P3IN & 0x20)
            buttonDebounce = 0;
    }
}



//Initializes board by allocating memory for it
void boardInit(int*** boards){
    int i, j;

    //allocates memory for board
    int** board = (int**)malloc(sizeof(int*) * ROW);
    int** collisionMap = (int**)malloc(sizeof(int*) * ROW);

    for(i = 0; i < ROW; i++){
        board[i] = (int*)malloc(sizeof(int) * COL);
        collisionMap[i] = (int*)malloc(sizeof(int) * COL);
    }

    //Ensures both boards are all zeroes
    for(i = 0; i < ROW; i++){
        for(j = 0; j < COL; j++){
            board[i][j] = 0;
            collisionMap[i][j] = 0;
        }
    }

    // adds a boarder of 1s for the game board.
    for(i = 0; i < ROW; i++){
        board[i][21] = 1;
    }
    for(j = 0; j < COL; j++){
        board[0][j] = 1;
        board[11][j] = 1;
    }
    boards[0] = board;
    boards[1] = collisionMap;
}


/*
Checks if the game over condition is met, whether the top is filled up
*/
void gameState(int **board){
    int j;
    for(j=0; j<COL; j++){
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
    int i, j;

    for(j=0; j<COL; j++){
        board[lineClear][j] = 0;
    }
    for(i = lineClear-1; i > 0; i--){
        for(j = 0; j<COL; j++){
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
   int i, j;
   for(i=ROW-1; i>0; i--) {
          int numberOfOnes = 0;
      for(j=0;j<COL;j++) {
         if(board[i][j] == 1){
            numberOfOnes++;
            printf("number of 1's = %d\n",numberOfOnes); //for testing purposes
         }
      }
      if(numberOfOnes == COL){
            printf("line needs to be cleared\n"); //for testing purposes
        clearLine(board, i);
      }
   }

}

/*
Updates board after down colission
*/
void boardUpdater(int shapeSelectVar, int positionXY[], int rotation, int** board)
{
    struct Tetromino shape = shapeLib(shapeSelectVar, rotation);

    int i, j;
    
        }
    }

    for(i = 0; i < shape.matrix[4][4]; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < shape.matrix[4][3]; j++)  // uses the recorded width in shape matrix
        {
            if(shape.matrix[i][j] == 1)
            {
                //maps the shape onto the game board
                board[i + ((positionXY[0] - 4)/(width + 1))][j + ((positionXY[1]-4)/(width + 1))] = 1;
            }
        }
    }
}

/*
Draws board after a down colission resulting in at least one line clear
*/
void boardDraw(int** board)
{
    Graphics_Rectangle tempRect = {0, 0, 0, 0};

    int i, j;

    for(i = 0; i < ROW; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < COL; j++)  // uses the recorded width in shape matrix
        {
            if(board[i][j] == 1)
            {
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            }
            else
            {
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            }
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

/*
Frees allocated board from memory
*/
void freeBoard(int **board){
    int i;
    for(i = 0; i < ROW; i++){
        free(board[i]);
    }

 free(board);
}



