
#include "headers/uart.h"
#include <xc.h>

void uart_init(void) {
    
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;            

    //  Setting baud rate
    // The below code for Oscillator: _8MHz, Baud Rate: 9600
    TXSTAbits.SYNC = 0;
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 12; 

    // Serial enable
    RCSTAbits.SPEN = 1;              
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;

    // Transmitter 
    PIE1bits.TXIE = 0; // interrupte enable
    IPR1bits.TXIP = 0; // priority  

    // Receiver
    PIE1bits.RCIE = 1; // interrupte enable
    IPR1bits.RCIP = 0; // priority
}

unsigned char uart_read(){
    return RCREG;
}

void uart_write(unsigned char data){
    while(!TXSTAbits.TRMT);
    TXREG = data;
}