#include "stdint.h"
#include "stdbool.h"
#include "HALTimer.h"

void delay(uint32_t ms)
{
	uint32_t start = hal_timer_get_1ms_counter();
	uint32_t goal = start + ms;
	if (goal < start)	// goal is overflow
		while (goal < hal_timer_get_1ms_counter());	// wait until counter is overflow
	while (goal > hal_timer_get_1ms_counter());
}
