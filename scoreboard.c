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
// scoreboard.c - MSP-EXP432P401R + Educational Boosterpack MkII - Joystick
//
//          Displays lines cleared and handles next shape
//
//****************************************************************************

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <math.h>

/* Graphic library context */
Graphics_Context g_sContext;

struct Score {
    int nextBlock;
    int linesCleared;
};

/* Function: score_init
 * 
 * Prints score screen format and creates score struct to be used later with
 * the score_update function
 */

struct Score score_init(Graphics_Context* g_sContext, struct Score* s)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    
    // sets initial score data
    int n = rand() % 7;
    s->nextBlock = n;
    s->linesCleared = 0;

    // prints score data and format to LCD
    Graphics_drawString(&g_sContext, "Lines", -1, 80, 30, true);
    Graphics_drawString(&g_sContext, "Next Block", -1, 80, 60, true);

    return s;
}

/* Function: score_update
 * 
 * Clears score display, increments lines cleared and generates a new block,
 * then displays new score data
 *
 * Changes: int to void (in main: update struct linesCleared from global, update global nextBlock from struct)
 */

void score_update(Graphics_Context* g_sContext, struct Score* s)
{
    char buffer[10];        // string buffer for printing lines cleared
    int n = s->nextBlock;   // saves next block for later return
    
    // erases current score data from LCD
    sprintf(buffer, "%d", s->linesCleared);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext, buffer, -1, 80, 40, true);

    // need to be able to pass in indices to myDrawShape in order to print out next block
    // myDrawShape(s->nextBlock, x, y)
    // erases shape from scoreboard

    // updates lines cleared and next block
    int n = rand() % 7;

    // writes new score data to LCD
    sprintf(buffer, "%d", s->linesCleared);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&g_sContext, buffer, -1, 80, 40, true);

    // need to be able to pass in indices to myDrawShape in order to print out next block
    // myDrawShape(s->nextBlock, x, y)
    // writes new shape onto scoreboard
}
