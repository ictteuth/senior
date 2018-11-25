/*
 * app1.c
 *
 *  Created on: Nov 25, 2018
 *      Author: Ian
 */

#include <msp430.h>
#include <msp430fr2355.h>

#include "app1.h"
#include "LED_control.h"
#include "display_control.h"


void initTimerB1()
{
    TB1CCTL0 |= CCIE;
    TB1CCR0 = 0x0FFF;  //timing
    TB1CTL |= TBSSEL__SMCLK | MC__UP | ID__8;

    xSpot = 0;
    ySpot = 5;

}


/*
 * Draws a 'circle' on the display
 * x and y range from 0-11
 * y range from 0-7
 * color is RED, GREEN, or BLUE
 */
void drawSpot(int x, int y, int brightness, int color)
{

    int xL = x-1;
    int xH = x+1;
    int yL = y-1;
    int yH = y+1;

    char rgb;
    switch (color)
    {
    case RED:
        rgb = brightness << 5;
        break;

    case GREEN:
        rgb = brightness << 2;
        break;

    case BLUE:
        rgb = (brightness/2);
        break;
    }

    int xPixel;
    int yPixel;

    for (xPixel = xL; xPixel <= xH; xPixel++)
    {
        for (yPixel = yL; yPixel <= yH; yPixel++)
        {
            changePixel(xPixel, yPixel, rgb);
        }
    }


}


#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_B1 (void)

{
    drawSpot(xSpot, ySpot, 5, RED);
    xSpot++;
    if (xSpot > 11)
    {
        xSpot = 0;
        ySpot++;
        if (ySpot > 11)
        {
            ySpot = 0;
            setSolidColor(0b00000001);
        }
    }
}

