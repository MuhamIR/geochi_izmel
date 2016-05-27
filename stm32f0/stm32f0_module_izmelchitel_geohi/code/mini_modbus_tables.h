#ifndef _MINI_MODBUS_TABLES_H_
#define _MINI_MODBUS_TABLES_H_
#include "TypeDef.h"


#define MINI_MODBUS_GENERAL_ID 0x1205
#define SOFTWARE_VERSION 2
#define SOFTWARE_SUBVERSION 0
#define HASH_CODE 0x0000

#define MAX_TIME_BETWEEN_MES	5000

const UI OUTPUTS_NUM =	8;

//STATUS_BITS
//ADC error(EOC pin unnormal behevior)
#define ADC_ERROR	((U16)1 << 0)
//NO CONNECTION error
#define NO_CONN_ERROR	((U16)2 << 1)
//FLASH erroe
#define FLASH_ERROR	((U16)1 << 1)


__packed struct RO_table_t
{
	U32 number_to_execute;
	U16 outputs_state;
//	U8 short_currents[OUTPUTS_NUM];						// 0 - 3	Обработанное значение АЦП: выкл, КЗ, обрыв, ток А*100
	U16 statusReg;										// 			Регистр состояния: бит0-ошибка АЦП, бит1-нет связи, бит2-ошибка в калибровочных значениях, бит3-ошибка в температурном сенсоре
//	U16 ADCValues[OUTPUTS_NUM];								// 7-22		Последнее значение АЦП
	U16 timeLO;												// 23			Время младший регистр
	U16 timeHI;												// 24			Время старший регистр
};



struct RW_table_t {
	U32 toggle:1;
	U32 pause:1;
	U32 number:30;
};

struct channel_timing_t
{
	U16 on_offset;
	U16 on_delay;
};

struct ROM_table_t {
	U16 netID : 8;													// новый ID
	U16 rs485_responce_enable : 1;	
	U16 reserve : 7;	
	U16 period;
	U32 default_number_to_execute;
	channel_timing_t channel_timings[OUTPUTS_NUM];
};

extern RO_table_t & RO_table;
extern RW_table_t & RW_table;
extern ROM_table_t & ROM_table;

#define MINI_MODBUS_RW_TABLE_SIZE	(sizeof(RW_table_t)+1)/2
#define MINI_MODBUS_RO_TABLE_SIZE	(sizeof(RO_table_t)+1)/2
#define MINI_MODBUS_RW_FLASH_TABLE_SIZE	(sizeof(ROM_table_t)+1)/2

extern U16 mini_modbus_RW_memory[MINI_MODBUS_RW_TABLE_SIZE];
extern U16 mini_modbus_RO_memory[MINI_MODBUS_RO_TABLE_SIZE];
extern U16 mini_modbus_RW_flash_memory[MINI_MODBUS_RW_FLASH_TABLE_SIZE];
extern const volatile U16 mini_modbus_RW_real_flash_memory[512];

#define MINI_MODBUS_ID (ROM_table.netID)
#define rs485_response_flag (ROM_table.rs485_responce_enable)

#define FLASH_DATA_WITH_MODIFY  0x08007C00


#endif
