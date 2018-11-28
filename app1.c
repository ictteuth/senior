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
#include "antenna_control.h"
#include "adc.h"

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
    float xAvg = 0;
    float yAvg = 0;
    float zAvg = 0;

    int xInt = 0;
    int yInt = 0;
    int zInt = 0;

    while (1)
    {

        switch(mode)
        {
        case 0:
            if (isNewMode == 1)
            {
                isNewMode = 0;
                setSolidColor(0b00000001);
            }
            while (!positionReady); //wait for position data
            //disregard outliers

            //moving average position data for smoothing
            xAvg = 0.6*xAvg + 0.4*xCurrent;
            yAvg = 0.6*yAvg + 0.4*(yCurrent-100);
            zAvg = 0.7*zAvg + (0.3*(zCurrent-300));

            //round values
            xInt = (int)(xAvg + 0.5);
            yInt = (int)(yAvg + 0.5);
            zInt = (int)(zAvg*0.04);
            if (zInt > 7)
            {
                zInt = 7;
            }
            else if (zInt < 1)
            {
                zInt = 1;
            }



            //clear screen and draw new square
            setSolidColor(0b00000001);  //dim blue
            drawSpot(xInt*0.04 + 0, yInt*0.015, 8-zInt, RED); //x testing
    //        drawSpot(5, yInt/30, 7, RED); //y testing

            positionReady = 0;

            break;

        case 1:
            if (isNewMode)
            {
                isNewMode = 0;
                setSolidColor(0b00000100);
            }
            break;

        case 2:
            if (isNewMode)
            {
                //reinitialize variables
                xSpot = 1;
                ySpot = 1;
                bright = 0;
                aColor = RED;
                isNewMode = 0;
            }
            break;

        }//switch(mode)
//        while (!positionReady); //wait for position data
        //disregard outliers

/*
        //moving average position data for smoothing
        xAvg = 0.6*xAvg + 0.4*xCurrent;
        yAvg = 0.6*yAvg + 0.4*(yCurrent-100);
        zAvg = 0.7*zAvg + (0.3*(zCurrent-300));

        //round values
        xInt = (int)(xAvg + 0.5);
        yInt = (int)(yAvg + 0.5);
        zInt = (int)(zAvg*0.04);
        if (zInt > 7)
        {
            zInt = 7;
        }
        else if (zInt < 1)
        {
            zInt = 1;
        }



        //clear screen and draw new square
        setSolidColor(0b00000001);  //dim blue
        drawSpot(xInt*0.04 + 0, yInt*0.015, 8-zInt, RED); //x testing
//        drawSpot(5, yInt/30, 7, RED); //y testing

        positionReady = 0;
        */
    }

}



void initTimerB1()
{
    TB1CTL |= BIT2; //Clear some stuff

    TB1CCTL0 |= CCIE;
    TB1CCR0 = 0x7FFF;  //timing 9fff
    TB1CTL |= TBSSEL__SMCLK | MC__UP | ID__8;

    xSpot = 1;
    ySpot = 1;
    bright = 0;
    aColor = RED;
    anAxis = 0;

    /****************************************************************************************************************************************
    ***********************************************TESTINIG OP AMP CREATURE OF THE DARKNESS**************************************************
    *****************************************************************************************************************************************/
    P2DIR |= BIT0 | BIT1 | BIT2;    //outputs controlling op amp
    opAmpCreatureCounter = 10;
    oacc2 = 0;
    activateX();

    adc_init();
    /*****************************************************************************************************************************************/
    /*****************************************************************************************************************************************/

    mode = 0;
    isNewMode = 1;
    switchDelay = 50;

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


#pragma vector = PORT2_VECTOR
__interrupt void Button (void)
{
    mode++;
    if (mode > 2)
    {
        mode = 0;
    }
    isNewMode = 1;

    P2IFG &= ~BIT6;
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_B1 (void)

{
    //    drawSpot(xSpot, ySpot, bright>>1, aColor);
/*
    if (switchDelay == 0)
    {
        if (P2IN & BIT6 == 0)
        {
            mode++;
            if (mode > 2)
            {
                mode = 0;
            }
            switchDelay = 50;
            isNewMode = 1;
        }
    }
    else
    {
        switchDelay--;
    }
*/
    switch(mode)
    {
    case 0:     //basic
        switch (anAxis)
        {
        case 0:
            xCurrent = find_x();
            activateY();
            anAxis = 1;
            break;
        case 1:
            yCurrent = find_y();
            activateZ();
            anAxis = 2;
            break;
        case 2:
            zCurrent = find_z();
            activateX();
            anAxis = 0;
            positionReady = 1;
            break;
        }

        break;
    case 1:     //game
        switch (anAxis)
        {
        case 0:
            xCurrent = find_x();
            activateY();
            anAxis = 1;
            break;
        case 1:
            yCurrent = find_y();
            activateZ();
            anAxis = 2;
            break;
        case 2:
            zCurrent = find_z();
            activateX();
            anAxis = 0;
            positionReady = 1;
            break;
        }


        break;

    case 2:     //pattern
        positionReady = 1;

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

        break;
    }

//    switch (anAxis)
//    {
//    case 0:
//        xCurrent = find_x();
//        activateY();
//        anAxis = 1;
//        break;
//    case 1:
//        yCurrent = find_y();
//        activateZ();
//        anAxis = 2;
//        break;
//    case 2:
//        zCurrent = find_z();
//        activateX();
//        anAxis = 0;
//        positionReady = 1;
//        break;
//    }

//    xCurrent = find_x();
//    yCurrent = find_y();
//    zCurrent = find_z();
//    positionReady = 1;

    oacc2++;
    /************************************************************************************************************************************/
//    opAmpCreatureCounter--;
//    if (opAmpCreatureCounter == 0)
//    {
//        switch(oacc2)
//        {
//        case 0:
//            oacc2 = 1;
//            P2OUT &= ~BIT2;
//            P2OUT |= BIT0;
//            break;
//
//        case 1:
//            oacc2 = 2;
//            P2OUT &= ~BIT0;
//            P2OUT |= BIT1;
//            break;
//
//        case 2:
//            oacc2 = 0;
//            P2OUT &= ~BIT1;
//            P2OUT |= BIT2;
//            break;
//        }
//        opAmpCreatureCounter = 100;
//    }

    /************************************************************************************************************************************/
//    bright++;
//    if (bright > 15)
//    {
//        bright = 0;
//        ySpot += 3;
//        if (aColor == RED)
//            {
//                aColor = GREEN;
//            }
//            else if (aColor == GREEN)
//            {
//                aColor = BLUE;
//            }
//            else
//            {
//                aColor = RED;
//            }
//        if (ySpot > 11)
//        {
//            ySpot = 1;
//            xSpot += 3;
//            if (xSpot > 11)
//            {
//                xSpot = 1;
//                setSolidColor(0b11111111);
//            }
//        }
//    }
}

