#include "GPIO.h"


void C_GPIO::init_pin(GPIO_TypeDef * port, U16 pin)
{
	_pin = (U16)1 << pin;
	_port = port;
	assert_param(IS_GET_GPIO_PIN(_pin));
	_pin_number = pin;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_Init(_port, &GPIO_InitStructure);	
	_port->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (_pin_number * 2));
  _port->OSPEEDR |= ((uint32_t)(GPIO_Speed_Level_3) << (_pin_number * 2));
}

void C_GPIO::change_state(void)
{
	if (GPIO_ReadOutputDataBit(_port, _pin))
		GPIO_ResetBits(_port, _pin);
	else		
		GPIO_SetBits(_port, _pin);
}


void C_GPIO::set_mode(GPIOMode_TypeDef mode)
{
	_port->MODER  &= ~(3UL << (_pin_number * 2));
	_port->MODER |= (((uint32_t)mode) << (_pin_number * 2));
}


void C_GPIO::set_as_input(GPIOPuPd_TypeDef type)
{
	_port->PUPDR  &= ~(3UL << (_pin_number * 2));
	_port->PUPDR |= (((uint32_t)type) << (_pin_number * 2));
	_port->MODER  &= ~(3UL << (_pin_number * 2));
	_port->MODER |= (((uint32_t)GPIO_Mode_IN) << (_pin_number * 2));	
}

void C_GPIO::set_as_output(GPIOOType_TypeDef type)
{
	_port->MODER  &= ~(3UL << (_pin_number * 2));
	_port->MODER |= (((uint32_t)GPIO_Mode_OUT) << (_pin_number * 2));
	_port->OTYPER  &= ~(1UL << (_pin_number));
	_port->OTYPER |= (((uint32_t)type) << (_pin_number));
}

void C_GPIO::set_state(UI state)
{
	if (state)
		set();
	else
		clear();
}


