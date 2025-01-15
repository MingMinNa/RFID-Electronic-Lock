#include "headers/internal_setting.h"
#include "headers/resistor.h"
#include <xc.h>
#include <pic18f4520.h>

// Variable Resistor
void resistor_init(){
    TRISAbits.TRISA0 = 1;
    
    ADCON1bits.VCFG0 = 0;    // use Vdd as reference voltage(V-)
    ADCON1bits.VCFG1 = 0;    // use Vss as reference voltage(V+)
    ADCON1bits.PCFG = 0x0E;  // AN0(analog), others(digital)
    ADCON0bits.CHS = 0b0000; // choose AN0
    
    ADCON2bits.ADCS = 0b001;
    ADCON2bits.ACQT = 0b001;
    ADCON0bits.ADON = 1;     // turn on ADC
    ADCON2bits.ADFM = 0;     // left justified (If use right justified, please change "get_resistor_value" function)
    
    PIE1bits.ADIE = 0;       // No use ADC Interrupt
    
    ADCON0bits.GO = 1;
}

int get_resistor_value(){
    
    ADCON0bits.CHS = 0b0000; // Analog Channel Select Bit: choose AN0
    ADCON0bits.GO = 1;
    
    while (ADCON0bits.GO_nDONE);
    
    // Left Justified (ADFM = 0)
    // "The left justified format provides an 8-bit result in the ADRESH register"
    return ADRESH;
    
    // Right Justified (ADFM = 1)
    // "The right justified format provides the 10-bit converted result in ADRESH:ADRESL 
    //  with the upper two bits in bits 1, 0 of the ADRESH and lower eight bits in ADRESL"
    
    // int val = ((ADRESH & 0b11) << 8) | ADRESL;
    // return val;
}
