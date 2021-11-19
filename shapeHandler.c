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

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <math.h>

#define width 5
int shapeSelect = 0;   //Test code to manually pick shapes
Graphics_Rectangle standardRect = {0, 0, 0 + width, 0 + width};


struct Tetromino {
    int matrix[5][5];    //All tetrominoes can fit into a 4x4 matrix, however the last two columns of row 5 contain the bounds for drawing the shape quickly to increase performance.
         //This goes from 0 to 3 and represents 90 degrees of clockwise rotation.
};

int positionXY[2] = {28, 4};
int rotation = 0;

/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];
int start_counter = 5*3*1000000;    // 5 seconds

struct Tetromino myShapeHandler(int shapeSelectVar)
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


void myDrawShape(int shapeSelectVar)
{
    struct Tetromino shape = myShapeHandler(shapeSelectVar);

    Graphics_Rectangle tempRect = {0, 0, 0, 0};

    int i, j;
    for(i = 0; i < shape.matrix[4][4]; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < shape.matrix[4][3]; j++)  // uses the recorded width in shape matrix
        {
            if(shape.matrix[i][j] == 1)
            {
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

void gameClockInit()
{
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_disableInterrupt(TIMER32_0_BASE);
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



    gameClockInit();

    //Making a simple grid. Will be removed in final iteration
    int i,j;

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_Rectangle backgroundSquare = {3, 3, 63, 123};
    Graphics_fillRectangle(&g_sContext, &backgroundSquare);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle tempRect;
    for(i = 0; i < 20; i++)   // uses the recorded height in shape matrix
    {
        for(j = 0; j < 10; j++)  // uses the recorded width in shape matrix
        {
            int xCord = ((width * j) + j) + 4;
            int yCord = ((width * i) + i) + 4;
            tempRect = (Graphics_Rectangle) {standardRect.xMin + xCord,
                standardRect.yMin + yCord, //+ positionXY[1],
                standardRect.xMax + xCord - 1, //+ positionXY[0],
                standardRect.yMax + yCord - 1}; //+ positionXY[1]};
            Graphics_fillRectangle(&g_sContext, &tempRect);
        }
    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);

    MAP_PCM_gotoLPM0();
    myDrawShape(shapeSelect);
    while(1)
    {
        static int gameClockTracker = 0;
        if(gameClockTracker == 0 && positionXY[1] < 105)
        {
            Timer32_haltTimer(TIMER32_0_BASE);
            Timer32_setCount(TIMER32_0_BASE, start_counter);
            Timer32_startTimer(TIMER32_0_BASE, true);

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            positionXY[1] += width + 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);
        }
        gameClockTracker = Timer32_getValue(TIMER32_0_BASE);

        if(positionXY[1] > 100)
        {
            positionXY[1] = 4;
            shapeSelect++;
            if(shapeSelect > 6)
                shapeSelect = 0;
        }

    }
}


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

        if(resultsBuffer[0] < 6000  && positionXY[0] > 6 && xJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            positionXY[0] -= width + 1;
            xJoystickLock = 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);
        }
        else if(resultsBuffer[0] > 10000 && positionXY[0] < 50 && xJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            positionXY[0] += width + 1;
            xJoystickLock = 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);
        }

        /*if(resultsBuffer[1] > 10000 && positionXY[1] > 25 && yJoystickLock == 0)     //Removed the players ability to move up.
        {
            positionXY[1] -= width + 1;
            yJoystickLock = 1;
        }
        else*/ if(resultsBuffer[1] < 6000 && positionXY[1] < 105 && yJoystickLock == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            positionXY[1] += width + 1;
            yJoystickLock = 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);

        }

        //Joystick DeBounce
        if(resultsBuffer[0] > 6000 && resultsBuffer[0] < 10000)
        {
            xJoystickLock = 0;
        }
        if(resultsBuffer[1] > 6000 && resultsBuffer[1] < 10000)
        {
            yJoystickLock = 0;
        }
        /* Determine if JoyStick button is pressed
        if (!(P4IN & GPIO_PIN1))
            r = 10;
        else
            r = 25;
        */

        if(~P3IN & 0x20 && buttonDebounce == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            rotation ++;
            if(rotation > 3)
                rotation = 0;
            buttonDebounce = 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);
        }
        else if(~P5IN & 0x02 && buttonDebounce == 0)
        {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
            myDrawShape(shapeSelect);

            rotation --;
            if(rotation < 0)
                rotation = 3;
            buttonDebounce = 1;

            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
            myDrawShape(shapeSelect);
        }

        if(P5IN & 0X02 && P3IN & 0x20)
            buttonDebounce = 0;
    }
}

