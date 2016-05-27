#ifndef _MINI_MODBUS_H__
#define _MINI_MODBUS_H__

#include "TypeDef.h"
#include "packet_handler.h"
#include "mini_modbus_tables.h"

enum MINI_MODBUS_RESPONSE_T	{MINI_MODBUS_RESPONSE_OK = 0, MINI_MODBUS_RESPONSE_LOGIC_ERROR = 1, MINI_MODBUS_RESPONSE_ALLIGN_ERROR = 2, MINI_MODBUS_RESPONSE_NO_COMMAND_SUPPORT = 3 };
enum MINI_MODBUS_command_t 
{ 
	MINI_MODBUS_READ_RO_COMMAND = 1, MINI_MODBUS_READ_RW_COMMAND = 3, MINI_MODBUS_WRITE_RW_COMMAND	= 4, 
	MINI_MODBUS_READ_FLASH_BUFFER_COMMAND	= 5, MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND = 6, 
	MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND = 7, MINI_MODBUS_READ_DEVICE_INFO_COMMAND = 2, MINI_MODBUS_SHORT_RESPONCE_COMMAND = 65 
};

extern UI modbus_update_flag;
extern UI modbus_copy_to_ROM_request;
UI data_handler(UI data);
void init_mini_modbus(void);
void RS485_handler_func(UART_packet * pkt);
void response_send(MINI_MODBUS_command_t command, MINI_MODBUS_RESPONSE_T result);
extern UI conn_last_mes_inc_counter;

#endif
