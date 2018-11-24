#include <msp430.h>
#include "LED_control.h"


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	initLED();

	return 0;
}
