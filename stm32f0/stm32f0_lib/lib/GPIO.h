#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f0xx.h"
#include "System.h"
#include "delay.h"

class C_GPIO 
{
	public:
		C_GPIO(){};
	  	C_GPIO(GPIO_TypeDef * port, U16 pin){init_pin(port, pin);};			
	  	void init_pin(GPIO_TypeDef * port, U16 pin);		
		void set_as_input(GPIOPuPd_TypeDef type = GPIO_PuPd_NOPULL);
		void set_as_output(GPIOOType_TypeDef type = GPIO_OType_PP);	
		inline void set(void){_port->BSRR = _pin;};			
		inline void clear(void){_port->BRR = _pin;};
		void set_state(UI state);		
		UI get_state(void){ return (_port->IDR & _pin)? 1 : 0; };
		void change_state(void);	
		void set_mode(GPIOMode_TypeDef mode);
	protected:	
		GPIO_TypeDef * _port;
		U16 _pin;
		U16 _pin_number;
};

#endif

