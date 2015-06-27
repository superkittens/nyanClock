/* 
 * File:   time.h
 * Author: superkittens
 *
 * Created on June 20, 2015, 5:37 PM
 */

#include <rtcc.h>

#ifndef TIME_H
#define	TIME_H

int timeInit(rtccTime * pTime, unsigned int hour, unsigned int min);
int alarmInit(rtccTime *pAlarm, unsigned int hour, unsigned int min);
int enableTime();
int getTime(rtccTime *pTime);
int writeTime(rtccTime *pTime);

//  Utilities for time read/write
unsigned char dtobcd(unsigned int);
unsigned int bcdtod(unsigned char);

#endif	/* TIME_H */

