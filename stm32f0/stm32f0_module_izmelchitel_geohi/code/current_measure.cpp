//#include "macros.h"
//#include "filters.h"
//#include "mini_modbus_tables.h"
//#include "stm32f0xx_adc.h"


//U16 state[OUTPUTS_NUM];
//U16 ADC_values[OUTPUTS_NUM];
//U8 ADC_ready_mask = 0;
//U32 adc_factor;

//void start_measure(UI chan)
//{
//	const static U32 channels[OUTPUTS_NUM] = 
//	{
//		ADC_Channel_0, ADC_Channel_1, ADC_Channel_2, ADC_Channel_3, 
//		ADC_Channel_6, ADC_Channel_7, ADC_Channel_14, ADC_Channel_15
//	};
//	ADC1->CHSELR = channels[chan];
//	ADC1->CR |= ADC_CR_ADSTART;
//}

//void ADC_Config(void)
//{ 
//	//8 аналоговых входов для считывания тока ключей
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//  /* ADC1 Periph clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//	/* ADC1 DeInit */  
//  ADC_DeInit(ADC1);
//	adc_factor = ADC_GetCalibrationFactor(ADC1);
//  
//  ADC_InitTypeDef    ADC_InitStructure;
//  ADC_StructInit(&ADC_InitStructure);
//  
//  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
//  ADC_Init(ADC1, &ADC_InitStructure);
//		

//  
//  /* Convert the ADC1 Channel 1 with 239.5 Cycles as sampling time */ 
//  //ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_239_5Cycles);
//	ADC1->SMPR = ADC_SampleTime_41_5Cycles;//самое медленное - самое точное
//	
//  
//  /* Enable ADC1 */
//  ADC1->CR |= ADC_CR_ADEN;

//  /* Wait the ADRDY falg */
//  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

//  /* ADC1 Start Conversion */ 
//  start_measure(0);
//}

//void ADC_update_continuous(void)
//{
//	
//	if(ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC) != RESET)
//	{
//		static UI chan = 0;		
//		ADC_values[chan] = ADC_GetConversionValue(ADC1);
//		ADC_ready_mask |= 1 << chan;
//		chan = (chan + 1)%8;
//		start_measure(chan);
//	}
//}

//void ADC_update_1_ms(void)
//{
//	extern CExponentialSmooth filters[];
//	RO_table.statusReg &= ~ADC_ERROR;
//	if (ADC_ready_mask != 0xff)
//	{
//		RO_table.statusReg |= ADC_ERROR;
//	}
//	ADC_ready_mask = 0;	
//	extern UI outputs_state;

//	for (UI i = 0; i < OUTPUTS_NUM; i++)
//	{
//		RO_table.ADCValues[i] = ADC_values[i];
//		if (RO_table.ADCValues[i] == 0xfff)
//		{
//			RO_table.statusReg |= ADC_ERROR;
//		}
//		if (outputs_state & (1 << i))
//		{	
//			if (RO_table.ADCValues[i] == 0)
//			{
//				RO_table.short_currents[i] =  KZ;
//			}
//			else if (RO_table.ADCValues[i] < 7)
//			{
//				RO_table.short_currents[i] =  OBRIV;
//			}
//			else
//			{
//				int temp = (int)ROM_table.current1 +  
//				((int)RO_table.ADCValues[i] - (int)ROM_table.current1_calibration[i]) * ((int)ROM_table.current2 - (int)ROM_table.current1) / 
//				((int)ROM_table.current2_calibration[i] - (int)ROM_table.current1_calibration[i]);
//				if (temp < 0)
//					temp = 0;
//				if (temp < 20000)
//				{
//					//меньше 20А - точность 
//					RO_table.short_currents[i] = (temp + 50)/100 + NORMAL_STATE;
//				}
//				else
//				{
//					UI short_current = (temp - 20000 + 500)/1000 + 203;
//					restrict_max<UI>(short_current, 0xff);
//					RO_table.short_currents[i] = short_current;
//				}
//			}
//		}
//		else
//		{
//			RO_table.short_currents[i] =  NO_DIAGNOSTIC;
//		}
//	}
//}

