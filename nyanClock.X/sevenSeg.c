#include "sevenSeg.h"
#include <stdlib.h>
#include <xc.h>

//  Seven Segment Display is COMMON ANODE

int sevenSegInit(){

    //  PORTD is dedicated for current sinks
    //  PORTD[0:6] for Segments A-G respectively.  PORTD[7] for colon
    TRISD = 0x00;
    LATD = 0x7F;

    //  PORTA[0:3] is dedicated for current sources
    TRISA &= 0xF0;
    LATA &= 0xF0;

    return 0;
}

//  Used for displaying time.  For general numbers, use the displayNum() function
int dispTime(unsigned int hour, unsigned int min){

    if(genDelayLockStatus){
        return 1;
    }
    
    COLONON();
    unsigned int dig0, dig1;

    if((hour >= 0) && (hour < 10)){
        displayDigit(hour, DIG2);
        genDelay_ms(DELAYTIME);
        displayDigit(0, DIG3);
        genDelay_ms(DELAYTIME);
        displayOFF();
    }

    else if(hour >= 10){
        dig0 = hour % 10;
        dig1 = hour / 10;

        displayDigit(dig0, DIG2);
        genDelay_ms(DELAYTIME);
        displayDigit(dig1, DIG3);
        genDelay_ms(DELAYTIME);
        displayOFF();
    }

    if((min >= 0) && (min < 10)){
        displayDigit(min, DIG0);
        genDelay_ms(DELAYTIME);
        displayDigit(0, DIG1);
        genDelay_ms(DELAYTIME);
        displayOFF();
    }

    else if(min >= 10){
        dig0 = min % 10;
        dig1 = min / 10;

        displayDigit(dig0, DIG0);
        genDelay_ms(DELAYTIME);
        displayDigit(dig1, DIG1);
        genDelay_ms(DELAYTIME);
        displayOFF();
    }

}

// Special function to display setting the hour or min depending on the value of hourMin.  hourMin = 0 -> minute, 1 -> hour
int dispSetTime(unsigned int time, int hourMin){

    if(genDelayLockStatus){
        return 1;
    }
    
    switch(hourMin){

        //  Case 0 handles setting the minute
        case 0:
            if((time >= 0) && (time <= 9)){
                displayDigit(time, DIG0);
                genDelay_ms(DELAYTIME);
                displayDigit(0, DIG1);
                genDelay_ms(DELAYTIME);
                displayOFF();
             }

            else if(time >= 10){
                int dig0 = time % 10;
                int dig1 = time / 10;

                displayDigit(dig0, DIG0);
                genDelay_ms(DELAYTIME);
                displayDigit(dig1, DIG1);
                genDelay_ms(DELAYTIME);
                displayOFF();
            }

            break;

        //  Case 1 handles setting hour
        case 1:
            if((time >= 0) && (time <= 9)){
                displayDigit(time, DIG2);
                genDelay_ms(DELAYTIME);
                displayDigit(0, DIG3);
                genDelay_ms(DELAYTIME);
                displayOFF();
             }

            else if(time >= 10){
                int dig0 = time % 10;
                int dig1 = time / 10;

                displayDigit(dig0, DIG2);
                genDelay_ms(DELAYTIME);
                displayDigit(dig1, DIG3);
                genDelay_ms(DELAYTIME);
                displayOFF();
            }

            break;

    }
}


//  Used to display general, non-time numbers
int displayNum(unsigned int number){

    if(genDelayLockStatus){
        return 1;
    }
    //  The maximum number a four-digit display is 9999.  Need to check for this first
    if(number > 9999){ return -1; }

    //  If number is a single digit
    if((number >= 0) && (number <= 9)){

        displayDigit(number, DIG0);
        genDelay_ms(DELAYTIME);
    }

    //  If number is double digits
    else if((number > 9) && (number <= 99)){

        int dig0 = number % 10;
        int dig1 = number / 10;
        int del = 0;

        displayDigit(dig0, DIG0);
        genDelay_ms(DELAYTIME);
        displayDigit(dig1, DIG1);
        genDelay_ms(DELAYTIME);
        
    }

    else if((number > 99) && (number <= 999)){

        int dig0 = number % 10;
        int dig1 = number % 100 / 10;
        int dig2 = number / 100;
        int del = 0;

        displayDigit(dig0, DIG0);
        genDelay_ms(DELAYTIME);
        displayDigit(dig1, DIG1);
        genDelay_ms(DELAYTIME);
        displayDigit(dig2, DIG2);
        genDelay_ms(DELAYTIME);

    }

    else if((number > 999) && (number <= 9999)){

        int dig0 = number % 10;
        int dig1 = number % 1000 % 100 / 10;
        int dig2 = number % 1000 / 100;
        int dig3 = number / 1000;
        int del = 0;

        displayDigit(dig0, DIG0);
        genDelay_ms(DELAYTIME);
        displayDigit(dig1, DIG1);
        genDelay_ms(DELAYTIME);
        displayDigit(dig2, DIG2);
        genDelay_ms(DELAYTIME);
        displayDigit(dig3, DIG3);
        genDelay_ms(DELAYTIME);

    }

    displayOFF();
}


//  displayDigit actually switches on/off GPIOs to light segment LEDs
int displayDigit(unsigned int number, int digit){

    if((number > 9) || (number < 0)){ 
        return -1;
    }
    
    //  Sequence for lighting a digit:
    //  1.  Turn off all segments (leave colon alone) and sources
    //  2.  Flip on the pins specified in SEGSINKARRAY[number] by using the bitwise OR operation
    //  3.  Turn on specified source pin (int digit)
    LATA &= 0xF0;
    LATD &= 0x80;
    
    LATD |= SEGSINKARRAY[number];
    PORTA |= SEGDIGARRAY[digit];

    return 0;
}


void displayOFF(){
    PORTB |= 0x7F;
    PORTA &= 0xF0;
}
