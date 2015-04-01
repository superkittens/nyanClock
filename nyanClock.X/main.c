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
//#include "genUtil.h"
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

    sevenSegInit();
    rotEncInit();

    //  RTCC Configuration
    rtccTime RTCCTime;
    rtccTime RTCCRead;
    rtccTime RTCCAlarm;

    unsigned int RTCHour;
    unsigned int RTCMin;

    RtccInitClock();
    RtccWrOn();
    mRtccSetClockOe(0);
    RTCCTime.f.hour = dtobcd(22);
    RTCCTime.f.min = dtobcd(47);
    RTCCTime.f.sec = 0;

    //  Alarm setup here is temporary.  Must be removed for final release
    RTCCAlarm.f.hour = 0x12;
    RTCCAlarm.f.min = 0x00;
    RTCCAlarm.f.sec = 0x00;

    RtccSetAlarmRpt(RTCC_RPT_MIN, 1);
    RtccSetChimeEnable(1, 1);

    RtccWriteTime(&RTCCTime, 1);
    RtccWriteAlrmTime(&RTCCAlarm);
    mRtccOn();
    mRtccAlrmEnable();
    mRtccWrOff();

    while(1){

        
        RtccReadTime(&RTCCRead);
        RTCHour = bcdtod(RTCCRead.f.hour);
        RTCMin = bcdtod(RTCCRead.f.min);

        dispTime(RTCHour, RTCMin);
        
        if(!clickEvent()){
            displayOFF();
            //settingsCycle();
            setTime();
        }


    }


    return (EXIT_SUCCESS);
}

//  Function to handle the click of the rotary encoder to set time, alarm and alarm on/off 
int settingsCycle(){

    int setting = 0;
    int settingTemp = setting;
    int tmp = 0;
    int i = 0;
    int alarmOnOffColour = 0;

    //displayNum(1);

    while(1){

        tmp = readEnc();
        if(tmp == -99){
            return -1;
        }

        else if(tmp){
            setting += tmp;
            setting = checkRotEncLimits(0, 2, setting);

            if(setting != settingTemp){

                switch(setting){

                    case 0:
                        //  Light setTime LED
                        break;

                    //  Two cases in set alarm indicator.  If alarm is currently OFF, then LED will be red.  If it is ON, the LED will be green
                    case 1:
                        if(!alarmOnOffColour){
                        //  Light OFF LED
                        }

                        if(alarmOnOffColour){
                        //  Light ON LED
                        }

                        break;

                    case 2:
                        //  Light Exit LED
                        break;
                }

                settingTemp = setting;
            }
        }

        if(!clickEvent()){

            for(i = 0; i < 5; i++){
                __delay_ms(50);
                __delay_ms(50);
            }

            switch(setting){
                case 0:
                    setting = setTime();
                    break;

                case 1:
                    setting = setTime();
                    break;

            }
        }

        if((!clickEvent()) && (setting == 2)){
            break;
        }
    }

    return 0;
}


int setTime(){

    int del = 0;
    int counter = 0;
    int tmp = 0;
    int hourMin = 0;
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
        if(tmp == -99){ 
            return -1; 
        }

        else if(tmp){
            counter += tmp;
            counter = checkRotEncLimits(0, 59, counter);
        }

        dispSetTime(counter, hourMin);

        if(!clickEvent()){ 

            displayOFF();

            __delay_ms(50);
            __delay_ms(50);

            if(hourMin == 0){
                RTCCSetTimeDate.f.min = dtobcd(counter);
                hourMin = 1;
                tmp = 0;
                counter = 0;
            }

            else if(hourMin == 1){
                RTCCSetTimeDate.f.hour = dtobcd(counter);
                RTCCSetTimeDate.f.sec = 0;
                break;
            }
        }
    }
  
    RtccWriteTime(&RTCCSetTimeDate, 1);
    mRtccWrOff();

    //  Returns 2, which is the "Exit" option for the settings function (from which setTime was called)
    return 2;

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
