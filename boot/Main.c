#include "stdint.h"
#include "stdbool.h"
#include "HALInterrupt.h"
#include "HALUART.h"
#include "HALTimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "kernel.h"

static void hw_init(void);
static void kernel_init(void);

static void printf_test(void);
static void timer_test(void);

void user_task0(void);
void user_task1(void);
void user_task2(void);

void main(void)
{
	hw_init();

	uint32_t i = 100;
	while (i--) {
		hal_uart_put_char('H');
	}
	hal_uart_put_char('\n');
	putstr("Hello World!\n");

	printf_test();
	timer_test();

	kernel_init();

	while (true);
}

static void hw_init(void)
{
	// init interrupt -> init else
	hal_interrupt_init();
	hal_uart_init();
	hal_timer_init();
}

static void kernel_init(void)
{
	uint32_t taskid;

	kernel_task_init();
	kernel_event_flag_init();
	
	taskid = kernel_task_create(user_task0);
	if (taskid == NOT_ENOUGH_TASK_NUM)
		putstr("Failed to create task0\n");

	taskid = kernel_task_create(user_task1);
	if (taskid == NOT_ENOUGH_TASK_NUM)
		putstr("Failed to create task1\n");

	taskid = kernel_task_create(user_task2);
	if (taskid == NOT_ENOUGH_TASK_NUM)
		putstr("Failed to create task2\n");

	kernel_start();
}

static void printf_test(void)
{
	char *str = "test string";
	char *nullptr = 0;
	uint32_t i = 10;
	uint32_t *sysctrl0 = (uint32_t *)0x10001000;

	d_printf("%s\n", "Hello printf");
	d_printf("str: [%s]\n", str);
	d_printf("%s is null pointer\n", nullptr);
	d_printf("%u = %u\n", i, 10);
	d_printf("dec = %u, hex = %x\n", 0xff, 0xff);
	d_printf("print 0 [%u]\n", 0);
	d_printf("SYSCTRL0 %x\n", *sysctrl0);
}

static void timer_test(void)
{
	for (uint32_t i = 0; i < 3; i++) {
		d_printf("current count : %u\n", hal_timer_get_1ms_counter());
		delay(1000);
	}
}

void user_task0(void)
{
	uint32_t local = 0;
	d_printf("User Task #0 SP = 0x%x\n", &local);
	while (true) {
		bool pending_event = true;
		while (pending_event) {
			kernelevent_flag_t handle_event = kernel_wait_events(kernelevent_flag_uartin | kernelevent_flag_cmdout);
			switch (handle_event) {
				case kernelevent_flag_uartin:
					d_printf("\nUART In Event handled\n");
					kernel_send_events(kernelevent_flag_cmdin);
					break;
				case kernelevent_flag_cmdout:
					d_printf("\nCmdOut Event handled\n");
					break;
				default:
					pending_event = false;
					break;
			}
		}
		kernel_yield();
	}
}

void user_task1(void)
{
	uint32_t local = 0;
	d_printf("User Task #1 SP = 0x%x\n", &local);
	while (true) {
		kernelevent_flag_t handle_event = kernel_wait_events(kernelevent_flag_cmdin);
		switch (handle_event) {
			case kernelevent_flag_cmdin:
				d_printf("\nCmdIn Event handled\n");
				break;
		}
		kernel_yield();
	}
}

void user_task2(void)
{
	uint32_t local = 0;
	d_printf("User Task #2 SP = 0x%x\n", &local);
	while (true) {
		kernel_yield();
	}
}
