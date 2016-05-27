#include "mini_modbus.h"
#include "UART_driver.h"
#include "packet_handler.h"
#include "CRC.h"

extern C_RS485 rs485;
extern U16 Crc16(const U8 *pcBlock, UI len);
UI conn_last_mes_inc_counter = MAX_TIME_BETWEEN_MES;
UI modbus_update_flag = 1;
UI modbus_copy_to_ROM_request = 0;


namespace
{
void mini_modbus_read_from_flash(void)
{
	for(UI i = 0; i < MINI_MODBUS_RW_FLASH_TABLE_SIZE; i++)
		mini_modbus_RW_flash_memory[i] = mini_modbus_RW_real_flash_memory[i];
}



enum MINI_MODBUS_command_type_t {READ_TABLE, WRITE_TABLE, SPECIAL_COMMAND, UNKNOWN}; 

MINI_MODBUS_command_type_t get_type(MINI_MODBUS_command_t cmd)
{
	switch ( cmd )
	{
		case MINI_MODBUS_READ_RO_COMMAND:
		case MINI_MODBUS_READ_RW_COMMAND:
		case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
		case MINI_MODBUS_READ_DEVICE_INFO_COMMAND:
			return READ_TABLE;
		case MINI_MODBUS_WRITE_RW_COMMAND:
		case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
			return WRITE_TABLE;
		case MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND:
		case MINI_MODBUS_SHORT_RESPONCE_COMMAND:
			return SPECIAL_COMMAND;
		default:
			return UNKNOWN;
	}
}

struct modbus_table_t
{
	U16 * array;
	UI size;
};

inline modbus_table_t get_write_table_info(MINI_MODBUS_command_t cmd)
{
	modbus_table_t table;
	switch (cmd)
	{
		case MINI_MODBUS_WRITE_RW_COMMAND:
			table.size = MINI_MODBUS_RW_TABLE_SIZE;
			table.array = mini_modbus_RW_memory;
			break;
		case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
			table.size = MINI_MODBUS_RW_FLASH_TABLE_SIZE;
			table.array = mini_modbus_RW_flash_memory;
			break;
		default:
			table.size = 0;
			table.array = NULL;
	}
	return table;
}

const UI MINI_MODBUS_DEVICE_INFO_TABLE_SIZE = 12;


U16 device_info_array[MINI_MODBUS_DEVICE_INFO_TABLE_SIZE] = 
{
	MINI_MODBUS_GENERAL_ID,
	((U16)(SOFTWARE_VERSION & 0xff) << 8 ) | (SOFTWARE_SUBVERSION & 0xff),
	HASH_CODE,
	MINI_MODBUS_RO_TABLE_SIZE,
	MINI_MODBUS_RW_TABLE_SIZE,
	MINI_MODBUS_RW_FLASH_TABLE_SIZE
};

modbus_table_t get_read_table_info(MINI_MODBUS_command_t cmd)
{
	modbus_table_t table;
	switch (cmd)
	{
		case MINI_MODBUS_READ_RO_COMMAND:
			table.size = MINI_MODBUS_RO_TABLE_SIZE;
			table.array = mini_modbus_RO_memory;
			break;
		case MINI_MODBUS_READ_RW_COMMAND:
			table.size = MINI_MODBUS_RW_TABLE_SIZE;
			table.array = mini_modbus_RW_memory;
			break;
		case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
			table.size = MINI_MODBUS_RW_FLASH_TABLE_SIZE;
			table.array = mini_modbus_RW_flash_memory;
			break;
		case MINI_MODBUS_READ_DEVICE_INFO_COMMAND:
			table.size = 6;
			table.array = device_info_array;
			break;
		default:
			table.size = 0;
			table.array = NULL;
	}
	return table;
}

}//namespace

void mini_modbus_write_to_flash(void)
{
	FLASH_Unlock();
	FLASH_ErasePage(FLASH_DATA_WITH_MODIFY);
	U16 * ptr = (U16 *) FLASH_DATA_WITH_MODIFY;
	for(UI i = 0; i < MINI_MODBUS_RW_FLASH_TABLE_SIZE; i++)
	{
		FLASH_ProgramHalfWord( (U32)ptr++, mini_modbus_RW_flash_memory[i]);
	}
	FLASH_Lock();
}

volatile UI spi_last_mes_time = 1000;

volatile UI spi_word_counter = 0;
volatile bool spi_error_packet_flag = 0;
volatile UI spi_command = 0;
volatile UI spi_address = 0;
U8 crc_data[4];
UI acc_crc;
UI data_address;
UI data_size;
U16 * data_ptr;
int data_counter;
U16 spi_buf[128];

const UI SPI_NORMAL = 0x5555;
const UI SPI_ERROR = 0x3333;


enum modbus_spi_state_t {MODE_IS_UNKNOWN, WORD_MODE, SEQ_WRITE, SEQ_READ};
modbus_spi_state_t spi_state = MODE_IS_UNKNOWN;
MINI_MODBUS_command_t add_cmd;

UI word_mode_handler(UI data)
{
	UI res = SPI_ERROR;
	if (spi_error_packet_flag == 0)
	{
		if (spi_word_counter == 0)
		{
			crc_data[0] = data;
			crc_data[1] = data>>8;
			if ( ((data >> 12) ^ MINI_MODBUS_ID)  & 0xf)
				spi_error_packet_flag = 1;
			//ильнур, почти маска 4 бита
			spi_command = (data >> 8) & 0x0f;
			spi_address = data & 0xff;
			switch(spi_command)
			{
				case MINI_MODBUS_READ_RO_COMMAND:
					res = mini_modbus_RO_memory[spi_address];
					crc_data[2] = res;
					crc_data[3] = res>>8;
					break;
				case MINI_MODBUS_READ_RW_COMMAND:
					res = mini_modbus_RW_memory[spi_address];
					crc_data[2] = res;
					crc_data[3] = res>>8;
					break;
				case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
					res = mini_modbus_RW_flash_memory[spi_address];
					crc_data[2] = res;
					crc_data[3] = res>>8;
					break;
				
				case MINI_MODBUS_WRITE_RW_COMMAND:
				case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
					res = SPI_NORMAL;
					break;
				case MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND:
					if (spi_address == 0)
						res = SPI_NORMAL;
					else
					{
						res = SPI_ERROR;
						spi_error_packet_flag = 1;
					}
					break;
				default:
					res = SPI_ERROR;
					spi_error_packet_flag = 1;
					break;
			}
		}
		else if (spi_word_counter == 1)
		{
			switch(spi_command)
			{
				case MINI_MODBUS_READ_RO_COMMAND:
				case MINI_MODBUS_READ_RW_COMMAND:
				case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
					res = Crc16(crc_data, 4);
					crc_data[2] = res;
					crc_data[3] = res>>8;
					break;
				
				case MINI_MODBUS_WRITE_RW_COMMAND:
				case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
					crc_data[2] = data;
					crc_data[3] = data>>8;
					res = SPI_NORMAL;
					break;
				case MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND:
					if (data == 0)
					{
						crc_data[2] = 0;
						crc_data[3] = 0;
						res = SPI_NORMAL;
					}
					else
					{
						res = SPI_ERROR;
						spi_error_packet_flag = 1;
					}
					break;
				default:
					res = SPI_ERROR;
					spi_error_packet_flag = 1;
					break;
			}
		}
		else if (spi_word_counter == 2)
		{
			switch(spi_command)
			{
				case MINI_MODBUS_READ_RO_COMMAND:
				case MINI_MODBUS_READ_RW_COMMAND:
				case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
					res = SPI_NORMAL;
					conn_last_mes_inc_counter = 0;
					break;
				
				case MINI_MODBUS_WRITE_RW_COMMAND:
				case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
				case MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND:
					if (Crc16(crc_data, 4) == data)
					{
						switch(spi_command)
						{
							case MINI_MODBUS_WRITE_RW_COMMAND:
								{
									U16 temp = crc_data[2] | ((UI)crc_data[3]<<8);
									if (mini_modbus_RW_memory[spi_address] != temp)
									{
										mini_modbus_RW_memory[spi_address] = temp;
										modbus_update_flag = 1;
									}
								}
								break;
							case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
								mini_modbus_RW_flash_memory[spi_address] = crc_data[2] | ((UI)crc_data[3]<<8);
								modbus_update_flag = 1;
								break;
							case MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND:
								//mini_modbus_write_to_flash();
								modbus_copy_to_ROM_request = 1;
								break;
						}
						res = SPI_NORMAL;
						conn_last_mes_inc_counter = 0;
					}
					else
						res = SPI_ERROR;
					break;
				default:
					spi_error_packet_flag = 1;
					res = SPI_ERROR;
					break;
			}
		}
		
		if (++spi_word_counter == 3)
		{
			spi_word_counter = 0;
			spi_state = MODE_IS_UNKNOWN;
		}
	}
	else
		res = SPI_ERROR;
	return res;
}









static inline void calcCRC (U16  word)
{
	acc_crc = calcCRC16 (acc_crc, word);
	acc_crc = calcCRC16 (acc_crc, word >> 8);
}

inline UI read_next(void)
{
	UI res = *data_ptr++;
	data_counter--;
	calcCRC (res);
	spi_word_counter++;
	return res;
}

inline UI write_next(UI data)
{
	*data_ptr++ = data;
	data_counter--;
	calcCRC (data);
	spi_word_counter++;
	return data;
}



UI data_handler(UI data)
{
	UI res = SPI_ERROR;
	if (spi_last_mes_time > 30)
	{
		spi_word_counter = 0;
		spi_error_packet_flag = false;
	}
	
	if (spi_error_packet_flag == false)
	{
		if ((spi_word_counter == 0) || (spi_state == MODE_IS_UNKNOWN))
		{
			spi_word_counter = 0;
			spi_state = MODE_IS_UNKNOWN;
		}
		switch (spi_state)
		{
			case MODE_IS_UNKNOWN:
				spi_command = (data >> 8) & 0x0f;
				if (spi_command == 0xf)
				{
					acc_crc = 0;
					calcCRC (data);
					add_cmd = (MINI_MODBUS_command_t)(data & 0xFF);
					switch ( add_cmd )
					{
						case MINI_MODBUS_READ_RO_COMMAND:
						case MINI_MODBUS_READ_RW_COMMAND:
						case MINI_MODBUS_READ_FLASH_BUFFER_COMMAND:
						case MINI_MODBUS_READ_DEVICE_INFO_COMMAND:
							spi_state = SEQ_READ;
							res = SPI_NORMAL;
							break;
						case MINI_MODBUS_WRITE_RW_COMMAND:
						case MINI_MODBUS_WRITE_FLASH_BUFFER_COMMAND:
							spi_state = SEQ_WRITE;
							res = SPI_NORMAL;
							break;
						default:
							spi_state = MODE_IS_UNKNOWN;
							break;
					}
					spi_word_counter++;
				}
				else
				{
					spi_state = WORD_MODE;
					res = word_mode_handler(data);
				}
				
				break;
			case WORD_MODE:
				res = word_mode_handler(data);
				break;
			case SEQ_WRITE:
				if (spi_word_counter == 1)
				{
					calcCRC (data);
					data_address = (data >> 8) & 0xff;
					data_size = (data >> 0) & 0xff;
					
					modbus_table_t table = get_write_table_info(add_cmd);
					
					if ((table.array == NULL) || ((data_address + data_size) > table.size))
					{
						spi_error_packet_flag = true;
						goto spi_handler_end;
					}

					data_ptr = spi_buf;
					data_counter = data_size;
					spi_word_counter++;
					res = SPI_NORMAL;
				}
				else 
				{
					if (data_counter > 0)
					{
						res = write_next(data);
					}
					else if (data_counter == 0)
					{
						res = acc_crc;
						if (data == acc_crc)
						{
							//записать данные
							modbus_table_t table = get_write_table_info(add_cmd);
							data_ptr = table.array + data_address;
							for(UI i = 0; i < data_size; i++)
							{
								data_ptr[i] = spi_buf[i];
							}
							modbus_update_flag = 1;
						}
						
						data_counter = -1;
					}
					else if (data_counter == -1)
					{
						res = SPI_NORMAL;
						spi_state = MODE_IS_UNKNOWN;
						conn_last_mes_inc_counter = 0;
					}
				}
				break;
			case SEQ_READ:
				if (spi_word_counter == 1)
				{
					calcCRC (data);
					data_address = (data >> 8) & 0xff;
					data_size = (data >> 0) & 0xff;
					
					modbus_table_t table = get_read_table_info(add_cmd);
					
					if ((table.array == NULL) || ((data_address + data_size) > table.size))
					{
						spi_error_packet_flag = true;
						goto spi_handler_end;
					}

					data_ptr = table.array + data_address;
					data_counter = data_size;
					res = read_next();
				}
				else 
				{
					if (data_counter > 0)
					{
						res = read_next();
					}
					else if (data_counter == 0)
					{
						res = acc_crc;
						data_counter = -1;
					}
					else if (data_counter == -1)
					{
						res = SPI_NORMAL;
						spi_state = MODE_IS_UNKNOWN;
						conn_last_mes_inc_counter = 0;
					}
				}
				break;
			default:
				spi_state = MODE_IS_UNKNOWN;
				break;
		}
	}
spi_handler_end:
	spi_last_mes_time = 0;
	return res;
};

void response_send(MINI_MODBUS_command_t command, MINI_MODBUS_RESPONSE_T result)
{
	if (rs485_response_flag)
	{
		U8 mas[4];
		mas[0] = MINI_MODBUS_ID;
		mas[1] = MINI_MODBUS_SHORT_RESPONCE_COMMAND;
		mas[2] = command;
		mas[3] = result;
		rs485.send_in_packet(mas,4);
	}
}

void init_mini_modbus(void)
{
	mini_modbus_read_from_flash();
}

static void write_memory( UART_packet * pkt )
{
	U8 * ptr = &pkt->_data[1];
	MINI_MODBUS_command_t command = (MINI_MODBUS_command_t)*ptr++;
	UI address = *ptr++;
	UI size = *ptr++;
	
	modbus_table_t table = get_write_table_info(command);
	UI table_size = table.size;
	U8 * write_ptr = (U8*)table.array;
	
	if (write_ptr == NULL)
	{
		response_send(command, MINI_MODBUS_RESPONSE_LOGIC_ERROR);
		return;
	}
	if (table_size == 0)
	{
		response_send(command, MINI_MODBUS_RESPONSE_ALLIGN_ERROR);
		return;
	}
	write_ptr += 2 * address;
	if (2*size == (pkt->_size - 4) )
	{
		if ((address + size) <= table_size)
		{
			UI loop = 2*size;
			while(loop--)
			{
				if (*write_ptr != *ptr)
					modbus_update_flag = 1;
				*write_ptr++ = *ptr++;
			}
			response_send(command, MINI_MODBUS_RESPONSE_OK);
			
			return;
		}
		else
		{
			response_send(command, MINI_MODBUS_RESPONSE_ALLIGN_ERROR);
			return;
		}
	}
	else
	{
		response_send(command, MINI_MODBUS_RESPONSE_LOGIC_ERROR);
		return;
	}
}


static void read_memory( UART_packet * pkt )
{
	U8 * ptr = &pkt->_data[1];
	MINI_MODBUS_command_t command = (MINI_MODBUS_command_t)*ptr++;
	UI address = *ptr++;
	UI size = *ptr++;
	

	modbus_table_t table = get_read_table_info(command);
	UI table_size = table.size;
	U8 * read_ptr = (U8*)table.array;
	
	if (read_ptr == NULL)
	{
		response_send(command, MINI_MODBUS_RESPONSE_LOGIC_ERROR);
		return;
	}
	if (table_size == 0)
	{
		response_send(command, MINI_MODBUS_RESPONSE_ALLIGN_ERROR);
		return;
	}
	
	read_ptr += 2 * address;
	
	if ((pkt->_size - 4) == 0)
	{
		if ((address + size) <= table_size)
		{			
			UI loop = 2*size;
			U8 * array = new U8[2*size+4];
			U8 * write_ptr = array;
			*write_ptr++ = MINI_MODBUS_ID;
			*write_ptr++ = command;
			*write_ptr++ = address;
			*write_ptr++ = size;
			while(loop--)
				*write_ptr++ = *read_ptr++;
			rs485.send_in_packet(array,2*size+4);
			delete[] array;
			return;
		}
		else
		{
			response_send(command, MINI_MODBUS_RESPONSE_ALLIGN_ERROR);
			return;
		}
	}
	else
	{
		response_send(command, MINI_MODBUS_RESPONSE_LOGIC_ERROR);
		return;
	}
}

void RS485_handler_func(UART_packet * pkt)
{
	U8 * ptr = pkt->_data;
	if (*ptr++ == MINI_MODBUS_ID)
	{
		conn_last_mes_inc_counter = 0;
		MINI_MODBUS_command_t cmd = (MINI_MODBUS_command_t)*ptr;		
		switch (get_type(cmd))
		{
			case WRITE_TABLE:
				write_memory( pkt );
				break;
			case READ_TABLE:
				read_memory( pkt );
				break;
			case SPECIAL_COMMAND:
				if (cmd == MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND)
				{
					mini_modbus_write_to_flash();
					response_send(MINI_MODBUS_WRITE_FLASH_FROM_BUFFER_COMMAND, MINI_MODBUS_RESPONSE_OK);
				}
				break;
			case UNKNOWN:
				response_send(cmd, MINI_MODBUS_RESPONSE_NO_COMMAND_SUPPORT);
				break;
		}
	}
}



