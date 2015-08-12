/* 
 * File:   led.h
 * Author: superkittens
 *
 * Created on June 17, 2015, 9:43 PM
 */

#ifndef LED_H
#define	LED_H

//  Macros to enable alarm LEDs (Tristate -> 0)
#define ALM_LED_RED_EN() (TRISEbits.TRISE0 = 0)
#define ALM_LED_GRN_EN() (TRISEbits.TRISE1 = 0)
#define ALM_LED_BLUE_EN() (TRISEbits.TRISE2 = 0)

//  Macros to enable indicator LEDs
#define IND_LED_SETTIME_EN() (TRISAbits.TRISA5 = 0)
#define IND_LED_ALMON_EN() (TRISBbits.TRISB1 = 0)
#define IND_LED_ALMOFF_EN() (TRISBbits.TRISB2 = 0)
#define IND_LED_EXIT_EN()   (TRISBbits.TRISB6 = 0)

//  Alarm LED ON/OFF Macros
#define ALM_LED_RED_ON() (LATEbits.LE0 = 1)
#define ALM_LED_GRN_ON() (LATEbits.LE1 = 1)
#define ALM_LED_BLUE_ON() (LATEbits.LE2 = 1)
#define ALM_LED_RED_OFF() (LATEbits.LE0 = 0)
#define ALM_LED_GRN_OFF() (LATEbits.LE1 = 0)
#define ALM_LED_BLUE_OFF() (LATEbits.LE2 = 0)

//  Indicator LED ON/OFF Macros
#define IND_LED_SETTIME_ON() (LATAbits.LA5 = 1)
#define IND_LED_ALMON_ON() (LATBbits.LB2 = 0)     //  Port B1 must be a sink
#define IND_LED_ALMOFF_ON() (LATBbits.LB1 = 0)    //  Port B2 must be a sink
#define IND_LED_EXIT_ON() (LATBbits.LB6 = 1);
#define IND_LED_SETTIME_OFF() (LATAbits.LA5 = 0)
#define IND_LED_ALMON_OFF() (LATBbits.LB2 = 1)
#define IND_LED_ALMOFF_OFF() (LATBbits.LB1 = 1)
#define IND_LED_EXIT_OFF()  (LATBbits.LB6 = 0)

int enableLED(){
    
    ALM_LED_RED_EN();
    ALM_LED_GRN_EN();
    ALM_LED_BLUE_EN();
    
    IND_LED_SETTIME_EN();
    IND_LED_ALMON_EN();
    IND_LED_ALMOFF_EN();
    IND_LED_EXIT_EN();
    
    ALM_LED_RED_OFF();
    ALM_LED_GRN_OFF();
    ALM_LED_BLUE_OFF();
    
    IND_LED_SETTIME_OFF();
    IND_LED_ALMON_OFF();
    IND_LED_ALMOFF_OFF();
}

int ALL_ALM_LED_ON(){
    ALM_LED_RED_ON();
    ALM_LED_GRN_ON();
    ALM_LED_BLUE_ON();
    
}

int ALL_ALM_LED_OFF(){
    ALM_LED_RED_OFF();
    ALM_LED_GRN_OFF();
    ALM_LED_BLUE_OFF();
}

int ALL_IND_LED_OFF(){
    IND_LED_SETTIME_OFF();
    IND_LED_ALMON_OFF();
    IND_LED_ALMOFF_OFF();
    IND_LED_EXIT_OFF();
}

void toggleLED(unsigned int sequence){
    ALL_ALM_LED_OFF();
    switch(sequence){
        case 0:
            ALM_LED_RED_ON();
            break;
            
        case 1:
            ALM_LED_GRN_ON();
            break;
            
        case 2:
            ALM_LED_BLUE_ON();
            break;
            
        case 3:
            ALM_LED_RED_ON();
            ALM_LED_GRN_ON();
            break;
            
        case 4:
            ALM_LED_GRN_ON();
            ALM_LED_BLUE_ON();
            break;
            
        case 5:
            ALL_ALM_LED_ON();
            break;
            
        default:
            ALL_ALM_LED_OFF();
            break;
    }
}
#endif	/* LED_H */

