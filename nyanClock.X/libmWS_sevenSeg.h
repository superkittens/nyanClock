/* 
 * File:   libmWS_sevenSeg.h
 * Author: superkittens
 *
 * Created on February 5, 2015, 10:24 PM
 */

#ifndef LIBMWS_SEVENSEG_H
#define	LIBMWS_SEVENSEG_H


#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

typedef enum {COM_ANODE, COM_CATHODE} commonType_t;
typedef enum {PORT_A, PORT_B, PORT_C, PORT_D} port_t;
enum {DIG0, DIG1, DIG2, DIG3};

const int DELAYTIME = 50;
const unsigned char CASEGARRAY[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};
const unsigned char CADIGARRAY[4] = {0x01, 0x02, 0x04, 0x08};

commonType_t comType;
port_t portDef1;
port_t portDef2;

int sevenSegInit(commonType_t, port_t, port_t);
int displayNum(int number, bool colon);
int displayNumTimed(int number, bool colon, int duration);
int displayChar(char letter);
int displayDigit(int number, int digit);
void displayOFF();

#endif	/* LIBMWS_SEVENSEG_H */

