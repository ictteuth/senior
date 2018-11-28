/*
 * antenna_control.c
 *
 *  Created on: Nov 27, 2018
 *      Author: ictteuth
 */

#include "antenna_control.h"
#include <msp430.h>
#include <msp430fr2355.h>


void activateX()
{
    P2OUT |= BIT0;
    P2OUT &= ~(BIT1 | BIT2);

}

void activateY()
{
    P2OUT |= BIT1;
    P2OUT &= ~(BIT0 | BIT2);

}

void activateZ()
{
    P2OUT |= BIT2;
    P2OUT &= ~(BIT0 | BIT1);

}



