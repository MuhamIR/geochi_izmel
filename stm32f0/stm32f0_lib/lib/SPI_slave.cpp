#include "SPI_slave.h"

void CSlaveSPI::init( SPI_TypeDef * hard_spi, const hard_SPI_slave_config * config, spi_data_handler handler )
{
	assert_param(handler != NULL);
	_handler = handler;
	SPI_InitTypeDef SPI_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	_spi = hard_spi;
	if (hard_spi == SPI1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	}
	else if (hard_spi == SPI2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	}
	NVIC_Init(&NVIC_InitStructure);
	SPI_I2S_DeInit(hard_spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = (config->num_of_bit - 1) << 8;
	SPI_InitStructure.SPI_CPOL = config->CPOL;
	SPI_InitStructure.SPI_CPHA = config->CPHA;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_Init(hard_spi, &SPI_InitStructure);
	
	/* Initialize the FIFO threshold */
	SPI_RxFIFOThresholdConfig(hard_spi, SPI_RxFIFOThreshold_QF);

	/* Enable the SPI peripheral */
	SPI_Cmd(hard_spi, ENABLE);	
	
	SPI_I2S_ITConfig(hard_spi, SPI_I2S_IT_RXNE, ENABLE);
	
	write_to_FIFO(0);//не уверен что это надо
}

CSlaveSPI::CSlaveSPI( SPI_TypeDef * hard_spi, const hard_SPI_slave_config * config, spi_data_handler handler )
{
	init(hard_spi, config, handler);
}

void CSlaveSPI::write_to_FIFO(UI data)
{
	if ((_spi->CR2 & 0xf00 ) > SPI_DataSize_8b)
	{
		SPI_I2S_SendData16(_spi, data);
	}
	else
	{
		SPI_SendData8(_spi, data);
	}	
}

UI CSlaveSPI::read_from_FIFO(void)
{
	if ((_spi->CR2 & 0xf00 ) > SPI_DataSize_8b)
	{
		return SPI_I2S_ReceiveData16(_spi);
	}
	else
	{
		return SPI_ReceiveData8(_spi);
	}	
}

void CSlaveSPI::interrupt_handler(void)
{
	if (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == SET)
	{
		U16 data = 0;
		bool flag = false;
		while(SPI_GetReceptionFIFOStatus(_spi)!=SPI_ReceptionFIFOStatus_Empty)
		{
			flag = true;
			if ((_spi->CR2 & 0xf00 ) > SPI_DataSize_8b)
			{
				data = SPI_I2S_ReceiveData16(_spi);
			}
			else
			{
				data = SPI_ReceiveData8(_spi);
			}	
		}
		if(flag && (SPI_TransmissionFIFOStatus_Empty == SPI_GetTransmissionFIFOStatus(_spi)))
		{
			if ((_spi->CR2 & 0xf00 ) > SPI_DataSize_8b)
			{
				SPI_I2S_SendData16(_spi, _handler(data));
			}
			else
			{
				SPI_SendData8(_spi, _handler(data));
			}
		}
	}
}

