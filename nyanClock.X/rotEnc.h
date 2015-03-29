/* 
 * File:   rotEnc.h
 * Author: superkittens
 *
 * Created on February 22, 2015, 12:04 PM
 */

#ifndef ROTENC_H
#define	ROTENC_H

#include <xc.h>

static int encoderStates[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1,0};
static unsigned char oldState = 0;
unsigned char PCTemp = 0x00;

int rotEncInit();
int readEnc();
int clickEvent();

#endif	/* ROTENC_H */

