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
#include "sound.h"

#define _XTAL_FREQ 16000000


#pragma config WDTEN = OFF, OSC = HS, CPUDIV = OSC1, CFGPLLEN = OFF, PLLDIV =1, XINST = OFF, RTCOSC = T1OSCREF

//  States in das state machine are defined here
int state_settingsCycle();
int state_setTime();
int state_setAlarm();
int state_soundAlarm();

volatile unsigned char tmp = 0x00;
volatile int test = 0;
volatile int showTime = 0;
int settingsFlag = 0;
int isAlarmOn = 0;
int alarmFlag = 0;

const int MAX_SELECTION = 2;

int main(int argc, char** argv) {

    //  Setup interrupts
    RCONbits.IPEN = 1;      //  Enable priorities
    INTCONbits.GIEH = 1;    //  Enable high priority interrupts
    INTCONbits.GIEL = 1;    //  Disable low priority interrupts
    INTCONbits.TMR0IE = 1;  //  Enable TMR0 overflow interrupt
    INTCON2bits.T0IP = 1;   //  Set TMR0 int to low priority
    PIE3bits.RTCCIE = 1;    //  Enable RTCC Alarm interrupt
    IPR3bits.RTCCIP = 1;    //  Set RTCC Alarm int to high priority
    ANCON1bits.PCFG12 = 1;  //  Set RB0 to DIO
    TRISBbits.TRISB0 = 1;
    INTCONbits.INT0IE = 1;  //  Enable External INT0 interrupt
    INTCON2bits.INTEDG0 = 0;//  Set INT0 interrupt on falling edge
    
    RTCCIF = 0;
    
    enableLED();
    initGeneralTimer();
    sevenSegInit();
    rotEncInit();
    soundInit();
    
    rtccTime RTCCTime;
    rtccTime RTCCRead;
    rtccTime RTCCAlarm;
    
    timeInit(&RTCCTime, 0, 0);
    alarmInit(&RTCCAlarm, 0, 1);
    enableTime();
    
    tmp |= ((PORTB & 0x30) >> 4);

    ALM_LED_RED_ON();
    genDelay_ms(100);
    ALM_LED_RED_OFF();
    ALM_LED_GRN_ON();
    genDelay_ms(100);
    ALM_LED_GRN_OFF();
    ALM_LED_BLUE_ON();
    genDelay_ms(100);
    ALM_LED_BLUE_OFF();
 

    
    unsigned int count = 0;

    //  Idle state
    while(1){

        if(showTime){
            RtccReadTime(&RTCCRead);
            unsigned int RTCHour = bcdtod(RTCCRead.f.hour);
            unsigned int RTCMin = bcdtod(RTCCRead.f.min);
            for(count = 0; count < 5000; count++){
                dispTime(RTCHour, RTCMin);
            }
            
            showTime = 0;
        
        }
            
        
        if(!clickEvent()){
            displayOFF();
            genDelay_ms(300);
            REEnableInterrupts();
            state_settingsCycle();
        }
        
        if(alarmFlag){
            state_soundAlarm();
        }

    }
         


    return (EXIT_SUCCESS);
}


//  Settings selection state
int state_settingsCycle(){
    
    int selection = 0;
    int selectionChg = -1;
    settingsFlag = 1;
    int firstTime = 1;
    int test = 0;
    
    while(1){
        
        if(!clickEvent()){
            
            //  Set Time
            if(selection == 0){
                genDelay_ms(300);
                selection = state_setTime();
                selectionChg = -1;
                
            }
            
            //  Turn Alarm on or off
            else if(selection == 1){
                genDelay_ms(300);
                
                if(!isAlarmOn){
                    IND_LED_ALMOFF_OFF();
                    IND_LED_ALMON_ON();
                    isAlarmOn = 1;
                    state_setAlarm();
                }
                
                else if(isAlarmOn){
                    IND_LED_ALMOFF_ON();
                    IND_LED_ALMON_OFF();
                    isAlarmOn = 0;
                }
            }
            
            else if(selection == 2){
                ALL_IND_LED_OFF();
                REDisableInterrupts();
                genDelay_ms(300);
                break;
            }
            
            else{
                if(!test){
                    ALL_ALM_LED_ON();
                    test = 1;
                }
                
                else if(test){
                    ALL_ALM_LED_OFF();
                    test = 0;
                }
                
                genDelay_ms(300);
            }
        }
        
        if(REFlags.turnFlag){
            selection += REReadEncoder(&tmp);
            if(selection > MAX_SELECTION){
                selection = 0;
            }
            
            else if(selection < 0){
                selection = MAX_SELECTION;
            }
            
        }
        
        
        if(selection != selectionChg){
            selectionChg = selection;
            ALL_IND_LED_OFF();
            switch(selection){
            
                //  0 = Set Time
                case 0:
                    IND_LED_SETTIME_ON();
                    break;
                    
                //  1 = Set Time    
                case 1:
                    IND_LED_ALMOFF_ON();
                    break;
                    
                //  2 = Exit    
                case 2:
                    IND_LED_EXIT_ON();
                    break;
                    
                default:
                    ALM_LED_GRN_ON();
                    break;
            }
            
            if(!firstTime){
                int j = 0;
                for(j = 0; j < 500; j++);
                tmp = 0x00;
                tmp |= ((PORTB & 0x30) >> 4);
                RERelease();
            }
        
            firstTime = 0;
        }
      
    }
    return 0;
}


int state_setTime(){

    int del = 0;
    int counter = 0;
    int hourMin = 0;
    int j = 0;
    rtccTime RTCCSetTimeDate;

    //RtccWrOn();

    //  Set Minute
    while(1){
        if(REFlags.turnFlag){
            counter += REReadEncoder(&tmp);
            for(j = 0; j < 500; j++);
            tmp = 0x00;
            tmp |= ((PORTB & 0x30) >> 4);
            RERelease();
        }

        dispSetTime(counter, hourMin);

        if(!clickEvent()){ 
            
            genDelay_ms(300);

            displayOFF();

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
    //mRtccWrOff();

    return 0;

}

int state_setAlarm(){
    int del = 0;
    int counter = 0;
    int hourMin = 0;
    int j = 0;
    rtccTime RTCCSetAlarm;

    //  Set Minute
    while(1){
        if(REFlags.turnFlag){
            counter += REReadEncoder(&tmp);
            for(j = 0; j < 500; j++);
            tmp = 0x00;
            tmp |= ((PORTB & 0x30) >> 4);
            RERelease();
        }

        dispSetTime(counter, hourMin);

        if(!clickEvent()){ 
            
            genDelay_ms(300);

            displayOFF();

            if(hourMin == 0){
                RTCCSetAlarm.f.min = dtobcd(counter);
                hourMin = 1;
                tmp = 0;
                counter = 0;
            }

            else if(hourMin == 1){
                RTCCSetAlarm.f.hour = dtobcd(counter);
                RTCCSetAlarm.f.sec = 0;
                break;
            }
        }
    }
  
    RtccWriteAlrmTime(&RTCCSetAlarm);
    //mRtccWrOff();

    return 1;
}

int state_soundAlarm(){
    unsigned int sequence = 0;
    setupForPlayback();
    genDelay_ms(300);
    while(genDelayLockStatus);
    
    while(alarmFlag){
        toggleLED(sequence);
        playback();
        sequence++;
        
        if(sequence > 5){
            sequence = 0;
        }
    }
    
    disableAmplifier();
    alarmFlag = 0;
    return 0;
}


void interrupt high_priority hp_isr(){
    
    //  External Interrupt Routine
    if(INT0IE && INT0IF){
        
        if(alarmFlag){
            alarmFlag = 0;
            showTime = 1;
            INT0IF = 0;
            return;
        }
        else{
            showTime = 1;
            INT0IF = 0;
            return;
        }
    }
    
    //  Timer0 routine
    if(TMR0IE && TMR0IF){
        genDelayLockStatus = 0;
        TMR0IF = 0;
        return;
    }
    
    //  Rotary Encoder Routine
    if(RBIE && RBIF){
        
        if((REFlags.turnFlag)){
            RBIF = 0;
            return;
        }
        
        else{
        
            tmp = tmp << 2;
            tmp |= ((PORTB & 0x30) >> 4);
        
            if(tmp >= 0xD0){
                if((tmp == 0xE1) || (tmp == 0xD2)){
                    RELockout();
                    RBIF = 0;
                    return;
                }
                
                else{
                    RBIF = 0;
                    return;
                }
            }
        
            else{
                RBIF = 0;
                return;
            }
        
        }
        
    }
    
    //  Alarm Routine
    if(RTCCIE && RTCCIF){
        alarmFlag = 1;
        RTCCIF = 0;
        return;
    }
    
}

void interrupt low_priority lp_isr(){
    
    
    
    
}
