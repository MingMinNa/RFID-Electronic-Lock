#include "headers/internal_setting.h"
#include "headers/resistor.h"
#include <xc.h>

// Variable Resistor
void resistor_init(){
    TRISAbits.TRISA0 = 1;
    
    ADCON1bits.VCFG0 = 0;   // use Vdd as reference voltage(V-)
    ADCON1bits.VCFG1 = 0;   // use Vss as reference voltage(V+)
    ADCON1bits.PCFG = 0x0E; // AN0(analog), others(digital)
    
    
    ADCON2bits.ADCS = 0b000; // TAD = Fosc/2
    ADCON2bits.ACQT = 0b001; // Tacq = 0b001
    ADCON2bits.ADFM = 0;     // left justified (If use right justified, please change "get_resistor_value" function)
    
    ADCON0bits.CHS = 0b0000; // choose AN0
    ADCON0bits.ADON = 1;     // turn on ADC
    
    interrupt_init();
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    IPR1bits.ADIP = 1;     
    
    ADCON0bits.GO = 1;
}

int get_resistor_value(){
    
    // Left Justified (ADFM = 0)
    return ADRESH;
    
    // Right Justified (ADFM = 1)
    /*
    int val = ((ADRESH & 0b11) << 8) + ADRESL;
       return val;
       */
}