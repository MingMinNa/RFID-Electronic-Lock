
#include "headers/uart.h"
#include <xc.h>
#include <pic18f4520.h>

char display_info[MAX_INFO_LEN];
int info_len = 0;

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
    // No use transmitter interrupt
    // PIE1bits.TXIE = 1; // interrupte enable
    // IPR1bits.TXIP = 1; // priority  

    // Receiver
    PIE1bits.RCIE = 1;    // interrupte enable
    IPR1bits.RCIP = 1;    // priority
    
    for(int i = 0; i < MAX_INFO_LEN; ++i)
        display_info[i] = '\0';
    info_len = 0;
}

unsigned char uart_read(){
    return RCREG;
}

void clear_buffer(){
    display_info[0] = '\0';
    info_len = 0;
}