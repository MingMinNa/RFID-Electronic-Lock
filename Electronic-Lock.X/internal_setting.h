
#ifndef INTERRUPT_H
#define	INTERRUPT_H

# define _8MHz 0b111
# define _4MHz 0b110
# define _2MHz 0b101
# define _1MHz 0b100
# define _500kHz 0b011
# define _250kHz 0b010
# define _125kHz 0b001

// Timer
void TMR1_init();
void TMR2_init();
void restart_TMR1();

// CCP
void CCP1_init();
void CCP2_init();

// Oscillator
void oscillator_init(int frequency);

// Interrupt
void interrupt_init();
void INT0_open();
void INT1_open();

#endif	/* INTERRUPT_H */

