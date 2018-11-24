/*
 * LED_control.c
 *
 *  Created on: Nov 19, 2018
 *      Author: Ian
 */

#include <msp430.h>
#include <msp430fr2355.h>
#include "LED_control.h"



void initLED()
{
    TB0CTL |= BIT2; //Clear some stuff

    /******* FOR TESTING PURPOSES *********/
    P2DIR |= BIT2;  //Set pin to output (pin 28)
    P2OUT |= BIT2;  //Turn on LED
    /**************************************/

    //row control lines as outputs
    //set port 3 0-7 to output
    //set port 5 1-4 to output
    P3DIR |= 0xFF;
    P5DIR |= BIT1 | BIT2 | BIT3 | BIT4;

    //data lines and select lines as outputs
    P2DIR |= BIT4 | BIT5;
    P4DIR |= BIT0 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
    P6DIR |= BIT6;

    //disable decoder output
    P4OUT &= ~BIT3;

    PM5CTL0 &= ~LOCKLPM5;

    TB0CCTL0 |= CCIE;
    TB0CCR0 = 0x00FF;  //timing
    TB0CTL |= TBSSEL__SMCLK | MC__UP | ID__8;

    __bis_SR_register(GIE);

    currentRow = 0;
    currentMask = MASK0;

    int x,y;

    for (x = 0; x < 12; x++)
    {
        for (y = 0; y < 36; y++)
        {
            changePixel(x, y, 0b01100011);
        }
    }

    changePixel(2, 0, 0b00000011);
    changePixel(2, 1, 0b00000001);
    changePixel(2, 2, 0b00011100);

    changePixel(0, 0, 0b00000011);
    changePixel(0, 1, 0b00000100);
    changePixel(0, 2, 0b00111100);


}

/*
 * Updates the pixel at coordinates (x,y) to have the value specified by rgb.
 * modifies imgArr.
 */
void changePixel(int x, int y, char rgb)
{
    char* r = &imgArr[y][3*x];
    char* g = r+1;
    char* b = r+2;

    char val;

    //red
    switch ((rgb & 0b11100000) >> 5)
    {
    case 0b000:
        val = 0b00000000;
        break;

    case 0b001:
        val = 0b00000010;
        break;

    case 0b010:
        val = 0b00000110;
        break;

    case 0b011:
        val = 0b00001110;
        break;

    case 0b100:
        val = 0b00011110;
        break;

    case 0b101:
        val = 0b00111110;
        break;

    case 0b110:
        val = 0b01111110;
        break;

    case 0b111:
        val = 0b11111110;
        break;
    }
    *r = val;


    //green
    switch ((rgb & 0b00011100) >> 2)
    {
    case 0b000:
        val = 0b00000000;
        break;

    case 0b001:
        val = 0b00000010;
        break;

    case 0b010:
        val = 0b00000110;
        break;

    case 0b011:
        val = 0b00001110;
        break;

    case 0b100:
        val = 0b00011110;
        break;

    case 0b101:
        val = 0b00111110;
        break;

    case 0b110:
        val = 0b01111110;
        break;

    case 0b111:
        val = 0b11111110;
        break;
    }
    *g = val;


    //blue
    switch (rgb & 0b00000011)
    {
    case 0b00:
        val = 0b00000000;
        break;

    case 0b01:
        val = 0b00000110;
        break;

    case 0b10:
        val = 0b00011110;
        break;

    case 0b11:
        val = 0b01111110;
        break;
    }
    *b = val;

}


/*
 * Sends active high signal to turn on the next column as determined from
 * the currentCol global variable
 *
 *    ROW       PIN
 *    0         5.4
 *    1         5.3
 *    2         3.7
 *    3         3.6
 *    4         3.5
 *    5         3.4
 *    6         5.2
 *    7         3.3
 *    8         3.2
 *    9         3.1
 *    10        3.0
 *    11        5.1
 */
void nextRow()
{
    //turn off power to all rows
    P3OUT &= 0xFF;
    P5OUT &= (BIT1 | BIT2 | BIT3 | BIT4);

    //prepare the next row
    currentRow++;
    if (currentRow > 11) currentRow = 0;

    //update all columns to the current row
    currentMask = MASK0;
    updateColumns(currentRow, currentMask);

    //turn on power to the current row
    switch(currentRow)
    {
    case 0:
        P5OUT &= ~BIT1;
        P5OUT |= BIT4;
        break;

    case 1:
        P5OUT &= ~BIT4;
        P5OUT |= BIT3;
        break;

    case 2:
        P5OUT &= ~BIT3;
        P3OUT |= BIT7;
        break;

    case 3:
        P3OUT &= ~BIT7;
        P3OUT |= BIT6;
        break;

    case 4:
        P3OUT &= ~BIT6;
        P3OUT |= BIT5;
        break;

    case 5:
        P3OUT &= ~BIT5;
        P3OUT |= BIT4;
        break;

    case 6:
        P3OUT &= ~BIT4;
        P5OUT |= BIT2;
        break;

    case 7:
        P5OUT &= ~BIT2;
        P3OUT |= BIT3;
        break;

    case 8:
        P3OUT &= ~BIT3;
        P3OUT |= BIT2;
        break;

    case 9:
        P3OUT &= ~BIT2;
        P3OUT |= BIT1;
        break;

    case 10:
        P3OUT &= ~BIT1;
        P3OUT |= BIT0;
        break;

    case 11:
        P3OUT &= ~BIT0;
        P5OUT |= BIT1;
        break;
    }

}

/*
 * called by a timer interrupt to start updating a row with the next mask
 */
void updateISR()
{
    currentMask <<= 1;
    if (currentMask == 0)
    {
        nextRow();
    }
    else
    {
        updateColumns(currentRow, currentMask);
    }
}

/*
 * Updates all columns with the proper values for the given row
 */
void updateColumns(int row, char mask)
{
    volatile int i;


    updateCol0_4(row, mask);

    //set select lines to 000
    P4OUT &= ~(BIT7 | BIT5 | BIT6);

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol5_9(row, mask);

    //set select lines to 001
    P4OUT ^= BIT6;

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol10_14(row, mask);

    //set select lines to 010
    P4OUT ^= (BIT5 | BIT6);

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol15_17(row, mask);

    /***************************************************************************************************************************/
//    P4OUT |= BIT4;
//    P2OUT &= ~BIT4;
//    P6OUT &= ~BIT6;
    /***************************************************************************************************************************/


    //set select lines to 011
    P4OUT ^= BIT6;

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol18_22(row, mask);

    //set select lines to 100
    P4OUT ^= (BIT7 | BIT5 | BIT6);

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol23_27(row, mask);

    //set select lines to 101
    P4OUT ^= BIT6;

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol28_32(row, mask);

    //set select lines to 110
    P4OUT ^= (BIT5 | BIT6);

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;


    updateCol33_35(row, mask);

    //set select lines to 111
    P4OUT ^= BIT6;

    //enable output
    P4OUT |= BIT3;

    //small delay
    i = 0;
    do {i++;} while (i < 10);

    //disable output
    P4OUT &= ~BIT3;

}

/*
 * Sends the proper outputs to columns 0 through 4 for the given row
 */
void updateCol0_4(int row, char mask)
{
    char* arrayPtr = &imgArr[row][0];

    //D0
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT4;
    }
    else
    {
        P4OUT |= BIT4;
    }

    //D1
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT4;
    }
    else
    {
        P2OUT |= BIT4;
    }

    //D2
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P6OUT &= ~BIT6;
    }
    else
    {
        P6OUT |= BIT6;
    }

    //D3
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT5;
    }
    else
    {
        P2OUT |= BIT5;
    }

    //D4
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT0;
    }
    else
    {
        P4OUT |= BIT0;
    }

}

/*
 * Sends the proper outputs to columns 5 through 9 for the given row
 */
void updateCol5_9(int row, char mask)
{
    char* arrayPtr = &imgArr[row][5];

    //D0
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT4;
    }
    else
    {
        P4OUT |= BIT4;
    }

    //D1
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT4;
    }
    else
    {
        P2OUT |= BIT4;
    }

    //D2
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P6OUT &= ~BIT6;
    }
    else
    {
        P6OUT |= BIT6;
    }

    //D3
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT5;
    }
    else
    {
        P2OUT |= BIT5;
    }

    //D4
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT0;
    }
    else
    {
        P4OUT |= BIT0;
    }


}

/*
 * Sends the proper outputs to columns 10 through 14 for the given row
 */
void updateCol10_14(int row, char mask)
{
    char* arrayPtr = &imgArr[row][10];

    //D0
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT4;
    }
    else
    {
        P4OUT |= BIT4;
    }

    //D1
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT4;
    }
    else
    {
        P2OUT |= BIT4;
    }

    //D2
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P6OUT &= ~BIT6;
    }
    else
    {
        P6OUT |= BIT6;
    }

    //D3
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT5;
    }
    else
    {
        P2OUT |= BIT5;
    }

    //D4
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT0;
    }
    else
    {
        P4OUT |= BIT0;
    }


}

/*
 * Sends the proper outputs to columns 15 through 17 for the given row
 */
void updateCol15_17(int row, char mask)
{
    char* arrayPtr = &imgArr[row][15];

    //D0
    if ((*arrayPtr & mask) == 0)
    {
        P4OUT &= ~BIT4;
    }
    else
    {
        P4OUT |= BIT4;
    }

    //D1
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P2OUT &= ~BIT4;
    }
    else
    {
        P2OUT |= BIT4;
    }

    //D2
    arrayPtr++;
    if ((*arrayPtr & mask) == 0)
    {
        P6OUT &= ~BIT6;
    }
    else
    {
        P6OUT |= BIT6;
    }

}

/*
 * Sends the proper outputs to columns 18 through 22 for the given row
 */
void updateCol18_22(int row, char mask)
{

}

/*
 * Sends the proper outputs to columns 23 through 27 for the given row
 */
void updateCol23_27(int row, char mask)
{

}

/*
 * Sends the proper outputs to columns 28 through 32 for the given row
 */
void updateCol28_32(int row, char mask)
{

}

/*
 * Sends the proper outputs to columns 33 through 35 for the given row
 */
void updateCol33_35(int row, char mask)
{

}



//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
//#else
//#error Compiler not supported!
//#endif
{
    P2OUT ^= BIT2;
    updateISR();
}

