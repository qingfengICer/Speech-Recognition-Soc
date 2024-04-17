
#define _DELAY_C_
#include "delay.h"

TIMING_TYPE SysTickTiming;

void SysTick_Handler(void)
{
	SysTickTiming.tick ++;
	if (SysTickTiming.uart0RX != 0) {
		SysTickTiming.uart0RX --;
	}
}

void SysTick_init(void)
{
	//1ms timer interrupt
	SysTick_Config(SYSTEM_CLOCK / 1000);
}

void Delay_ms(uint32_t ms)
{
	uint32_t now = SysTickTiming.tick;
	while((SysTickTiming.tick - now) < ms);
}






