#include "packet_handler.h"
#include "packet_def.h"
#include "CRC.h"

#define COMPLETE	0
#define DATA_READY 	1
#define LOCKED		2


C_UART_ReceivePacket::C_UART_ReceivePacket( ) : UART_packet()
{
	_flag = COMPLETE;
	_esc_flag = 0;
}

UI  C_UART_ReceivePacket::add(U8 data)
{
	if ((data == START_PAKET_SIMBOL) && (_esc_flag == 0) )
	{
		_size = 0;
		_flag = LOCKED;
	}
	else if( _flag == LOCKED )
	{
		if (data == END_PAKET_SIMBOL) 
		{
			if (_size >= PAKET_MIN_SIZE)
			{
				U16 crc_calc = Crc16(_data,_size - 2) ;
				U16 crc_rec =  ((UI)_data[_size - 1]  << 8) | (UI)_data[_size - 2];
				_size -= 2;
				if (crc_calc == crc_rec	)
					_flag = DATA_READY;
				else
					_flag = COMPLETE;
			}
			else
				_flag = COMPLETE;

		}
		else if  ( (_size < (UART_MAX_PACKET_SIZE - 1) ))
		{
			if ( _esc_flag )
			{
				_esc_flag = 0;
				if (data == ALT_ESC)
					_data[_size] = ESC_SIMBOL;
				else if (data == ALT_START)
					_data[_size] = START_PAKET_SIMBOL;
				else if (data == ALT_END)
					_data[_size] = END_PAKET_SIMBOL;
				else
					_flag = COMPLETE;
				_size++;
			}
			else
			{
				if (data == ESC_SIMBOL)
				{
					_esc_flag = 1;
				}
				else
				{
					_data[_size++] = data;
				}
			}
		}
		else 
		{
			_flag = COMPLETE;
		}
	}
	return _flag;
}

void UART_handler::handler(void)
{
	while (not _uart->is_empty())
	{
		if (add(_uart->getc()) == DATA_READY)
		{
			_flag = COMPLETE;
			_handler(this);
		}
	}
}




