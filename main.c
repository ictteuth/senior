#include <msp430.h>
#include "LED_control.h"
#include "app1.h"
#include "lcd.h"

void showMenu();


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
//	__delay_cycles(10000);

	CSCTL1 |= DCORSEL_7; //speed up clock

	lcd_init();
	initLED();
	initTimerB1();


	//set up gpio for button

	P2DIR &= ~BIT6; //input

	//set up interrupt for button
	P2IES |= BIT6;
	P2IE |= BIT6;
	P2IFG &= ~BIT6;


	program1MainLoop();
	return 0;
}

void showMenu()
{

}
