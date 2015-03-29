
/*  Seven Segment Display Library for pic18F microcontrollers
 *
 */

#include "libmWS_sevenSeg.h"

/*The seven segment initialization dedicates port1 */
int sevenSegInit(commonType_t commonType, port_t port1, port_t port2){

    //  Check to see if ports passed are the same.  You cannot have the same ports for port1 and port2!
    if(port1 == port2){ return -1;}

    comType = commonType;
    portDef1 = port1;
    portDef2 = port2;

    /*  In the COMMON ANODE case, port1 will be current sink.  We will set the following initial conditions:
     *  1. Set TRISx to OUTPUT
     *  2. Set all pins in PORTx HIGH except for PORTx[7] which is connected to the ANODE of the colon (ie.  PORTx[7]-----|>--- GND)
     */

    if(commonType == COM_ANODE){
        switch(port1){
            case PORT_A:
                TRISA &= 0xF0;
                PORTA = 0x7F;
                break;

            case PORT_B:
                TRISB = 0x00;
                PORTB = 0x7F;
                break;

            case PORT_C:
                TRISC = 0x00;
                PORTC = 0x7F;
                break;

            case PORT_D:
                TRISD = 0x00;
                PORTD = 0x7F;
                break;

            default:
                return -2;
                break;
        }

        /*  In the COMMON ANODE case, port2 will be the current source.  The initial conditions are:
         *  1. Set TRISy to OUTPUT
         *  2. Set only pins in PORTy[0:3] LOW.  Bitmasking is required to not interfere with PORTy[4:7] as they may have been assigned to something else!
         */

        switch(port2){
            case PORT_A:
                TRISA &= 0xF0;
                PORTA &= 0xF0;
                break;

            case PORT_B:
                TRISB &= 0xF0;
                PORTB &= 0xF0;
                break;

            case PORT_C:
                TRISC &= 0xF0;
                PORTC &= 0xF0;
                break;

            case PORT_D:
                TRISD &= 0xF0;
                PORTD &= 0xF0;
                break;

            default:
                return -3;
                break;
        }

    }

    else if(commonType == COM_CATHODE){

        //  Support for COMMON CATHODE to be added

    }

    return 0;

}

int displayNum(int number, bool colon){

    //  The max value we can display on a 4-digit, seven segment display is 9999
    if(number > 9999){ return -1; }

    if(colon){

        switch(portDef1){

            case PORT_A:
                PORTA |= 0x80;
                break;

            case PORT_B:
                PORTB |= 0x80;
                break;

            case PORT_C:
                PORTC |= 0x80;
                break;

            case PORT_D:
                PORTD |= 0x80;
                break;
        }
    }

    //  If number is a single digit
    if((number >= 0) && (number <= 9)){ displayDigit(number, DIG0); }

    //  If number is between 10 and 99
    if((number > 9) && (number <= 99)){

        int dig0 = number % 10;
        int dig1 = number / 10;

        displayDigit(dig0, DIG0);
        __delay_ms(DELAYTIME);
        displayDigit(dig1, DIG1);
        __delay_ms(DELAYTIME);

        displayOFF();
    }

    if((number > 99) && (number <= 999)){}

    if((number > 999) && (number <= 9999)){}
}

int displayNumTimed(int number, bool colon, int duration){

    
}

int displayDigit(int number, int digit){

    if(comType == COM_ANODE){

        switch(portDef1){

            case PORT_A:
                PORTA |= 0x0F;
                PORTA = CASEGARRAY[number];
                break;

            case PORT_B:
                PORTB |= 0x0F;
                PORTB = CASEGARRAY[number];
                break;

            case PORT_C:
                PORTC |= 0x0F;
                PORTC = CASEGARRAY[number];
                break;

            case PORT_D:
                PORTD |= 0x0F;
                PORTD = CASEGARRAY[number];
                break;

            default:
                return -1;

        }

        switch(portDef2){

            case PORT_A:
                PORTA &= 0xF0;
                PORTA |= CADIGARRAY[digit];
                break;

            case PORT_B:
                PORTB &= 0xF0;
                PORTB |= CADIGARRAY[digit];
                break;

            case PORT_C:
                PORTC &= 0x0F;
                PORTC |= CADIGARRAY[digit];
                break;

            case PORT_D:
                PORTD &= 0xF0;
                PORTD |= CADIGARRAY[digit];
                break;

            default:
                return -2;

        }
    }

    if(comType == COM_CATHODE){

    }

    return 0;
}

void displayOFF(){

    if(comType == COM_ANODE){

        switch(portDef2){

            case PORT_A:
                PORTA = 0x00;
                break;

            case PORT_B:
                PORTB = 0x00;
                break;

            case PORT_C:
                PORTC = 0x00;
                break;

            case PORT_D:
                PORTD = 0x00;
                break;

            default:
                return -1;

        }

        switch(portDef1){

            case PORT_A:
                PORTA = 0x7F;
                break;

            case PORT_B:
                PORTB = 0x7F;
                break;

            case PORT_C:
                PORTC = 0x7F;
                break;

            case PORT_D:
                PORTD = 0x7F;
                break;

            default:
                return -2;
        }
    }

    if(comType == COM_CATHODE){}

}

