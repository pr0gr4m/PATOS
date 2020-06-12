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
static void critical_section_test(uint32_t p, uint32_t taskid);

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
	kernel_mq_init();
	kernel_sem_init(1);
	
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

static uint32_t shared_value;
static void critical_section_test(uint32_t p, uint32_t taskid)
{
	kernel_lock_sem();
	d_printf("User Task #%u Send = %u\n", taskid, p);
	shared_value = p;
	kernel_yield();
	delay(1000);
	d_printf("User Task #%u Shared Value = %u\n", taskid, shared_value);
	kernel_unlock_sem();
}

void user_task0(void)
{
	uint32_t local = 0;
	uint8_t cmdbuf[16];
	uint32_t cmdbufidx = 0;
	uint8_t uartch = 0;
	d_printf("User Task #0 SP = 0x%x\n", &local);
	while (true) {
		bool pending_event = true;
		while (pending_event) {
			kernelevent_flag_t handle_event = kernel_wait_events(kernelevent_flag_uartin | kernelevent_flag_cmdout);
			switch (handle_event) {
				case kernelevent_flag_uartin:
					kernel_recv_msg(kernelmq_task0, &uartch, 1);
					if (uartch == '\r') {
						cmdbuf[cmdbufidx] = '\0';
						while (true) {
							kernel_send_events(kernelevent_flag_cmdin);
							if (kernel_send_msg(kernelmq_task1, &cmdbufidx, 1) == false) {
								kernel_yield();
							} else if (kernel_send_msg(kernelmq_task1, cmdbuf, cmdbufidx) == false) {
								uint8_t rollback;
								kernel_recv_msg(kernelmq_task1, &rollback, 1);
								kernel_yield();
							} else {
								break;
							}
						}
						cmdbufidx = 0;
					} else {
						cmdbuf[cmdbufidx++] = uartch;
						cmdbufidx %= 16;
					}
					break;
				case kernelevent_flag_cmdout:
					//d_printf("\nCmdOut Event handled by task 0\n");
					critical_section_test(5, 0);
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
	uint8_t cmdlen = 0;
	uint8_t cmd[16] = { 0, };
	d_printf("User Task #1 SP = 0x%x\n", &local);
	while (true) {
		kernelevent_flag_t handle_event = kernel_wait_events(kernelevent_flag_cmdin);
		switch (handle_event) {
			case kernelevent_flag_cmdin:
				memclr(cmd, 16);
				kernel_recv_msg(kernelmq_task1, &cmdlen, 1);
				kernel_recv_msg(kernelmq_task1, cmd, cmdlen);
				d_printf("\nrecv cmd: [%s]\n", cmd);
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
		critical_section_test(3, 2);
		kernel_yield();
	}
}
