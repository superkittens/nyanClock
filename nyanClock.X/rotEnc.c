#include "rotEnc.h"

int rotEncInit(){

    //  Rotary Encoder A and B terminals use PORTC[4:5]
    TRISC |= 0x34;
    return 0;
}

int readEnc(){

    PCTemp = 0x00;
    oldState = oldState << 2;
    oldState |= ((PORTC & 0x30) >> 4);

    if((oldState < 0x00) || (oldState > 0xF)){ return -99; }
    return (encoderStates[( oldState & 0x0F)]);
}

int clickEvent(){

    if(PORTCbits.RC3 == 1){ return 0; }
    else if(PORTCbits.RC3 == 0){ return 1; }

    else{ return -2; }
}