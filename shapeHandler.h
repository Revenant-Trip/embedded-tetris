#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>



/* Graphic library context */
Graphics_Context g_sContext;

#define width 5

struct Tetromino shapeLib(int shapeSelectVar, int rotation);
int** positionUpdater(int shapeSelectVar, int positionXY[], int rotation);
void drawShape(struct Tetromino shape, int postiontXY[]);
