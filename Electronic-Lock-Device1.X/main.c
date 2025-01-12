
#include "headers/button.h"
#include "headers/buzzer.h"
#include "headers/internal_setting.h"
#include "headers/resistor.h"
#include "headers/uart.h"
#include "headers/rfid.h"
#include "headers/led.h"
#include "string.h"
#include <xc.h>
#include <pic18f4520.h>

// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)


/*
 1. Mode 1(LED1 is on) and LED2 is on 
 * Registration Mode
 
 2. Mode 1(LED1 is on) and LED2 is off 
 * Deregistration Mode
 
 3. Mode 2(LED1 is off) and LED2 is on
 * Check Mode(Ready)
 
 4. Mode 2(LED1 is off) and LED2 is off
 * Check Mode(Not Ready)
 */
int mode = 1;
extern unsigned char digit;
extern char display_info[MAX_INFO_LEN];
extern int info_len;

void main(void) {
    interrupt_init();
    oscillator_init(_8MHz);
    
    buzzer_init();
    led_init();
    button_init(1);
    resistor_init();
    uart_init();
    rfid_init();
    
    while(1){
        /* Variable Resistor */
        int resistor_value = get_resistor_value();
        if(resistor_value <= 128){
            led_output_digit(digit | (0b01));
            mode = 1;
        }
        else{
            led_output_digit(0b10);
            mode = 2;
        }
    }
    return;
}

/* Interrupt Handlers */
void __interrupt(high_priority) H_ISR(){
    /* INT0 Interrupt */
    if(INTCONbits.INT0IF){
        
        if(mode == 1){
            led_output_digit(digit ^ (0b10));
        }
        __delay_ms(100);
        INTCONbits.INT0IF = 0;
    }
    
    /* UART Read Interrupt */
    if(RCIF){
        if(RCSTAbits.OERR){
            CREN = 0;   // Error happend
            Nop();
            CREN = 1;   // Error completed
        }
        unsigned char ret = rfid_read();
        if(ret == 1){       // Operation Successful
            buzzer_accept();
        }
        else if(ret == 2){  // Operation Fail
            buzzer_reject();
        }
    }
    
    /* TMR1 Interrupt */
    if (PIR1bits.TMR1IF) {
        // Do something
        
        __delay_us(100);
        // TMR1 Interrupt completed
        PIR1bits.TMR1IF = 0;
    }
    
    /* TMR2 Interrupt */
    if (PIR1bits.TMR2IF) {
        // Do something
        
        __delay_us(100);
        // TMR2 Interrupt completed
        PIR1bits.TMR2IF = 0;
    }
    
    return;
}

void __interrupt(low_priority) L_ISR(){
    
    return;
}