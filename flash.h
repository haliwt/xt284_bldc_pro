#ifndef __FLASH_H__
#define __FLASH_H__

void FLASH_UnLock(void);
void FLASH_Lock(void);
void FLASH_Write(uint8_t FLASHModule, uint16_t Addr, uint8_t Data);
uint8_t FLASH_Read(uint8_t FLASHModule, uint16_t Addr);
void FLASH_Erase(uint8_t FLASHModule, uint16_t Addr);


void	eeprom_write(FLASH_TYPES  *in);
void  eeprom_read(FLASH_TYPES  *out);

#endif




