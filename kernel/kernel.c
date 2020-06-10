#include "stdint.h"
#include "stdbool.h"
#include "kernel.h"

void kernel_start(void)
{
	kernel_task_start();
}

void kernel_yield(void)
{
	kernel_task_sched();
}
