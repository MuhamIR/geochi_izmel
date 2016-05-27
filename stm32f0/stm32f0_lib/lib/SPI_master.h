#ifndef _SPI_MASTER_H__
#define _SPI_MASTER_H__
#include "System.h"
#include "GPIO.h"

typedef struct 
{
	UI prescaller;
	UI num_of_bit;
	UI CPOL;
	UI CPHA;
}hard_SPI_config;

// EXAMPLE
// const hard_SPI_config spi_config =
// {
// 	SPI_BaudRatePrescaler_64,
// 	SPI_DataSize_8b,
// 	I2S_CPOL_Low,
// 	SPI_CPHA_1Edge
// };


enum ssel_t {SSEL0 = 0, SSEL1 = 1, SSEL2 = 2, SSEL3 = 3, SSEL4 = 4, SSEL5 = 5, SSEL6 = 6, SSEL7 = 7};


class CMasterSPI
{
	public:
		virtual UI transmit(UI data, ssel_t ssel) = 0;
		virtual void num_of_bit_set(UI num_of_bit) = 0;
};


class CMasterSPIHardware : public CMasterSPI
{
	public:
		CMasterSPIHardware(){};
		CMasterSPIHardware( SPI_TypeDef * hard_spi, const hard_SPI_config * config );
		void init( SPI_TypeDef * hard_spi, const hard_SPI_config * config );
		virtual UI transmit(UI data, ssel_t ssel = SSEL0);
		virtual void num_of_bit_set(UI num_of_bit);
	protected:
		SPI_TypeDef * _spi;
};


class CMasterSPIHardwareMultySS : public CMasterSPIHardware
{
	public:
		CMasterSPIHardwareMultySS(){};
		CMasterSPIHardwareMultySS( SPI_TypeDef * hard_spi, const hard_SPI_config * config , UI ssel_num = 1, bool invert_flag = false);
		void init( SPI_TypeDef * hard_spi, const hard_SPI_config * config  , UI ssel_num = 1, bool invert_flag = false);
		virtual UI transmit(UI data, ssel_t ssel = SSEL0);
		void init_ssel(ssel_t ssel, GPIO_TypeDef * port, U16 pin);
	protected:
		C_GPIO * _ssel_array;
		UI _ssel_num;
		bool _invert_flag;
};



#endif
