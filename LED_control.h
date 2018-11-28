/*
 * LED_control.h
 *
 *  Created on: Nov 9, 2018
 *      Author: Ian
 */

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#define MASK0 0b00000001





// array that holds all rgb values for pixels in order useful for programming
// due to a slight mistake, the color order is blue-green-red in this array
//  and on the physical board.
char imgArr[12][36];

// variable that keeps track of which row is currently on
int currentRow;

// mask used to select RGB bits, used to set brightness level
char currentMask;

/*
 * Initializes the LED array and sets up timers and interrupts
 */
void initLED();

/*
 * Updates the pixel at coordinates (x,y) to have the value specified by rgb.
 * modifies imgArr.
 */
void changePixel(int x, int y, char rgb);

/*
 * called by a timer interrupt to start updating a row with the next mask
 */
void updateISR();

/*
 * Updates all columns with the proper values for the given row
 */
void updateColumns(int row, char mask);

/*
 * Sends the proper outputs to columns 0 through 4 for the given row
 */
void updateCol0_4(int row, char mask);

/*
 * Sends the proper outputs to columns 5 through 9 for the given row
 */
void updateCol5_9(int row, char mask);

/*
 * Sends the proper outputs to columns 10 through 14 for the given row
 */
void updateCol10_14(int row, char mask);

/*
 * Sends the proper outputs to columns 15 through 17 for the given row
 */
void updateCol15_17(int row, char mask);

/*
 * Sends the proper outputs to columns 18 through 20 for the given row
 */
void updateCol18_20(int row, char mask);

/*
 * Sends the proper outputs to columns 21 through 25 for the given row
 */
void updateCol21_25(int row, char mask);

/*
 * Sends the proper outputs to columns 26 through 30 for the given row
 */
void updateCol26_30(int row, char mask);

/*
 * Sends the proper outputs to columns 31 through 35 for the given row
 */
void updateCol31_35(int row, char mask);

/*
 * Sends active high signal to turn on the next row
 */
void nextRow();


#endif /* LED_CONTROL_H_ */
