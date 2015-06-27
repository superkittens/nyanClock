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
#include "led.h"
#include "time.h"
#include <rtcc.h>

#define _XTAL_FREQ 16000000


#pragma config WDTEN = OFF, OSC = HS, CPUDIV = OSC1, CFGPLLEN = OFF, PLLDIV =1, XINST = OFF, RTCOSC = T1OSCREF

//  States in das state machine are defined here
int setTime();

int main(int argc, char** argv) {

    //  Setup interrupts
    RCONbits.IPEN = 1;      //  Enable priorities
    INTCONbits.GIEH = 1;    //  Enable high priority interrupts
    INTCONbits.GIEL = 0;    //  Disable low priority interrupts
    INTCONbits.TMR0IE = 1;  //  Enable TMR0 overflow interrupt
    INTCON2bits.T0IP = 1;   //  Set TMR0 int to high priority
    
    enableLED();
    initGeneralTimer();
    sevenSegInit();
    rotEncInit();
    
    rtccTime RTCCTime;
    rtccTime RTCCRead;
    
    timeInit(&RTCCTime, 0, 27);
    enableTime();
    
    ALM_LED_RED_ON();
    genDelay_ms(100);
    ALM_LED_RED_OFF();
    ALM_LED_GRN_ON();
    genDelay_ms(100);
    ALM_LED_GRN_OFF();
    ALM_LED_BLUE_ON();
    genDelay_ms(100);
    ALM_LED_BLUE_OFF();
   

    while(1){

        
        getTime(&RTCCRead);
        unsigned int RTCHour = bcdtod(RTCCRead.f.hour);
        unsigned int RTCMin = bcdtod(RTCCRead.f.min);

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
                //__delay_ms(50);
                //__delay_ms(50);
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
        //__delay_ms(50);
        //__delay_ms(50);
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
            for(del = 0; del < 5; del++){
                //__delay_ms(50);
                //__delay_ms(50);
            }

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


void interrupt high_priority hp_isr(){
    
    if(TMR0IE && TMR0IF){
        genDelayLockStatus = 0;
        TMR0IF = 0;
        return;
    }
    
}
