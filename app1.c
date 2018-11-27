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

/*
 * Main loop for program 1
 * called from main() after menu selection
 *
 * Uses time b1 to take measurements of position
 * draws square under wand
 * brightness varies with height
 *
 *
 */
void program1MainLoop()
{
    float xAvg;
    float yAvg;
    float zAvg;

    int xInt;
    int yInt;
    int zInt;

    while (1)
    {
        while (!positionReady); //wait for position data
        //disregard outliers

        //moving average position data for smoothing
        xAvg = 0.5*xAvg + 0.5*xCurrent;
        yAvg = 0.5*yAvg + 0.5*yCurrent;
        zAvg = 0.5*zAvg + 0.5*zCurrent;

        //round values
        xInt = (int)(xAvg + 0.5);
        yInt = (int)(yAvg + 0.5);
        zInt = (int)(zAvg + 0.5);


        //clear screen and draw new square
        setSolidColor(0b00000001);  //dim blue
        drawSpot(xAvg, yAvg, zAvg, RED);
    }

}



void initTimerB1()
{
    TB1CTL |= BIT2; //Clear some stuff

    TB1CCTL0 |= CCIE;
    TB1CCR0 = 0xFFFF;  //timing 9fff
    TB1CTL |= TBSSEL__SMCLK | MC__UP | ID__8;

    xSpot = 1;
    ySpot = 1;
    bright = 0;
    aColor = RED;

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
    drawSpot(xSpot, ySpot, bright>>1, aColor);

    bright++;
    if (bright > 15)
    {
        bright = 0;
        ySpot += 3;
        if (aColor == RED)
            {
                aColor = GREEN;
            }
            else if (aColor == GREEN)
            {
                aColor = BLUE;
            }
            else
            {
                aColor = RED;
            }
        if (ySpot > 11)
        {
            ySpot = 1;
            xSpot += 3;
            if (xSpot > 11)
            {
                xSpot = 1;
                setSolidColor(0b11111111);
            }
        }
    }
}

