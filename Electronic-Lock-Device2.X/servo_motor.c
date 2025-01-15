
#include "headers/servo_motor.h"
#include "headers/internal_setting.h"
#include <xc.h>
#include <pic18f4520.h>

extern int TMR2_prescaler;      // In Timer.c
extern int frequency;           // In Oscillator.c

int current_degree = 0;

void motor_init(){

    CCP1_init();
    TRISCbits.TRISC2 = 0;
    LATCbits.LATC2 = 0;
    
    /*
    * PWM period = (PR2 + 1) x 4 x Tosc x (TMR2 prescaler)
    *            = (0x9C + 1) x 4 x 2 x 10^(-6) x 16 ~=  0.02s = 20ms
    */
    TMR2_init(16, 1, 0x9C);
    set_degree(-90);
}

void set_degree(int degree){
    
    // Pulse Width: 500 ~ 2400 µs (-90° ~ 90°)
    /* 
    * Duty cycle = (CCPR1L:CCP1CON<5:4>) x Tosc x (TMR2 prescaler)
    *            = ((degree + 90) x (2400 - 500) / (90 - (-90))) + 500
    *            = ((degree + 90) x 95 / 9) + 500
    */
    int corresponding_mus = (int)((degree + 90) * 95 / 9) + 500;
    corresponding_mus = (corresponding_mus) / (2 * TMR2_prescaler);
    
    CCPR1L = ((unsigned char)(corresponding_mus)) / 4;
    CCP1CONbits.DC1B = ((unsigned char)(corresponding_mus)) % 4;
    current_degree = degree;
    
    return;
}