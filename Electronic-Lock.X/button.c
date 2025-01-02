#include "headers/button.h"
#include "headers/internal_setting.h"
#include <xc.h>
#include <pic18f4520.h>

void button_init(int use_Interrupt){
    ADCON1 = 0x0E;
    TRISBbits.TRISB1 = 1;
    LATBbits.LATB1 = 0;
    
    // if use_Interrupt != 0, use INT0
    if (use_Interrupt != 0){
        interrupt_init();
        INT0_open();
    }
}

// the function is used when check the button pressed by using polling(busy waiting) 
unsigned char button_pressed(){
    return (PORTBbits.RB0 == 0);
}