#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"
#include "memio.h"

void kernel_start(void)
{
	kernel_task_start();
}

void kernel_yield(void)
{
	kernel_task_sched();
}

void kernel_send_events(uint32_t event_list)
{
	for (uint32_t i = 0; i < 32; i++) {
		if ((event_list >> i) & 1) {
			kernelevent_flag_t sending_event = kernelevent_flag_empty;
			sending_event = (kernelevent_flag_t)SET_BIT(sending_event, i);
			kernel_event_flag_set(sending_event);
		}
	}
}

kernelevent_flag_t kernel_wait_events(uint32_t waiting_list)
{
	for (uint32_t i = 0; i < 32; i++) {
		if ((waiting_list >> i) & 1) {
			kernelevent_flag_t waiting_event = kernelevent_flag_empty;
			waiting_event = (kernelevent_flag_t)SET_BIT(waiting_event, i);
			if (kernel_event_flag_check(waiting_event))
				return waiting_event;
		}
	}
	return kernelevent_flag_empty;
}
