#include "stdbool.h"
#include "stdint.h"
#include "HALInterrupt.h"

__attribute__((interrupt("IRQ"))) void irq_handler(void)
{
	hal_interrupt_run_handler();
}

__attribute__((interrupt("FIQ"))) void fiq_handler(void)
{	// dummy handler
	while (true);
}
