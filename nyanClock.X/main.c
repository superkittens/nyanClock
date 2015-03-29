/* 
 * File:   main.c
 * Author: superkittens
 *
 * Created on January 31, 2015, 9:19 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "sevenSeg.h"
#include "rotEnc.h"
#include <rtcc.h>

#define _XTAL_FREQ 8000000


#pragma config WDTEN = OFF, OSC = INTOSC, XINST = OFF, RTCOSC = T1OSCREF

int setTime();
unsigned char dtobcd(unsigned int dec);
unsigned int bcdtod(unsigned char bcd);
unsigned int concatInt(unsigned int x, unsigned int y);

int main(int argc, char** argv) {

    //  Oscillator Configuration (Internal OSC, 8 MHz no PLL)
    OSCCONbits.IRCF = 0b111;
    
    unsigned int tmp = 0;
    unsigned int counter = 0;
    rtccTime RTCCTimeDate;
    rtccTime RTCCRead;

    sevenSegInit();
    rotEncInit();

    //  RTCC Configuration
    RtccInitClock();
    RtccWrOn();
    mRtccSetClockOe(0);
    RTCCTimeDate.f.hour = 0x00;
    RTCCTimeDate.f.min = 0x10;
    RTCCTimeDate.f.sec = 0;

    unsigned int RTCHour;
    unsigned int RTCMin;

    RtccWriteTime(&RTCCTimeDate, 1);
    mRtccOn();
    mRtccWrOff();

    while(1){


        RtccReadTime(&RTCCRead);
        RTCHour = bcdtod(RTCCRead.f.hour);
        RTCMin = bcdtod(RTCCRead.f.min);

        dispTime(RTCHour, RTCMin);
        
        if(!clickEvent()){
            displayOFF();
            setTime();
        }

 

        /*
        if(!clickEvent()){ counter = 0; }
        tmp = readEnc();

        if(tmp == -99){}
        
        else if(tmp){
            counter += tmp;
            if(counter < 0){ counter = 9999; }
            else if(counter > 9999){ counter = 0; }
        }
        
        displayNum(counter, 0);
        */
    }


    return (EXIT_SUCCESS);
}

int setTime(){
    int del = 0;
    int counter = 0;
    int tmp = 0;
    rtccTime RTCCSetTimeDate;

    RtccWrOn();

    for(del = 0; del < 5; del++){
        //  __delay_ms can't take an argument of 100, so we use 2 50 ms delays instead
        __delay_ms(50);
        __delay_ms(50);
    }

    //  Set Minute
    while(1){
        tmp = readEnc();
        if(tmp == -99){  }

        else if(tmp){
            counter += tmp;
            if(counter < 0){ counter = 59; }
            else if(counter > 59){ counter = 0; }
        }

        dispSetTime(counter, 0);

        if(!clickEvent()){ 
            for(del = 0; del < 5; del++){
                __delay_ms(50);
                __delay_ms(50);
                
                RTCCSetTimeDate.f.min = dtobcd(counter);
            }

            break;
        }
    }

    tmp = 0;
    counter = 0;
    //Set Hour
    while(1){
        tmp = readEnc();
        if(tmp == -99){  }

        else if(tmp){
            counter += tmp;
            if(counter < 0){ counter = 23; }
            else if(counter > 23){ counter = 0; }
        }

        dispSetTime(counter, 1);

        if(!clickEvent()){
            for(del = 0; del < 5; del++){
                __delay_ms(50);
                __delay_ms(50);

                RTCCSetTimeDate.f.hour = dtobcd(counter);
                RTCCSetTimeDate.f.sec = 0;
            }

            break;
        }
    }
    
    RtccWriteTime(&RTCCSetTimeDate, 1);
    mRtccWrOff();
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
