#ifndef _SPI_SLAVE_H__
#define _SPI_SLAVE_H__
#include "stm32f0xx.h"


typedef struct 
{
	UI num_of_bit;
	UI CPOL;
	UI CPHA;
}hard_SPI_slave_config;

// EXAMPLE
// const hard_SPI_slave_config spi_config =
// {
// 	SPI_DataSize_8b,
// 	I2S_CPOL_Low,
// 	SPI_CPHA_1Edge
// };

typedef UI (* spi_data_handler )(UI);

class CSlaveSPI 
{
	public:
		CSlaveSPI(){};
		CSlaveSPI( SPI_TypeDef * hard_spi, const hard_SPI_slave_config * config, spi_data_handler handler );
		void init( SPI_TypeDef * hard_spi, const hard_SPI_slave_config * config, spi_data_handler handler );		
		void interrupt_handler(void);
	protected:
		void write_to_FIFO(UI data);
		UI read_from_FIFO(void);
		SPI_TypeDef * _spi;
		spi_data_handler _handler;
};

#endif

