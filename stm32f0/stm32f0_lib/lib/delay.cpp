#include "delay.h"
#include "System.h"


#if (CCLK_FREQ == 48000000)
void short_delay(UI us_delay)
{
	if (us_delay)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();
	}
	else
		return;
	while(--us_delay)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();
	}
}
#elif (CCLK_FREQ == 8000000)
void short_delay(UI us_delay)
{
	if (us_delay)
	{
		__nop();__nop();__nop();__nop();
	}
	else
		return;
	while(--us_delay)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}
#endif
