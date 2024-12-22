
#ifndef BUTTON_H
#define	BUTTON_H

#define BUTTON_PRESSED (PORTBbits.RB0 == 0)
void button_init();

#endif	/* BUTTON_H */

