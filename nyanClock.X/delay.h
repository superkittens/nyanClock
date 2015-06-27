/* 
 * File:   delay.h
 * Author: superkittens
 *
 * Created on June 20, 2015, 4:33 PM
 */

#ifndef DELAY_H
#define	DELAY_H

//  Due to clock frequency and prescalar selection, we have a max limit on a delay time
#define MAX_TMR0_DELAY_TIME (4000)

//  Lock implemented to avoid genDelay from being called by different functions at the same time
volatile int genDelayLockStatus = 0;

int initGeneralTimer();
int genDelay_ms(unsigned int);

#endif	/* DELAY_H */

