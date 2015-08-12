#include "rotEnc.h"

int rotEncInit(){

    ANCON1bits.PCFG9 = 1;
    //  Rotary Encoder A and B terminals use PORTC[4:5]
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    
    REFlags.turnFlag = 0;
    return 0;
}

int readEnc(){

    PCTemp = 0x00;
    oldState = oldState << 2;
    oldState |= (PORTB & 0x30);

    if((oldState < 0x00) || (oldState > 0xFF)){ 
        return -99; 
    }

    return (encoderStates[( oldState & 0x0F)]);
}

int clickEvent(){

    if(PORTBbits.RB3 == 1){ 
        return 0; 
    }
    else if(PORTBbits.RB3 == 0){ 
        return 1; 
    }

    else{ 
        return -2; 
    }
}

int checkRotEncLimits(int lowerLimit, int upperLimit, int valueToCheck){

    if(valueToCheck < lowerLimit){
        return upperLimit;
    }

    else if(valueToCheck > upperLimit){
        return lowerLimit;
    }

    return valueToCheck;

}

void REEnableInterrupts(){
    INTCONbits.RBIE = 1;    //  Enable Change on RB Interrupt
    INTCON2bits.RBIP = 1;   //  RB Change int high priority
    return;
}

void REDisableInterrupts(){
    INTCONbits.RBIE = 0;    //  Enable Change on RB Interrupt
    return;
}

//  Locks up the Change on PORTB Interrupt so that another external change on PORTB 
//  doesn't interfere with the current operation triggered by the same interrupt
void RELockout(){
    REFlags.turnFlag = 1;
}

int REReadEncoder(unsigned char *collectedStates){
    
    if(*collectedStates == 0xE1){
        return 1;
    }
    
    else if(*collectedStates == 0xD2){
        return -1;
    }
    
    else{
        return 0;
    }
}

//  Releases Change on PORTB Interrupt so that it can be used again for another resource
void RERelease(){
    REFlags.turnFlag = 0;
    return;
}