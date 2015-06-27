/* 
 * File:   sevenSeg.h
 * Author: superkittens
 *
 * Created on February 11, 2015, 10:18 PM
 */

#include <xc.h>
#include "delay.h"

#ifndef SEVENSEG_H
#define	SEVENSEG_H

#define DIG0 0
#define DIG1 1
#define DIG2 2
#define DIG3 3

#define COLONON() (LATDbits.LD7 = 1)
#define COLONOFF() (LATDbits.LD7 = 0)

const int DELAYTIME = 5;
const unsigned char SEGSINKARRAY[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};
const unsigned char SEGDIGARRAY[4] = {0x08, 0x04, 0x02, 0x01};

int isColonON = 0;


int sevenSegInit();
int displayNum(unsigned int number);
int dispSetTime(unsigned int time, int hourMin);
int dispTime(unsigned int hour, unsigned int min);
int displayDigit(unsigned int number, int digit);
void displayOFF();

#endif	/* SEVENSEG_H */

