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
volatile int showTime = 0;
int settingsFlag = 0;
int alarmFlag = 0;

//  Set limits for numbers that can be displayed on the clock
static unsigned int timeLowerLimit = 0;
static unsigned int minuteUpperLimit = 59;
static unsigned int hourUpperLimit = 23;

static const int MAX_SELECTION = 2;

int main(int argc, char** argv) {

    //  Setup interrupts
    RCONbits.IPEN = 1;      //  Enable priorities
    INTCONbits.GIEH = 1;    //  Enable high priority interrupts
    INTCONbits.GIEL = 1;    //  Disable low priority interrupts

    enableLED();
    initGeneralTimer();
    sevenSegInit();
    rotEncInit();
    soundInit();
    
    rtccTime RTCCTime;
    rtccTime RTCCRead;
    rtccTime RTCCAlarm;
    
    timeInit(&RTCCTime, 0, 0);
    alarmInit(&RTCCAlarm, 0, 0);
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
            for(count = 0; count < 2000; count++){
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
    static int isAlarmOn = 0;
    
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
                    mRtccAlrmDisable();
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
                ALL_IND_LED_OFF();
                REDisableInterrupts();
                genDelay_ms(300);
                break;
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
                    
                //  1 = Set Alarm    
                case 1:
                    if(!isAlarmOn){
                        IND_LED_ALMON_OFF();
                        IND_LED_ALMOFF_ON();
                    }
                    
                    else if(isAlarmOn){
                        IND_LED_ALMOFF_OFF();
                        IND_LED_ALMON_ON();
                    }
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

    RtccWrOn();

    //  Set Minute
    while(1){
        if(REFlags.turnFlag){
            counter += REReadEncoder(&tmp);
            for(j = 0; j < 500; j++);
            tmp = 0x00;
            tmp |= ((PORTB & 0x30) >> 4);
            RERelease();
        }
        
        if(hourMin == 0){
            counter = checkLimits(counter, timeLowerLimit, minuteUpperLimit);
        }
        
        else if(hourMin == 1){
            counter = checkLimits(counter, timeLowerLimit, hourUpperLimit);
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
        
        if(hourMin == 0){
            counter = checkLimits(counter, timeLowerLimit, minuteUpperLimit);
        }
        
        else if(hourMin == 1){
            counter = checkLimits(counter, timeLowerLimit, hourUpperLimit);
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
    mRtccAlrmEnable();
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
    ALL_ALM_LED_OFF();
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
