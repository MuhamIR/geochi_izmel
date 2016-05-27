#include "stm32f0xx_it.h"
#include "UART_driver.h"
#include "macros.h"


VULONG systick_counter = 0;
volatile UI systick_flag = 0;

extern "C"
{

void SysTick_Handler(void)
{
	systick_counter++;
	systick_flag++;	
	extern UI spi_last_mes_time;
	spi_last_mes_time++;
}


void USART1_IRQHandler(void)
{
	extern C_RS485 rs485;
	rs485.interrupt_handle();
}

void USART2_IRQHandler(void)
{
	extern C_RS485 rs485;
	rs485.interrupt_handle();
}
}





