#include <msp430.h>
#include "LED_control.h"
#include "app1.h"

void showMenu();


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	CSCTL1 |= DCORSEL_7; //speed up clock

	initLED();
	initTimerB1();

	return 0;
}
