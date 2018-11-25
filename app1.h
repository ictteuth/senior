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

int xSpot;
int ySpot;
int bright;
int aColor;

void initTimerB1();


/*
 * Draws a 'circle' on the display
 * x and y range from 0-11
 * y range from 0-7
 * color is RED, GREEN, or BLUE
 */
void drawSpot(int x, int y, int brightness, int color);



#endif /* APP1_H_ */
