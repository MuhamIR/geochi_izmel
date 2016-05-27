#include "UART_driver.h"
#include "CRC.h"
#include "macros.h"

static const U32 CR1_TXE_EN = BIT7;
static const U32 CR1_RXNE_EN = BIT5;

static const U32 ISR_TXE_EN = BIT7;
static const U32 ISR_RXNE_EN = BIT5;


C_UART::C_UART(UI rec_buf_size, UI tr_buf_size):
_hard_uart(NULL),
tr_RB(tr_buf_size),
rec_RB(rec_buf_size)
{
}

void C_UART::set_hardware(USART_TypeDef * hard_uart, ULONG baud)
{
	_hard_uart = hard_uart;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if (_hard_uart == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
	else if (_hard_uart == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}

	
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(_hard_uart, &USART_InitStructure);
	
	_hard_uart->CR1 &= ~(BIT26 | BIT27 | BIT14 | BIT13 | BIT10 | BIT8 | BIT6 | BIT4);
	_hard_uart->CR1 |= CR1_RXNE_EN;
	USART_Cmd(_hard_uart, ENABLE);
	
}
void C_UART::put_spec( U8 data)
{
	if	( data == START_PAKET_SIMBOL)
	{
		tr_RB.Write(ESC_SIMBOL);
		tr_RB.Write(ALT_START);
	}
	else if  (data == END_PAKET_SIMBOL)
	{
		tr_RB.Write(ESC_SIMBOL);
		tr_RB.Write(ALT_END);
	}
	else if  (data == ESC_SIMBOL)
	{
		tr_RB.Write(ESC_SIMBOL);
		tr_RB.Write(ALT_ESC);
	}
	else 
	{
		tr_RB.Write(data);	
	}
}

void C_UART::send_in_packet ( U8 * p_data, UI size)
{
	UI crc = Crc16(p_data, size);
	tr_RB.Write(START_PAKET_SIMBOL);
	while(size--)
		put_spec( *p_data++ );	
	put_spec( crc & 0xff);
	put_spec( (crc >> 8) & 0xff);
	tr_RB.Write(END_PAKET_SIMBOL);	
	start_transmit();
}

void C_UART::send_string_in_packet (const char * str)
{
	send_in_packet ( (U8 *) str, strlen(str));	
}

void C_UART::put_array(U8 * buf, UI length )
{
	while(length--)
	{
		tr_RB.Write(*buf++);
	} 	
	start_transmit();
}		
void C_UART::puts( const char * str )
{
	put_array((U8 *) str, strlen(str));
}



void C_UART::interrupt_handle(void)
{
	U32 isr = _hard_uart->ISR;
	
	if(isr & ISR_RXNE_EN)
	{
		U8 temp = _hard_uart->RDR;
		rec_RB.Write(temp);
	}

	if ((isr & ISR_TXE_EN) && (_hard_uart->CR1 & CR1_TXE_EN))
	{   		
		if (tr_RB.IsEmpty())
		{
			_hard_uart->CR1 &= ~CR1_TXE_EN;
		}
		else 
		{
			U8 temp = 0;
			tr_RB.Read(temp);
			_hard_uart->TDR = temp;
		}
	}
	_hard_uart->ICR |= 0x121B5F ;
}
U8 C_UART::getc(void)
{
	U8 c = 0;	
 	rec_RB.Read(c);
 	return c;
}
void C_UART::start_transmit(void)
{
	_hard_uart->CR1 |= CR1_TXE_EN;
}

void C_RS485::set_hardware(USART_TypeDef * hard_uart, ULONG baud)
{
	C_UART::set_hardware(hard_uart, baud);
	USART_Cmd(_hard_uart, DISABLE);
	USART_DECmd(_hard_uart,ENABLE);
	USART_DEPolarityConfig(_hard_uart, USART_DEPolarity_High);
	USART_SetDEAssertionTime(_hard_uart,16); 
	USART_SetDEDeassertionTime(_hard_uart,16);
	USART_Cmd(_hard_uart, ENABLE);	
}



