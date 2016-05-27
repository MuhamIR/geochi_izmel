#include "max1031.h"
#include "delay.h"

#define MAX1031_SINGLE_CONV_TIME 25

void MAX1031::config( CMasterSPI * spi, ssel_t ssel)
{
	_ssel = ssel;
	_spi = spi;
	reconfig();
}


void MAX1031::reconfig( void)
{
	_state =  MAX1031_IDLE;
	_temperature = 0;
	_spi->num_of_bit_set(16);
	_spi->transmit ( 0x6A00, _ssel); //setup
	short_delay(1);
	_spi->transmit ( 0x6B00, _ssel); //setup
	short_delay(1);
	_spi->num_of_bit_set(8);
	_spi->transmit ( 1 << 5 , _ssel);	 //no average
	_spi->transmit ( 0x68, _ssel);
}

MAX1031::MAX1031( GPIO_TypeDef * a_port, UI a_pin_num, CMasterSPI * spi, ssel_t ssel)
{
	init_power_pin(a_port,a_pin_num);
	short_delay(5);
	config(spi, ssel);
}

void MAX1031::init_N_EOC_pin(GPIO_TypeDef * a_port, UI a_pin_num)
{
	_N_EOC_pin.init_pin(a_port, a_pin_num);
	_N_EOC_pin.set_as_input(GPIO_PuPd_UP);
}

bool MAX1031::conversion_is_complete(void)
{
	return !_N_EOC_pin.get_state();
}


void MAX1031::init_power_pin(GPIO_TypeDef * a_port, UI a_pin_num)
{
	_power_pin.init_pin(a_port, a_pin_num);
	_power_pin.set_as_output();
	power_enable();
	_power_state = true;
}

void MAX1031::power_state_set(bool state)
{
	if (state)
		power_enable();
	else
		power_disable();
}

// UI MAX1031::temperature_measure(void)
// {
// 	if (_state == MAX1031_IDLE)
// 	{
// 		_spi->transmit ( 0x87 , _ssel); //single conversion  with temperature info
// 		short_delay(50);
// 		UI temperature = ((USHORT)(_spi->transmit( 0x00, _ssel)&0x0f) << 8 );
// 		temperature |= ((USHORT)(_spi->transmit( 0x00, _ssel)) );
// 		_spi->transmit( 0x00, _ssel);
// 		_spi->transmit( 0x00, _ssel);
// 		_temperature = 	temperature;
// 		return temperature;
// 	}
// 	dtrap();
// 	return 0xffffffff;
// }

int MAX1031::start_measure(MAX1031_chan_t ch)
{
	if (_state == MAX1031_IDLE)
	{
		_spi->transmit ( (0x86 | (ch << 3)), _ssel); //single conversion 
		_state = MAX1031_SINGLE_START;
		_last_ch = ch;
		return 0;
	}
	return -1;
}
int MAX1031::complete_measure(void)
{
	if (_state == MAX1031_SINGLE_START) 	 // if conv was started and ...
	{
 		USHORT ADC_res = (USHORT)(_spi->transmit( 0x00, _ssel) & 0x0F) << 8 ;
		ADC_res |= (USHORT)(_spi->transmit( 0x00, _ssel) & 0xFF) ;
		_state = MAX1031_IDLE;
		_data[_last_ch] = ADC_res;
		return (ADC_res);
	}	
	return -1;
}

int MAX1031::full_measure(MAX1031_chan_t ch)
{
	start_measure(ch);
	wait();
	return complete_measure();
}

int MAX1031::start_multy_measure(MAX1031_chan_t ch)
{
	if (_state == MAX1031_IDLE)
	{
		_spi->transmit ( (0x80 | (ch << 3) ) , _ssel); 
		_state = MAX1031_MULTY_START;
		_last_ch = ch + 1;
		return 0;
	}
	return -1;
}

int MAX1031::full_multy_measure(MAX1031_chan_t ch)
{
	start_multy_measure(ch);
	wait();
	return complete_multy_measure();
}

int MAX1031::complete_multy_measure(void)
{
	if (_state == MAX1031_MULTY_START) 	 // if conv was started and ...
	{
		int res = 0;
		for (int i = 0; i < _last_ch; i++)
		{
	 		USHORT ADC_res = (USHORT)((_spi->transmit( 0x00, _ssel)) & 0x0F) << 8 ;
			ADC_res |= (USHORT)(_spi->transmit( 0x00, _ssel)& 0xFF) ;
			_data[i] = ADC_res;
			
		}
		_state = MAX1031_IDLE;
		return res;
	}	
	return -1;	
}

void MAX1031::wait(void)
{
	if (_state == MAX1031_SINGLE_START)
	{
		short_delay(MAX1031_SINGLE_CONV_TIME);
	}
	else if (_state == MAX1031_MULTY_START)
	{
		short_delay(MAX1031_SINGLE_CONV_TIME * _last_ch);
	}
	else
		dtrap();
}


