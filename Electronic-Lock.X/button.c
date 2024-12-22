#include "button.h"
#include "internal_setting.h"
#include <xc.h>

void Button_Initialize(int use_Interrupt){
    
    ADCON1 = 0x0E;          // must change 
    TRISBbits.TRISB1 = 1;
    LATBbits.LATB1 = 0;
    
    // if use_Interrupt != 0, use INT1
    if (use_Interrupt != 0){
        interrupt_init();
        INT1_open();
    }
}