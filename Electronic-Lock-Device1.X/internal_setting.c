
#include "headers/internal_setting.h"
#include "headers/uart.h"
#include <xc.h>
#include <pic18f4520.h>

// TMR1
int TMR1_prescaler = 1;
int TMR1_init_val = 0;
int TMR1_repeat = 0;

// TMR2
int TMR2_prescaler = 1;
int TMR2_postscaler = 1;
int TMR2_repeat = 0;

// Oscillator
int frequency = 0;

/* Timer */
void TMR1_init(int prescaler, int init_val){
    /*
    delay 0.5s(TMR1_Interrupt) in different oscillator frequency
        8MHz    : prescaler(8), init_val(65536 - 62500 = 3036)  => Must use another counter from 0 ~ 1 (2 times)
        4MHz    : prescaler(8), init_val(65536 - 62500 = 3036)
        2MHz    : prescaler(8), init_val(65536 - 31250 = 34286)
        1MHz    : prescaler(8), init_val(65536 - 15625 = 49911)
        500kHz  : prescaler(8), init_val(65536 - 7812 = 57724)
        250kHz  : prescaler(8), init_val(65536 - 3906 = 61630)
        125kHz  : prescaler(8), init_val(65536 - 1953 = 63583)
    */
    
    PIR1bits.TMR1IF = 0; 
    IPR1bits.TMR1IP = 1;  
    PIE1bits.TMR1IE = 1; 
    
    TMR1H = (unsigned char)(init_val / 0xFF);
    TMR1L = (unsigned char)(init_val % 0xFF);
            
    T1CON &= 0xF0;
    // prescaler: 1, 2, 4, 8
    if(prescaler == 2 || prescaler == 8)   T1CONbits.T1CKPS1 = 1;
    else                                   T1CONbits.T1CKPS1 = 0;
    if(prescaler == 4 || prescaler == 8)   T1CONbits.T1CKPS0 = 1;
    else                                   T1CONbits.T1CKPS0 = 0;
    
    TMR1_prescaler = prescaler;
    TMR1_init_val = init_val;
    
    T1CONbits.TMR1ON = 1;
}

void TMR1_restart(){
    T1CONbits.TMR1ON = 0;
    TMR1H = (unsigned char)(TMR1_init_val / 0xFF);
    TMR1L = (unsigned char)(TMR1_init_val % 0xFF);
    T1CONbits.TMR1ON = 1;
}

void TMR2_init(int prescaler, int postscaler, unsigned char _PR2){
    /*
    delay 0.5s(TMR2_Interrupt) in different oscillator frequency
        8MHz    : prescaler(16), postscaler(16), _PR2(39)  => Must use another counter from 0 ~ 99 (100 times) 
        4MHz    : prescaler(16), postscaler(16), _PR2(195) => Must use another counter from 0 ~ 9 (10 times)
        2MHz    : prescaler(16), postscaler(16), _PR2(97)  => Must use another counter from 0 ~ 9 (10 times)
        1MHz    : prescaler(16), postscaler(16), _PR2(49)  => Must use another counter from 0 ~ 9 (10 times)
        500kHz  : prescaler(16), postscaler(16), _PR2(244)
        250kHz  : prescaler(16), postscaler(16), _PR2(122)
        125kHz  : prescaler(16), postscaler(16), _PR2(61)
    */
    
    PIR1bits.TMR2IF = 0; 
    IPR1bits.TMR2IP = 1;  
    PIE1bits.TMR2IE = 1; 
    
    // prescaler: 1 ~ 16, postscaler: 1, 4, 16
    if(prescaler == 1)      T2CONbits.T2CKPS = 0b00;
    else if(prescaler == 4) T2CONbits.T2CKPS = 0b01;
    else if(prescaler == 16)T2CONbits.T2CKPS = 0b11;
    else                    T2CONbits.T2CKPS = 0b00;
    T2CONbits.T2OUTPS = (unsigned char)(postscaler - 1);
    
    TMR2_prescaler = prescaler;
    TMR2_postscaler = postscaler;
    
    PR2 = _PR2;
}


/* CCP */
void CCP1_init(){
    CCP1CONbits.CCP1M = 0b1100;
}


/* Oscillator */
void oscillator_init(int _frequency){
    frequency = _frequency;
    IRCF2 = (unsigned char)(_frequency / 4) % 2;
    IRCF1 = (unsigned char)(_frequency / 2) % 2;
    IRCF0 = (unsigned char)(_frequency) % 2;
}

/* Interrupt */
void interrupt_init(){
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;    //enable low priority interrupt
}

void INT0_open(){
    // INT0 is high priority interrupt
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
}