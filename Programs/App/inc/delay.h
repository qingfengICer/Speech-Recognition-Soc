#ifndef __DELAY_H
#define __DELAY_H

#include "soc.h"

typedef struct {
	uint16_t uart0RX;
	uint16_t idle;
	uint32_t tick;
} TIMING_TYPE;
extern TIMING_TYPE SysTickTiming;

void SysTick_init(void);
void Delay_ms(uint32_t ms);

#endif

