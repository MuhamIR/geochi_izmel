#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__
#include "stm32f0xx.h"
#include "RingBuffer.h"
#include "System.h"
#include "packet_def.h"



typedef RingBuffer<U8> SerialRingBuffer;

class C_UART
{
	public:
		C_UART(UI rec_buf_size = 512, UI tr_buf_size = 512);
		void set_hardware(USART_TypeDef * hard_uart, ULONG baud = 38400);
		void send_in_packet ( U8 * p_data, UI size);
		void send_string_in_packet ( const char * str);	
		void put_array(U8 * buf, UI length );		
		void puts(const char * str );
		void interrupt_handle(void);
		U8 getc(void);
		bool is_empty(void){return rec_RB.IsEmpty();}
	protected:	
		USART_TypeDef * _hard_uart;
		SerialRingBuffer  tr_RB;
		SerialRingBuffer  rec_RB;
		void start_transmit(void);
		void put_spec( U8 data);
};

typedef C_UART C_RS232;

class C_RS485 : public C_UART
{
	public:
		C_RS485(UI rec_buf_size = 512, UI tr_buf_size = 512):C_UART( rec_buf_size, tr_buf_size){}
		void set_hardware(USART_TypeDef * hard_uart, ULONG baud = 38400);	
};

#endif
