#include "main.h"
#include "GPIO.h"

extern C_GPIO led;	
static UI led_programm = LED_ALL_IS_GOOD;
static UI led_counter = 0;

void led_change_programm(LED_PROGRAMM_T prog)
{
	if (led_programm != prog)
	{
		led_counter = 0;
		led.clear();
		led_programm = prog;
	}
}

#define LED_PAUSE	1000
UI led_update(void)
{	
	led_counter++;
	if ( led_counter > (LED_PAUSE + led_programm * 512))
	{
		led_counter = 0;
		return 1;
	}
	if ( led_counter < LED_PAUSE)
		led.clear();
	else 
	{
		if (((led_counter-LED_PAUSE) & 0x1ff) < 256)
			led.clear();
		else
			led.set();
	}
	if ((led_programm == LED_ALL_IS_GOOD) || (led_programm == LED_NO_CONN))
		return 1;
	return 0;
}

