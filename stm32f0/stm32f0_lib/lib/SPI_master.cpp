#include "SPI_master.h"


void CMasterSPIHardware::init( SPI_TypeDef * hard_spi, const hard_SPI_config * config )
{
	SPI_InitTypeDef SPI_InitStructure;	
	_spi = hard_spi;
	if (hard_spi == SPI1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	else if (hard_spi == SPI2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	SPI_I2S_DeInit(hard_spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	SPI_InitStructure.SPI_DataSize = (config->num_of_bit - 1) << 8;
	SPI_InitStructure.SPI_CPOL = config->CPOL;
	SPI_InitStructure.SPI_CPHA = config->CPHA;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = config->prescaller;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(hard_spi, &SPI_InitStructure);
	
	SPI_SSOutputCmd(hard_spi, ENABLE);
	SPI_NSSPulseModeCmd(hard_spi, ENABLE);
	
	
	/* Initialize the FIFO threshold */
	SPI_RxFIFOThresholdConfig(hard_spi, SPI_RxFIFOThreshold_QF);

	/* Enable the SPI peripheral */
	SPI_Cmd(hard_spi, ENABLE);		
}

CMasterSPIHardware::CMasterSPIHardware( SPI_TypeDef * hard_spi, const hard_SPI_config * config )
:CMasterSPI()
{
	init (hard_spi,config);
}


UI CMasterSPIHardware::transmit(UI data, ssel_t ssel)
{	
	while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == SET)
		;
	if ((_spi->CR2 & 0xf00 ) > SPI_DataSize_8b)
	{
		SPI_I2S_SendData16(_spi, data);
		while ((_spi->SR & 3UL << 11) != 0)//FIFO is not empty
		;
		while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) != SET)
		;
		return SPI_I2S_ReceiveData16(_spi);
	}
	else
	{
		SPI_SendData8(_spi, data);
		while ((_spi->SR & 3UL << 11) != 0)//FIFO is not empty
		;
		while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) != SET)
		;
		return SPI_ReceiveData8(_spi);
	}	
}

void CMasterSPIHardware::num_of_bit_set(UI num_of_bit)
{
	SPI_DataSizeConfig(_spi, (num_of_bit - 1) << 8);
}

CMasterSPIHardwareMultySS::CMasterSPIHardwareMultySS( SPI_TypeDef * hard_spi, const hard_SPI_config * config , UI ssel_num, bool invert_flag)
{
	init( hard_spi, config  , ssel_num, invert_flag);
}

void CMasterSPIHardwareMultySS::init( SPI_TypeDef * hard_spi, const hard_SPI_config * config  , UI ssel_num, bool invert_flag)
{
	_invert_flag = invert_flag;
	_ssel_num = ssel_num;
	_ssel_array = new C_GPIO[_ssel_num];
	CMasterSPIHardware::init( hard_spi, config);
}

void CMasterSPIHardwareMultySS::init_ssel(ssel_t ssel, GPIO_TypeDef * port, U16 pin)
{
	_ssel_array[ssel].init_pin(port, pin);
	_ssel_array[ssel].set_as_output();
	_ssel_array[ssel].set();
}

UI CMasterSPIHardwareMultySS::transmit(UI data, ssel_t ssel)
{
	_ssel_array[ssel].clear();
	UI res = CMasterSPIHardware::transmit(data, ssel);
	while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_BSY) == SET)
		;
	_ssel_array[ssel].set();
	if (_invert_flag)
		res ^= 0xffffffff;
	return res;
}
