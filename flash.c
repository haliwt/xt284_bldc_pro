#include<include.h>
#include "my_type.h"

#define   EEPROM_ADDR_START	     0x00

void FLASH_UnLock(void)
{
	MLOCK = 0xAA;
}
/*****************************************************************************
 ** \brief	FLASH_Lock
 **			ÉÏËø
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void FLASH_Lock(void)
{
	MLOCK = 0x00;
}

/*****************************************************************************
 ** \brief	FLASH_Write
 **			Ð´»º´æ
 ** \param [in] FLASHModule	£º(1)FLASH_DATA
 **							  (2)FLASH_CODE
 **				Addr: 16bit	  (1)FLASH_DATA_ADDR: 0x00~0x3FF;
 **							  (2)FLASH_CODE_ADDR: 0x00~0x3FFF;	
 **				Data: 8bit
 ** \return  none
 ** \note	 
*****************************************************************************/
void FLASH_Write(uint8_t FLASHModule, uint16_t Addr, uint8_t Data)
{
	if( !(MCTRL & 0x20))
	{
		MDATA = Data;
		MADRL = Addr;
		MADRH = Addr>>8;
		MCTRL = FLASH_WRITE | FLASHModule;	
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();	
	}	
}

/*****************************************************************************
 ** \brief	FLASH_Read
 **			¶Á»º´æ
 ** \param [in]  FLASHModule£º(1)FLASH_DATA
 **							  (2)FLASH_CODE
 **				Addr: 16bit	  (1)FLASH_DATA_ADDR: 0x00~0x3FF;
 **							  (2)FLASH_CODE_ADDR: 0x00~0x3FFF;	
 ** \return  8bit Data
 ** \note	
*****************************************************************************/
uint8_t FLASH_Read(uint8_t FLASHModule, uint16_t Addr)
{
	MADRL = Addr;
	MADRH = Addr>>8;
	MCTRL = FLASH_READ | FLASHModule;	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	return (MDATA);
}

/*****************************************************************************
 ** \brief	FLASH_Erase
 **			²Á³ý
 ** \param [in]  FLASHModule£º(1)FLASH_DATA
 **							  (2)FLASH_CODE
 **				Addr: 16bit	  (1)FLASH_DATA_ADDR: 0x00~0x3FF;
 **							  (2)FLASH_CODE_ADDR: 0x00~0x3FFF;
 ** \return  none
 ** \note	
*****************************************************************************/
void FLASH_Erase(uint8_t FLASHModule, uint16_t Addr)
{
	MADRL = Addr;
	MADRH = Addr>>8;
	MCTRL  = FLASH_ERASE | FLASHModule;	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void	eeprom_write(FLASH_TYPES  *in)
{
	FLASH_UnLock();
	FLASH_Erase(FLASH_DATA,EEPROM_ADDR_START);
	FLASH_Write(FLASH_DATA,EEPROM_ADDR_START,in->pole);
	FLASH_Write(FLASH_DATA,EEPROM_ADDR_START+1,in->pwm_set.one.h);
	FLASH_Write(FLASH_DATA,EEPROM_ADDR_START+2,in->pwm_set.one.l);
	FLASH_Lock();
}

void  eeprom_read(FLASH_TYPES  *out)
{
	FLASH_UnLock();
	out->pole = FLASH_Read(FLASH_DATA,EEPROM_ADDR_START);
	out->pwm_set.one.h = FLASH_Read(FLASH_DATA,EEPROM_ADDR_START+1);
	out->pwm_set.one.l = FLASH_Read(FLASH_DATA,EEPROM_ADDR_START+2);
	FLASH_Lock();
}


