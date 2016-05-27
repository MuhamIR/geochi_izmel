#ifndef _MAX1031_H_
#define _MAX1031_H_

#include "System.h"
#include "SPI_master.h"
#include "GPIO.h"


class MAX1031 {  
	public:
		enum MAX1031_state_t { MAX1031_IDLE, MAX1031_SINGLE_START, MAX1031_MULTY_START};
		enum MAX1031_chan_t {AIN0 = 0, AIN1 = 1, AIN2 = 2, AIN3 = 3, AIN4 = 4, AIN5 = 5, AIN6 = 6, AIN7 = 7, AIN8 = 8, AIN9 = 9, AIN10 = 10, AIN11 = 11, AIN12 = 12, AIN13 = 13, AIN14 = 14, AIN15 = 15};
			
		MAX1031(){_power_state = false;};
		MAX1031( GPIO_TypeDef * a_port, UI a_pin_num, CMasterSPI * spi, ssel_t ssel = SSEL0);
			
		void init_power_pin(GPIO_TypeDef * a_port, UI a_pin_num);	
		void power_enable(void){_power_pin.set();_power_state = true;}
		void power_disable(void){_power_pin.clear();_power_state = false;}
		void power_state_set(bool state);	
		void config( CMasterSPI * spi, ssel_t ssel = SSEL0);
		void reconfig( void );	
						
		int start_measure(MAX1031_chan_t ch);
		int complete_measure(void);
		int full_measure(MAX1031_chan_t ch);
		int start_multy_measure(MAX1031_chan_t ch = AIN15);
		int complete_multy_measure(void);
		int full_multy_measure(MAX1031_chan_t ch = AIN15);
//		UI temperature_measure(void);
	
		U16 get_last(MAX1031_chan_t ch){return _data[ch];}		
		inline UI get_temperature(void){return _temperature;};
		void wait(void);
		
		void init_N_EOC_pin(GPIO_TypeDef * a_port, UI a_pin_num);		
		bool conversion_is_complete(void);
		
		
	protected:
		U16 _data[16];
		U16 _temperature;	
		UI _last_ch;
		MAX1031_state_t _state;
		CMasterSPI * _spi;
		C_GPIO _N_EOC_pin;
		C_GPIO _power_pin;
		ssel_t _ssel;
		bool _power_state;
};

typedef MAX1031 MAX1231;


#endif

