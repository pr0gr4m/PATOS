#include "stdint.h"
#include "stdio.h"
#include "event.h"

static uint32_t event_flag;

void kernel_event_flag_init(void)
{
	event_flag = 0;
}

void kernel_event_flag_set(kernelevent_flag_t event)
{
	event_flag |= (uint32_t)event;
}

void kernel_event_flag_clear(kernelevent_flag_t event)
{
	event_flag &= ~((uint32_t)event);
}

bool kernel_event_flag_check(kernelevent_flag_t event)
{
	if (event_flag & (uint32_t)event) {
		kernel_event_flag_clear(event);
		return true;
	}
	return false;
}
