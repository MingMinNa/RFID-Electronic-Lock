
#ifndef RFID_H
#define	RFID_H

unsigned char database_empty();
unsigned char database_full();

void rfid_init();
unsigned char rfid_read();
unsigned char insert_ID();
unsigned char remove_ID();
unsigned char check_ID();
unsigned char find_ID();

#endif	/* RFID_H */

