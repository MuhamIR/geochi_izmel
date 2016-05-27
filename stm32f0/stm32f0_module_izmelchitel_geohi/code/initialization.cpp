#include "SPI_slave.h"
#include "mini_modbus.h"
#include "filters.h"
#include "GPIO.h"

C_RS485 rs485(256,256);
extern void RS485_handler_func(UART_packet * pkt);
UART_handler rs485_handler(&rs485, RS485_handler_func);
C_GPIO led(GPIOA, 5);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
CExponentialSmooth filters[OUTPUTS_NUM];




void gpio_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF |RCC_AHBPeriph_GPIOD |RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOA, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
	
//USART2	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);	
//address detect pins
}

C_GPIO outputs[OUTPUTS_NUM];
void outputs_init(void)
{
	outputs[0].init_pin(GPIOA, GPIO_PinSource8);
	outputs[1].init_pin(GPIOB, GPIO_PinSource10);
	outputs[2].init_pin(GPIOB, GPIO_PinSource4);	
	outputs[3].init_pin(GPIOB, GPIO_PinSource5);
	
	
	
	outputs[4].init_pin(GPIOF, GPIO_PinSource1);
	outputs[5].init_pin(GPIOF, GPIO_PinSource0);
	outputs[6].init_pin(GPIOC, GPIO_PinSource13);
	outputs[7].init_pin(GPIOB, GPIO_PinSource7);
	
	for (UI i = 0; i < OUTPUTS_NUM; i++)
	{
		outputs[i].set_mode(GPIO_Mode_OUT);
	}
}


void initialization(void)
{
	SystemCoreClockUpdate ();
	

	rs485.set_hardware(USART2, 115200);
	short_delay(10000);
	
	gpio_init();
	
	led.set_as_output();
	if (SysTick_Config(SystemCoreClock / 10000))
		dtrap();
	
	extern void init_mini_modbus(void);
	init_mini_modbus();

//	extern void ADC_Config(void);
//	ADC_Config();
	
	outputs_init();

	
// 	while(1)
// 	{
// 		rs485.send_string_in_packet("123");
// 		short_delay(100000);
// 	}
	
}

