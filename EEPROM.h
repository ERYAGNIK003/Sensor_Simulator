#ifndef _EEPROM_H_
#define _EEPROM_H_
#include<stdio.h>
extern int EEPROM_Write(int,unsigned char);
extern void EEPROM_Close(void);
extern unsigned char EEPROM_Read(int);
#endif
