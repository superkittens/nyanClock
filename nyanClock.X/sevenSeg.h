/* 
 * File:   sevenSeg.h
 * Author: superkittens
 *
 * Created on February 11, 2015, 10:18 PM
 */

#include <xc.h>

#ifndef SEVENSEG_H
#define	SEVENSEG_H

#define _XTAL_FREQ 8000000
#define DIG0 0
#define DIG1 1
#define DIG2 2
#define DIG3 3

#define COLONON() (PORTBbits.RB7 = 1)
#define COLONOFF() (PORTBbits.RB7 = 0)

const int DELAYTIME = 50;
const unsigned char SEGSINKARRAY[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};
const unsigned char SEGSINKARRAYCOL[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x82, 0x82, 0xF8, 0x80, 0x98};
const unsigned char SEGDIGARRAY[4] = {0x01, 0x02, 0x04, 0x08};

int isColonON = 0;


int sevenSegInit();
int displayNum(unsigned int number);
int dispSetTime(unsigned int time, int hourMin);
int dispTime(unsigned int hour, unsigned int min);
int displayDigit(unsigned int number, int digit);
int delay(unsigned int delay);
void displayOFF();

#endif	/* SEVENSEG_H */

