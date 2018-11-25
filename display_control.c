/*
 * display_control.c
 *
 *  Created on: Nov 25, 2018
 *      Author: Ian
 */

#include <msp430.h>
#include <msp430fr2355.h>

#include "display_control.h"
#include "LED_control.h"

/*
 * sets all pixels to the specified rgb value
 */
void setSolidColor(char rgb)
{
    int x,y;

    for (x = 11; x >= 0; x--)
    {
        for (y = 35; y >= 0; y--)
        {
            changePixel(x,y,rgb);
        }
    }

}

