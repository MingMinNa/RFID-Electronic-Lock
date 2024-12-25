#include "headers/led.h"
#include <xc.h>
#include <pic18f4520.h>

unsigned char digit = 0;

void led_init(){
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    
    digit = 0b11;
    LATAbits.LATA1 = digit % 2;
    LATAbits.LATA2 = (digit / 2) % 2;
    
}

void led_output_digit(unsigned char _digit){
    if( 0 <= digit && digit <= 3){
        digit = _digit;
        LATAbits.LATA1 = digit % 2;
        LATAbits.LATA2 = (digit / 2) % 2;
    }
}