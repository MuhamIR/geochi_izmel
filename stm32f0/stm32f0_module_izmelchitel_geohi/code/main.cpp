#include "main.h"
#include "mini_modbus.h"
#include "macros.h"
#include "GPIO.h"

UI outputs_state = 0;

static UI in_cycle_counter = 0;


void _1ms_activity()
{
	extern C_GPIO outputs[OUTPUTS_NUM];
	
	if ((in_cycle_counter == 0) && (RW_table.pause))
	{
		return;
	}
	
	if (RO_table.number_to_execute)
	{
		outputs_state = 0;
		ff( i, OUTPUTS_NUM)
		{
			
			if ((in_cycle_counter >= ROM_table.channel_timings[i].on_offset) && 
				(in_cycle_counter < (ROM_table.channel_timings[i].on_offset + ROM_table.channel_timings[i].on_delay)))
			{
				outputs_state |= 1 << i;
			}
		}
		if (in_cycle_counter++ == ROM_table.period)
		{
			in_cycle_counter = 0;
			RO_table.number_to_execute--;
		}
	}
	else
	{
		outputs_state = 0;
	}
	
	ff(i, OUTPUTS_NUM)
	{
		outputs[i].set_state( outputs_state & (1 << i));
	}
}

	
int main(void)
{	
	extern void initialization(void);
	initialization();
	
	void set_init_values();
	set_init_values();
	
	bool check_flash(void);
	if (check_flash() == false)
	{
		RO_table.statusReg |= FLASH_ERROR;
	}
	
	
	while (1)
	{
		extern UART_handler rs485_handler;
		rs485_handler.handler();	
//		void ADC_update_continuous(void);
//		ADC_update_continuous();
		
		if (modbus_update_flag)
		{
			modbus_update_flag = 0;
			static int last_toggle = 0;
			if (RW_table.toggle != last_toggle)
			{
				last_toggle = RW_table.toggle;
				RO_table.number_to_execute = RW_table.number;
				in_cycle_counter = 0;
			}	
		}
		
		
		if ( modbus_copy_to_ROM_request )
		{
			modbus_copy_to_ROM_request = 0;
			extern void mini_modbus_write_to_flash(void);
			mini_modbus_write_to_flash();
		}
		
		extern volatile UI systick_flag;
		if (systick_flag)// раз в 100 микросекунд
		{
			systick_flag--;
			static UI ms_counter = 0;				
			if (++ms_counter == 10)
			{
				ms_counter = 0;
				
				
				static ULONG time_counter = 0;
				
				RO_table.timeLO = time_counter & 0xffff;
				RO_table.timeHI = (time_counter >> 16) & 0xffff;
				RO_table.outputs_state = outputs_state;
				time_counter++;
				
				
//				extern void ADC_update_1_ms(void);
//				ADC_update_1_ms();
				
				_1ms_activity();

				if (conn_last_mes_inc_counter < 0xffff)
					conn_last_mes_inc_counter++;
				
				if (conn_last_mes_inc_counter < MAX_TIME_BETWEEN_MES)
					RO_table.statusReg &= ~NO_CONN_ERROR;
				else
					RO_table.statusReg |= NO_CONN_ERROR;
				
				
				extern void led_change_programm(LED_PROGRAMM_T prog);
				extern UI led_update(void);
				if(led_update())
				{
					if (RO_table.statusReg)
					{
						if (RO_table.statusReg & ADC_ERROR)
							led_change_programm(LED_ADC_ERR);
						else if (RO_table.statusReg & NO_CONN_ERROR)
							led_change_programm(LED_NO_CONN);
						else if (RO_table.statusReg & FLASH_ERROR)
							led_change_programm(LED_FLASH_ERR);				
					}
					else			
						led_change_programm(LED_ALL_IS_GOOD);
				}	
			}
 		}
	}
}
