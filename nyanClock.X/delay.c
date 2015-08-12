#include <xc.h>
#include "delay.h"

//  TMR0 is used as a general purpose timer
int initGeneralTimer(){
    /*  TMR0 Settings:
     *  TMR0 Disabled
     *  16 Bit Mode
     *  Based off of Instruction Cycle Clock
     *  Low - High Transistion
     *  Prescaler 256
     */
    
    T0CONbits.TMR0ON = 0;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b111;
    return 0;
}

//  Why not "delay()"?  Because delay() is already defined in xc.h
int genDelay_ms(unsigned int delayTime_ms){
    
    genDelayLockStatus = 1;
    
    if(delayTime_ms > MAX_TMR0_DELAY_TIME){
        return 1;
    }
    
    if(delayTime_ms == 0){
        return 2;
    }
    
    unsigned int tmrReg = 65535 - (delayTime_ms * 16);
    unsigned char tmrRegL = tmrReg & 0xFF;
    unsigned char tmrRegH = (tmrReg >> 8) & 0xFF; 

    TMR0H = tmrRegH;
    TMR0L = tmrRegL;
    
    T0CONbits.TMR0ON = 1;
    
    //  ISR in main will release lock
    while(genDelayLockStatus);
    
    T0CONbits.TMR0ON = 0;
    
    return 0;
}