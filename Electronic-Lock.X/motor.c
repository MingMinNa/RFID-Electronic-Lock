#include "headers/internal_setting.h"
#include "headers/motor.h"
#include <xc.h>

// In internal_setting.c
extern int TMR2_prescaler;      
extern int Frequency;     

int current_degree = 0;

void motor_init(){

    // Call Oscillator_Initialize before using Motor
    CCP1_init();
    
    TRISCbits.TRISC1 = 0;
    LATCbits.LATC1 = 0;
    
    // Set up PR2, CCP to decide PWM period and Duty Cycle
    switch(Frequency){
        case _8MHz:
            // No Use This Oscillator Frequency
            break;
        case _4MHz:
            // No Use This Oscillator Frequency
            break;
        case _2MHz:
            // No Use This Oscillator Frequency
            break;
        case _1MHz:
            // No Use This Oscillator Frequency
            break;
        case _500kHz:   
            // % It's Ok, But NO Use Recommended %
            /*
            * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
            * = (0x9b + 1) * 4 * 2탎 * 16
            * = 0.019968s ~= 20ms
            */
            TMR2_init(16, 1, 0x9b);
            break;
        case _250kHz:
            /*
            * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
            * = (0x4d + 1) * 4 * 4탎 * 16
            * = 0.019968s ~= 20ms
            */
            TMR2_init(16, 1, 0x4d);
            break;
        case _125kHz:
            /*
            * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
            * = (0x9b + 1) * 4 * 8탎 * 4
            * = 0.019968s ~= 20ms
            */
            TMR2_init(4, 1, 0x9b);
            break;
        default:
            break;
    }
    set_degree(0);
    
}

void set_degree(int degree){
    
    // Pulse Width: 500 ~ 2400 탎 (-90� ~ 90�)
    
    /*
    * Duty cycle: P.152
    * = (CCPR1L:CCP1CON<5:4>) * Tosc * (TMR2 prescaler)
    */
    
    // Original Version: int corresponding_mus = (int)((degree + 90) * (2400 - 500) / (90 - (-90))) + 500;
    int corresponding_mus = (int)((degree + 90) * 95 / 9) + 500;
    
    switch(Frequency){
        case _8MHz:
            // Tosc = 0.125f;
            // corresponding_mus = (corresponding_mus * 8) / TMR2_prescaler;
            break;
        case _4MHz:
            // Tosc = 0.25f;
            // corresponding_mus = (corresponding_mus * 4) / TMR2_prescaler;
            break;
        case _2MHz:
            // Tosc = 0.5f;
            // corresponding_mus = (corresponding_mus * 2) / TMR2_prescaler;
            break;
        case _1MHz:
            // Tosc = 1.0f;
            // corresponding_mus = (corresponding_mus) / TMR2_prescaler;
            break;
        case _500kHz:
            // Tosc = 2.0f;
             corresponding_mus = (corresponding_mus) / (2 * TMR2_prescaler);
            break;
        case _250kHz:
            // Tosc = 4.0f;
            corresponding_mus = (corresponding_mus) / (4 * TMR2_prescaler);
            break;
        case _125kHz:
            // Tosc = 8.0f;
            corresponding_mus = (corresponding_mus) / (8 * TMR2_prescaler);
            break;
        default:
            break;
    }
    
    CCPR1L = ((unsigned char)(corresponding_mus)) / 4;
    CCP1CONbits.DC1B = ((unsigned char)(corresponding_mus)) % 4;
    current_degree = degree;
    
    return;
}