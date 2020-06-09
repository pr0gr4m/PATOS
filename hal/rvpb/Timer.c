#include "stdint.h"
#include "Timer.h"
#include "HALTimer.h"
#include "HALInterrupt.h"

extern volatile Timer_t *timer;

static void interrupt_handler(void);
static uint32_t internal_1ms_counter;

void hal_timer_init(void)
{
	// reset interface
	timer->timerxcontrol.bits.TimerEn = 0;
	timer->timerxcontrol.bits.TimerMode = 0;
	timer->timerxcontrol.bits.OneShot = 0;
	timer->timerxcontrol.bits.TimerSize = 0;
	timer->timerxcontrol.bits.TimerPre = 0;
	timer->timerxcontrol.bits.IntEnable = 1;
	timer->timerxload = 0;
	timer->timerxvalue = 0xFFFFFFFF;

	// set periodic mode
	timer->timerxcontrol.bits.TimerMode = TIMER_PERIODIC;
	timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
	timer->timerxcontrol.bits.OneShot = 0;
	timer->timerxcontrol.bits.TimerPre = 0;
	timer->timerxcontrol.bits.IntEnable = 1;

	uint32_t interval_1ms = TIMER_10HZ_INTERVAL / 100;
	
	timer->timerxload = interval_1ms;
	timer->timerxcontrol.bits.TimerEn = 1;

	internal_1ms_counter = 0;

	// Register Timer interrupt handler
	hal_interrupt_enable(TIMER_INTERRUPT);
	hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT);
}

uint32_t hal_timer_get_1ms_counter(void)
{
	return internal_1ms_counter;
}

static void interrupt_handler(void)
{
	internal_1ms_counter++;
	timer->timerxintclr = 1;
}
