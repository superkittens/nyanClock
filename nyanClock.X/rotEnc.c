#include "rotEnc.h"

int rotEncInit(){

    //  Rotary Encoder A and B terminals use PORTC[4:5]
    TRISB |= 0x38;
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