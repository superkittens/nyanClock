#include "sevenSeg.h"
#include <stdlib.h>
#include <xc.h>

//  Seven Segment Display is COMMON ANODE

int sevenSegInit(){

    //  PORTB is dedicated for current sinks
    //  PORTB[0:6] for Segments A-G respectively.  PORTB[7] for colon
    TRISB = 0x00;
    PORTB = 0x7F;

    //  PORTA is dedicated for current sources
    TRISA &= 0xF0;
    PORTA &= 0xF0;

    return 0;
}

//  Used for displaying time.  For general numbers, use the displayNum() function
int dispTime(unsigned int hour, unsigned int min){

    unsigned int dig0, dig1;

    if((hour >= 0) && (hour < 10)){
        displayDigit(hour, DIG2);
        delay(DELAYTIME);
        displayDigit(0, DIG3);
        delay(DELAYTIME);
        displayOFF();
    }

    else if(hour >= 10){
        dig0 = hour % 10;
        dig1 = hour / 10;

        displayDigit(dig0, DIG2);
        delay(DELAYTIME);
        displayDigit(dig1, DIG3);
        delay(DELAYTIME);
        displayOFF();
    }

    if((min >= 0) && (min < 10)){
        displayDigit(min, DIG0);
        delay(DELAYTIME);
        displayDigit(0, DIG1);
        delay(DELAYTIME);
        displayOFF();
    }

    else if(min >= 10){
        dig0 = min % 10;
        dig1 = min / 10;

        displayDigit(dig0, DIG0);
        delay(DELAYTIME);
        displayDigit(dig1, DIG1);
        delay(DELAYTIME);
        displayOFF();
    }

}

// Special function to display setting the hour or min depending on the value of hourMin.  hourMin = 0 -> minute, 1 -> hour
int dispSetTime(unsigned int time, int hourMin){

    switch(hourMin){

        //  Case 0 handles setting the minute
        case 0:
            if((time >= 0) && (time <= 9)){
                displayDigit(time, DIG0);
                delay(DELAYTIME);
                displayDigit(0, DIG1);
                delay(DELAYTIME);
                displayOFF();
             }

            else if(time >= 10){
                int dig0 = time % 10;
                int dig1 = time / 10;

                displayDigit(dig0, DIG0);
                delay(DELAYTIME);
                displayDigit(dig1, DIG1);
                delay(DELAYTIME);
                displayOFF();
            }

            break;

        //  Case 1 handles setting hour
        case 1:
            if((time >= 0) && (time <= 9)){
                displayDigit(time, DIG2);
                delay(DELAYTIME);
                displayDigit(0, DIG3);
                delay(DELAYTIME);
                displayOFF();
             }

            else if(time >= 10){
                int dig0 = time % 10;
                int dig1 = time / 10;

                displayDigit(dig0, DIG2);
                delay(DELAYTIME);
                displayDigit(dig1, DIG3);
                delay(DELAYTIME);
                displayOFF();
            }

            break;

    }
}


//  Used to display general, non-time numbers
int displayNum(unsigned int number){

    //  The maximum number a four-digit display is 9999.  Need to check for this first
    if(number > 9999){ return -1; }

    //  If number is a single digit
    if((number >= 0) && (number <= 9)){

        displayDigit(number, DIG0);
        delay(DELAYTIME);
    }

    //  If number is double digits
    else if((number > 9) && (number <= 99)){

        int dig0 = number % 10;
        int dig1 = number / 10;
        int del = 0;

        displayDigit(dig0, DIG0);
        delay(DELAYTIME);
        displayDigit(dig1, DIG1);
        delay(DELAYTIME);
        
    }

    else if((number > 99) && (number <= 999)){

        int dig0 = number % 10;
        int dig1 = number % 100 / 10;
        int dig2 = number / 100;
        int del = 0;

        displayDigit(dig0, DIG0);
        delay(DELAYTIME);
        displayDigit(dig1, DIG1);
        delay(DELAYTIME);
        displayDigit(dig2, DIG2);
        delay(DELAYTIME);

    }

    else if((number > 999) && (number <= 9999)){

        int dig0 = number % 10;
        int dig1 = number % 1000 % 100 / 10;
        int dig2 = number % 1000 / 100;
        int dig3 = number / 1000;
        int del = 0;

        displayDigit(dig0, DIG0);
        delay(DELAYTIME);
        displayDigit(dig1, DIG1);
        delay(DELAYTIME);
        displayDigit(dig2, DIG2);
        delay(DELAYTIME);
        displayDigit(dig3, DIG3);
        delay(DELAYTIME);

    }

    displayOFF();
}


//  displayDigit actually switches on/off GPIOs to light segment LEDs
int displayDigit(unsigned int number, int digit){

    if((number > 9) || (number < 0)){ return -1; }

    PORTB = SEGSINKARRAYCOL[number];

    PORTA &= 0xF0;
    PORTA |= SEGDIGARRAY[digit];

    return 0;
}


void displayOFF(){
    PORTB |= 0x7F;
    PORTA &= 0xF0;
}


//  Extremely crude delay function as __delay_x() functions don't seem to want to work here...
int delay(unsigned int delay){
    unsigned int i = 0;
    for(i = 0; i != delay; i++);
}