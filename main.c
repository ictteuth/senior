#include <msp430.h>
#include "LED_control.h"


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	CSCTL1 |= DCORSEL_7; //speed up clock

	initLED();

	return 0;
}
