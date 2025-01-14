#include "headers/button.h"
#include "headers/internal_setting.h"
#include <xc.h>
#include <pic18f4520.h>

void button_init(int use_Interrupt){
    ADCON1 = 0x0E;
    TRISBbits.TRISB1 = 1;
    LATBbits.LATB1 = 0;
    INT0_open();
}