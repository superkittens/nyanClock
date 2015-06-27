#include "time.h"

int timeInit(rtccTime *pTime, unsigned int hour, unsigned int min){
    
    RtccInitClock();
    RtccWrOn();
    mRtccSetClockOe(0);
    pTime->f.hour = dtobcd(hour);
    pTime->f.min = dtobcd(min);
    pTime->f.sec = 0;
    
    RtccWriteTime(pTime, 1);
    mRtccWrOff();
  
    return 0;
}

int alarmInit(rtccTime *pAlarm, unsigned int hour, unsigned int min){
    
    return 0;
}

int enableTime(){
    mRtccOn();
    return 0;
}

int getTime(rtccTime *pTime){
    
    RtccReadTime(pTime);
    return 0;
}

unsigned char dtobcd(unsigned int dec){

    if((dec <0) || (dec > 99)){
        return 255;
    }

    int i;
    unsigned char bcdConv = 0x00;

    for(i = 0; i <= 7; i++){

        if(dec & 0x0080){
            bcdConv = bcdConv << 1;
            bcdConv |= 0x01;
        }

        else{ bcdConv = bcdConv << 1; }
        
        dec = dec << 1;

        if(((bcdConv & 0x0F) >= 5) && (i != 7)){
            bcdConv += 0x03;
        }

        if(((bcdConv & 0xF0) >= 0x50) && (i != 7)){
            bcdConv += 0x30;
        }

        
    }

    return bcdConv;
}

/*  This is a very, very brute force method of converting from BCD to Decimal which is alright for our purposes as the numbers on the clock really only
 *  to 60.  A more elegant method is needed if we need bigger numbers
 */
unsigned int bcdtod(unsigned char bcd){

    if((bcd >= 0x00) && (bcd < 0x10)){
        return (int)bcd;
    }

    else if((bcd >= 0x10) && (bcd < 0x20)){
        return (int)bcd - 6;
    }

    else if((bcd >= 0x20) && (bcd < 0x30)){
        return (int)bcd - 12;
    }

    else if((bcd >= 0x30) && (bcd < 0x40)){
        return (int)bcd - 18;
    }

    else if((bcd >= 0x40) && (bcd < 0x50)){
        return (int)bcd - 24;
    }

    else if((bcd >= 0x50) && (bcd < 0x60)){
        return (int)bcd - 30;
    }
}
