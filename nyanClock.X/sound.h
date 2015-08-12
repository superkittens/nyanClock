/* 
 * File:   sound.h
 * Author: superkittens
 *
 * Created on July 27, 2015, 8:08 PM
 */

#ifndef SOUND_H
#define	SOUND_H

#define AMP_SHDN_TRIS (TRISCbits.TRISC7)
#define AMP_SHDN (LATCbits.LC7)
#define VS1053B_RST_TRIS (TRISCbits.TRISC2)
#define VS1053B_RST (LATCbits.LC2)

int soundInit();
void enableAmplifier();
void disableAmplifier();
void vsHoldRST();
void vsReleaseRST();
int setTempo();
void setVolume(char channel, char volume);
void setBank(char channel, char bank);
void setInstrument(char channel, char instrument);
void playNote(char channel, char note);
void setupForPlayback();
void playback();
void resetPlaybackParams();

static void txMIDI(char cmd, char data1, char data2);
static unsigned int parseDuration(char duration);

unsigned int numInstruments = 2;
//float tempo = 100.0;
unsigned int whole, half, quarter, eighth, sixteenth;
char volume = 127;
int exitAlarmFlag = 0;
static int scoreIndex = 0;

//  These scores may be better suited as formal data structures
//.............don't judge me, Chris!
unsigned int score1Length = 53;
unsigned int score1Repeat = 2;
unsigned int score1Count = 0;
char score1[53][4] = {
    {78, 'q', 52, 'q'},
    {80, 'q', 64, 'q'},
    {74, 'e', 54, 'q'},
    {75, 'e', 0, 'e'},
    {0, 'e', 66, 'q'},
    {71, 'e', 0, 'e'},
    {74, 'e', 51, 'q'},
    {73, 'e', 0, 'e'},
    {71, 'q', 63, 'q'},
    {71, 'q', 56, 'q'},
    {71, 'e', 68, 'q'},
    {73, 'e', 0, 'q'},
    {74, 'q', 49, 'q'},
    {74, 'e', 61, 'q'},
    {73, 'e', 0, 'e'},
    {71, 'e', 54, 'q'},
    {73, 'e', 0, 'e'},
    {75, 'e', 66, 'q'},
    {78, 'e', 0, 'e'},
    {80, 'e', 47, 'q'},
    {75, 'e', 0, 'e'},
    {78, 'e', 59, 'q'},
    {73, 'e', 0, 'e'},
    {75, 'e', 47, 'q'},
    {71, 'e', 0, 'e'},
    {73, 'e', 59, 'q'},
    {71, 'e', 0, 'e'},
    {75, 'q', 52, 'q'},
    {78, 'q', 64, 'q'},
    {80, 'e', 54, 'q'},
    {75, 'e', 0, 'e'},
    {78, 'e', 66, 'q'},
    {73, 'e', 0, 'e'},
    {75, 'e', 51, 'q'},
    {71, 'e', 0, 'e'},
    {73, 'e', 63, 'q'},
    {75, 'e', 0, 'e'},
    {73, 'e', 56, 'q'},
    {71, 'e', 0, 'e'},
    {68, 'e', 68, 'q'},
    {71, 'e', 0, 'e'},
    {74, 'q', 49, 'q'},
    {71, 'e', 61, 'q'},
    {73, 'e', 0, 'e'},
    {75, 'e', 54, 'q'},
    {78, 'e', 0, 'e'},
    {73, 'e', 66, 'q'},
    {75, 'e', 0, 'e'},
    {73, 'e', 47, 'q'},
    {71, 'e', 0, 'e'},
    {68, 'q', 59, 'q'},
    {71, 'q', 47, 'q'},
    {0, 'q', 59, 'q'}
};

unsigned int score2Length = 55;
unsigned int score2Repeat = 2;
unsigned int score2Count = 0;
char score2[55][4] = {
    {71, 'q', 52, 'q'},
    {66, 'e', 56, 'q'},
    {68, 'e', 0, 'e'},
    {71, 'q', 59, 'q'},
    {66, 'e', 63, 'q'},
    {68, 'e', 0, 'e'},
    {71, 'e', 51, 'q'},
    {73, 'e', 0, 'e'},
    {75, 'e', 54, 'q'},
    {71, 'e', 0, 'e'},
    {76, 'e', 59 , 'q'},
    {75, 'e', 0, 'e'},
    {76, 'e', 63, 'q'},
    {78, 'e', 0, 'e'},
    {71, 'q', 49, 'q'},
    {66, 'e', 52, 'q'},
    {68, 'e', 0, 'e'},
    {71, 'q', 56, 'q'},
    {66, 'e', 59, 'q'},
    {68, 'e', 0, 'e'},
    {76, 'e', 47, 'q'},
    {75, 'e', 0, 'e'},
    {73, 'e', 51, 'q'},
    {71, 'e', 0, 'e'},
    {66, 'e', 54, 'e'},
    {63, 'e', 0, 'e'},
    {64, 'e', 59, 'q'},
    {66, 'e', 0, 'e'},
    {71, 'q', 52, 'q'},
    {66, 'e', 56, 'q'},
    {68, 'e', 0, 'e'},
    {71, 'q', 59, 'q'},
    {66, 'e', 63, 'q'},
    {68, 'e', 0, 'e'},
    {71, 'e', 51, 'q'},
    {71, 'e', 0, 'e'},
    {73, 'e', 54, 'q'},
    {75, 'e', 0, 'e'},
    {71, 'e', 59, 'q'},
    {66, 'e', 0, 'e'},
    {68, 'e', 63, 'q'},
    {66, 'e', 0, 'e'},
    {71, 'q', 49, 'q'},
    {71, 'e', 52, 'q'},
    {70, 'e', 0, 'e'},
    {71, 'e', 56, 'q'},
    {66, 'e', 0, 'e'},
    {68, 'e', 59, 'q'},
    {70, 'e', 0, 'e'},
    {76, 'e', 47, 'q'},
    {75, 'e', 0, 'e'},
    {76, 'e', 51, 'q'},
    {78, 'e', 0, 'e'},
    {71, 'q', 54, 'q'},
    {70, 'q', 59, 'q'}
};




#endif	/* SOUND_H */

