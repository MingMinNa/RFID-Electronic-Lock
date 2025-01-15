
#ifndef UART_H
#define	UART_H

#define MAX_INFO_LEN 50

void uart_init();
unsigned char uart_read();
void uart_write(unsigned char data);
void clear_buffer();
void screen_display();

#endif	/* UART_H */

