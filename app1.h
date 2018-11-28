/*
 * app1.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Ian
 */

#ifndef APP1_H_
#define APP1_H_

#define RED 0
#define GREEN 1
#define BLUE 2



/*****************************************************************************************************************************************/
int opAmpCreatureCounter;
int oacc2;

int xVal;
/*****************************************************************************************************************************************/



int xSpot;
int ySpot;
int bright;
int aColor;

int anAxis;

float xCurrent;
float yCurrent;
float zCurrent;

int positionReady; //flag to tell if new position data is available

//button stuff
int mode;
int isNewMode;
int switchDelay;

//sets up timer b1 interrupt and stuff
void initTimerB1();


/*
 * Draws a 'circle' on the display
 * x and y range from 0-11
 * y range from 0-7
 * color is RED, GREEN, or BLUE
 */
void drawSpot(int x, int y, int brightness, int color);

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
void program1MainLoop();

#endif /* APP1_H_ */
