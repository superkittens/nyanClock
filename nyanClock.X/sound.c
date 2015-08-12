#include <xc.h>
#include "sound.h"
#include "delay.h"

//  Initialization functions: setup MIDI
int soundInit(){

    //  UART Setup
    TRISCbits.TRISC6 = 0;   //  Set C6 to output (TX)
    TXSTA1bits.TX9 = 0;     //  8-bit TX
    TXSTA1bits.SYNC = 0;    //  Asynchronous mode
    TXSTA1bits.BRGH = 1;    //  High speed BRG Mode
    RCSTA1bits.SPEN = 1;    //  Enable RX
    BAUDCON1bits.BRG16 = 0; //  No 16-bit mode
    BAUDCON1bits.RXDTP = 1;
    SPBRG1 = 31;            //  Baud Rate generator calculated value
    TXSTA1bits.TXEN = 1;    //  Enale UART
    
    //  Timer 3 has been ruled out in favour of using Timer 0 as its larger prescaler options make things easier in lengthy delays
    /*
    //  Timer 3 setup
    T3GCONbits.TMR3GE = 0;      //  Turn off TMR3 gating feature
    T3CONbits.TMR3CS = 0b00;    //  Clock Source = Instruction Clock (FOSC/4))
    T3CONbits.T3CKPS = 0b11;       //  No prescaler
    T3CONbits.RD16 = 1;         //  16-bit mode
    T3CONbits.TMR3ON = 0;       //  TMR3 off
    
    //  Timer 3 interrupt configs
    PIE2bits.TMR3IE = 1;        //  Enable TMR3 OVF interrupt
    IPR2bits.TMR3IP = 0;        //  TMR3 OVF set to low priority
    */
    
    //  Set amplifier SHDN pin tristate to output and shut off amplifier
    AMP_SHDN_TRIS = 0;
    AMP_SHDN = 0;
    
    //  VS1053B Reset line uses RC2 which must be configured to such that the analog line is detached
    ANCON1bits.PCFG11 = 1;
    //  Set VS1053B Reset pin tris to output
    VS1053B_RST_TRIS = 0;
    
    return 0;
}

void enableAmplifier(){
    AMP_SHDN = 1;
}

void disableAmplifier(){
    AMP_SHDN = 0;
}

void vsHoldRST(){
    VS1053B_RST = 0;
}

void vsReleaseRST(){
    VS1053B_RST = 1;
}

int setTempo(){
    float tempo = 100.0;
    //  The standard measure of time is a quarter note.
    //  Tempo is in Beats per Minute
    if(tempo < 60.0){
        return -1;
    }
    
    float baseTime = 1/((tempo / 60.0)) * 1000;
    
    quarter = (unsigned int)baseTime;
    half = quarter * 2;
    whole = quarter * 4;
    eighth = quarter / 2;
    sixteenth = quarter / 4;
    
    return 0;
}

static void txMIDI(char cmd, char data1, char data2){
    
    TXREG1 = cmd;
    while(!TXSTA1bits.TRMT);
    TXREG1 = data1;
    while(!TXSTA1bits.TRMT);
    
    if((cmd & 0xF0) <= 0xB0){
        TXREG1 = data2;
        while(!TXSTA1bits.TRMT);
    }
}

void setBank(char channel, char bank){
    //  0x00 = Bank Select
    txMIDI(((char)0xB0 | channel), (char)0x00, bank);
}

void setVolume(char channel, char volume){
    //  0x07 = Volume change
    txMIDI(((char)0xB0) | channel, (char)0x07, volume);
}

void setInstrumet(char channel, char instrument){
    txMIDI((char)0xC0 | channel, instrument, (char)0x00);
}

void playNote(char channel, char note){
    //  0x90 = Play note, last (char)127 parameter is the attack value
    txMIDI((char)0x90 | channel, note, (char)127);
}

void setupForPlayback(){
    
    vsHoldRST();
    enableAmplifier();
    setTempo();
    setBank((char)0x00, (char)0x00);
    setBank((char)0x00, (char)0x00);
    setVolume((char)0x00, volume);
    setVolume((char)0x01, volume);
    scoreIndex = 0;
    vsReleaseRST();
    
}

static unsigned int parseDuration(char duration){
    switch(duration){
        
        case 'q':
            return quarter;
            break;
            
        case 'h':
            return half;
            break;
            
        case 'w':
            return whole;
            break;
            
        case 'e':
            return eighth;
            break;
            
        case 's':
            return sixteenth;
            break;
            
        default:
            return -1;
            break;      
    }
}

void playback(){
    static int block = 0;
    char note1, note2;
    unsigned int dur1, dur2;
    unsigned int grandDuration = 0;
    
    if(block == 0){
        if(score1Count <= (score1Repeat - 1)){  
            if(scoreIndex <= (score1Length - 1)){
                note1 = score1[scoreIndex][0];
                dur1 = parseDuration(score1[scoreIndex][1]);
                note2 = score1[scoreIndex][2];
                dur2 = parseDuration(score1[scoreIndex][3]);
            
                //  Determine the shortest duration between the two parsed durations.  This will be the delay used.
                if(dur1 <= dur2){
                    grandDuration = dur1;
                }
            
                else if(dur2 < dur1){
                    grandDuration = dur2;
                }
            
                scoreIndex++;
                if(scoreIndex > (score1Length - 1)){
                    score1Count++;
                    scoreIndex = 0;
                }
            
                playNote(0, note1);
                playNote(1, note2);
            
                genDelay_ms(grandDuration);
            } 
        }
        
        else{
            block = 1;
            score1Count = 0;
            scoreIndex = 0;
        }
    }
    
    if(block == 1){
        if(score2Count <= (score2Repeat - 1)){
            if(scoreIndex <= (score2Length - 1)){
                note1 = score2[scoreIndex][0];
                dur1 = parseDuration(score2[scoreIndex][1]);
                note2 = score2[scoreIndex][2];
                dur2 = parseDuration(score2[scoreIndex][3]);
            
                //  Determine the shortest duration between the two parsed durations.  This will be the delay used.
                if(dur1 <= dur2){
                    grandDuration = dur1;
                }
            
                else if(dur2 < dur1){
                    grandDuration = dur2;
                }
            
                scoreIndex++;
                if(scoreIndex > (score2Length - 1)){
                    score2Count++;
                    scoreIndex = 0;
                }
            
                playNote(0, note1);
                playNote(1, note2);
            
                genDelay_ms(grandDuration);
            }
        }
        
        else{
            block = 0;
            score2Count = 0;
            scoreIndex = 0;
        }
        
    }
    
    
    
}

void resetPlaybackParams(){
    
    score1Count = 0;
    scoreIndex = 0;
}


