/*
 * adc.c
 *
 *  Created on: Nov 26, 2018
 *      Author: Ian
 */

#include <msp430.h>
#include <msp430fr2355.h>

void adc_init()
{
    P1DIR |= BIT1 + BIT3;   //A1(X) & A3(Y)
    P5DIR |= BIT0;          //A8(Z)
    ADCCTL0 |= ADCON;       //Turn on ADC
    ADCCTL1 |= ADCSSEL_2;   //Sets ADC clock source to SMCLK
    ADCMCTL0 |= ADCSREF_7;  //Sets reference to VEREF+ and VEREF-
//    ADCIFG &= ~ADCIFG0;     //Clear interrupt flag -- not sure if we need this
//    ADCIE |= ADCIE0;        //Enable interrupt on completion of conversion -- not sure if we need this
}

unsigned int find_x(void)
{
    ADCMCTL0 = ADCSREF_7 | ADCINCH_1;       //Selects A1 as input channel
    ADCCTL0 |= ADCENC | ADCSC;  //Enables and starts conversion
    while(!ADCIFG0);            //Waits for flag that is set when conversion completed (resets when ADCMEM0 is read)
    ADCCTL0 &= ~ADCENC;         //Disables conversions to allow input channel modification
    return ADCMEM0;             //Returns unsigned int value (only 8 bits used by default)
}

unsigned int find_y(void)
{
    ADCMCTL0 = ADCSREF_7 | ADCINCH_3;
    ADCCTL0 |= ADCENC | ADCSC;
    while(!ADCIFG0);
    ADCCTL0 &= ~ADCENC;
    return ADCMEM0;
}

unsigned int find_z(void)
{
    ADCMCTL0 = ADCSREF_7 | ADCINCH_8;
    ADCCTL0 |= ADCENC | ADCSC;
    while(!ADCIFG0);
    ADCCTL0 &= ~ADCENC;
    return ADCMEM0;
}
