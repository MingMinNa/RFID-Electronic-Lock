
#include "internal_setting.h"
#include <xc.h>


int TMR1_prescaler = 1;
int TMR1_init_val = 0;

int TMR2_prescaler = 1;
int TMR2_postscaler = 1;

int Frequency = 0;

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
    IPR1bits.TMR1IP = 0;  
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
    IPR1bits.TMR2IP = 0;  
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
    T2CONbits.TMR2ON = 1;
}

void restart_TMR1(){
    T1CONbits.TMR1ON = 0;
    TMR1H = (unsigned char)(TMR1_init_val / 0xFF);
    TMR1L = (unsigned char)(TMR1_init_val % 0xFF);
    T1CONbits.TMR1ON = 1;
}


/* CCP */
// For RFID
void CCP1_init(){
    
}

// For Motor
void CCP2_init(){
    CCP2CONbits.CCP2M = 0b1100;
}

/* Oscillator */
void oscillator_init(int frequency){
    // OSCCON: P.32
    Frequency = frequency;
    IRCF2 = (unsigned char)(frequency / 4) % 2;
    IRCF1 = (unsigned char)(frequency / 2) % 2;
    IRCF0 = (unsigned char)(frequency) % 2;
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
void INT1_open(){
    // set INT1 as high priority interrupt currently
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IP = 1;
}

/* Interrupt Handlers */
void __interrupt(high_priority) H_ISR(){
    
    if(INTCONbits.INT0IF){
        
        
        __delay_us(30);
        INTCONbits.INT0IF = 0;
    }
    
    if(INTCON3bits.INT1IF){
        
        
        __delay_us(30);
        INTCON3bits.INT1IF = 0;
    }
    
    return;
}

void __interrupt(low_priority) L_ISR(){
    
     // TMR1 Interrupt
    if (PIR1bits.TMR1IF) 
    {
        // Do something
        
        __delay_us(100);
        // TMR1 Interrupt completed
        restart_TMR1();
        PIR1bits.TMR1IF = 0;
    }
    
    // TMR2 Interrupt
    if (PIR1bits.TMR2IF) 
    {
        // Do something
        
        __delay_us(100);
        // TMR2 Interrupt completed
        PIR1bits.TMR2IF = 0;
    }
    
    return;
}