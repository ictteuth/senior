/*
 * lcd.c
 *
 *  Created on: Nov 28, 2018
 *      Author: ictteuth
 */

#include <msp430.h>
#include <msp430fr2355.h>
#include "lcd.h"

void lcd_init(void)
{
    lcd_command(0x33);
    lcd_command(0x32);
    lcd_command(0x2C);
    lcd_command(0x0C);
    lcd_command(0x01);
}

void lcd_command(char uf_lcd_x)
{
    P6DIR = 0xFF;
    char uf_lcd_temp = uf_lcd_x;
    P6OUT = 0x00;
    __delay_cycles(40000);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P6OUT = uf_lcd_x;
    __delay_cycles(40000);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P6OUT = uf_lcd_x;
    __delay_cycles(40000);
    P6OUT = 0x00;
    __delay_cycles(40000);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P6OUT = uf_lcd_x;
    __delay_cycles(40000);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P6OUT = uf_lcd_x;
    __delay_cycles(40000);
}

void lcd_char(char uf_lcd_x)
{
    P6DIR = 0xFF;
    char uf_lcd_temp = uf_lcd_x;
    P6OUT = 0x10;
    __delay_cycles(10000);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P6OUT = uf_lcd_x;
    __delay_cycles(10000);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P6OUT = uf_lcd_x;
    __delay_cycles(10000);
    P6OUT = 0x10;
    __delay_cycles(10000);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P6OUT = uf_lcd_x;
    __delay_cycles(10000);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P6OUT = uf_lcd_x;
    __delay_cycles(10000);
}

void lcd_str(char* aStr)
{
    while (*aStr != 0x00)
    {
        lcd_char(*aStr++);
    }
}

//4.0 -> D4
//4.1 -> D5
//4.2 -> D6
//4.3 -> D7
//4.4 -> RS
//4.5 -> E


