
#ifndef INTERRUPT_H
#define	INTERRUPT_H

# define _8MHz 0b111
# define _4MHz 0b110
# define _2MHz 0b101
# define _1MHz 0b100
# define _500kHz 0b011
# define _250kHz 0b010
# define _125kHz 0b001

#define _XTAL_FREQ 8000000

// Timer
void TMR1_init(int prescaler, int init_val);
void TMR2_init(int prescaler, int postscaler, unsigned char _PR2);
void TMR1_restart();

// CCP
void CCP1_init();

// Oscillator
void oscillator_init(int _frequency);

// Interrupt
void interrupt_init();
void INT0_open();

#endif	/* INTERRUPT_H */

