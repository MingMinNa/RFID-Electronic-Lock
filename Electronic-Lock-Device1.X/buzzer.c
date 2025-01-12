#include "headers/internal_setting.h"
#include "headers/buzzer.h"
#include <xc.h>
#include <pic18f4520.h>

void buzzer_init() {
    TMR2_init(16, 1, 124);
    
    CCP1CONbits.CCP1M = 0b1100;
    CCP1CONbits.DC1B = 0;
    buzzer_off();
    
    T2CONbits.TMR2ON = 1;
    TRISCbits.TRISC2 = 0;
}

void buzzer_on(unsigned char accept){
    if(accept == 1)
        PR2 = 100;
    else
        PR2 = 255;
    CCPR1L = 62;
}

void buzzer_off() {
    CCPR1L = 0;  
}

void buzzer_accept(){
    LATAbits.LATA2 = 0;
    for(int i = 0; i < 2; ++i){
        buzzer_on(1);
        __delay_ms(250);
        buzzer_off();
        __delay_ms(100);
    }
    LATAbits.LATA2 = 1;
}

void buzzer_reject(){
    LATAbits.LATA2 = 0;
    buzzer_on(0);
    __delay_ms(750);
    buzzer_off();
    LATAbits.LATA2 = 1;
}