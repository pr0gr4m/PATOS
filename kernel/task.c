#include "stdint.h"
#include "stdbool.h"
#include "ARMv7AR.h"
#include "task.h"

static kerneltcb_t task_list[MAX_TASK_NUM];
static kerneltcb_t *cur_tcb;
static kerneltcb_t *nxt_tcb;
static uint32_t alloc_tcb_index;
static uint32_t cur_tcb_index;

static kerneltcb_t *sched_rr(void);
static void save_context(void);
static void restore_context(void);

void kernel_task_init(void)
{
	alloc_tcb_index = 0;
	cur_tcb_index = 0;

	for (uint32_t i = 0; i < MAX_TASK_NUM; i++) {
		task_list[i].stack_base = (uint8_t *)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
		task_list[i].sp = (uint32_t)task_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

		task_list[i].sp -= sizeof(kerneltask_context_t);
		kerneltask_context_t *ctx = (kerneltask_context_t *)task_list[i].sp;
		ctx->pc = 0;
		ctx->spsr = ARM_MODE_BIT_SYS;
	}
}

uint32_t kernel_task_create(kerneltask_func_t start_func)
{
	kerneltcb_t *new_tcb = &task_list[alloc_tcb_index++];
	if (alloc_tcb_index > MAX_TASK_NUM)
		return NOT_ENOUGH_TASK_NUM;

	kerneltask_context_t *ctx = (kerneltask_context_t *)new_tcb->sp;
	ctx->pc = (uint32_t)start_func;
	return alloc_tcb_index - 1;
}

void kernel_task_start(void)
{
	nxt_tcb = &task_list[cur_tcb_index];
	restore_context();
}

void kernel_task_sched(void)
{
	cur_tcb = &task_list[cur_tcb_index];
	nxt_tcb = sched_rr();

	kernel_task_context_switch();
}

__attribute__((naked)) void kernel_task_context_switch(void)
{
	__asm__("B save_context");
	__asm__("B restore_context");
}

static kerneltcb_t *sched_rr(void)
{
	cur_tcb_index++;
	cur_tcb_index %= alloc_tcb_index;

	return &task_list[cur_tcb_index];
}

static __attribute__((naked)) void save_context(void)
{
	// save current task context into the current task stack
	__asm__("PUSH { lr }");
	__asm__("PUSH { r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12 }");
	__asm__("MRS r0, cpsr");
	__asm__("PUSH { r0 }");
	// save current task stack pointer into the current TCB
	__asm__("LDR r0, =cur_tcb");
	__asm__("LDR r0, [ r0 ]");
	__asm__("STMIA r0!, { sp }");
}

static __attribute__((naked)) void restore_context(void)
{
	// restore next task stack pointer from the next TCB
	__asm__("LDR r0, =nxt_tcb");
	__asm__("LDR r0, [ r0 ]");
	__asm__("LDMIA r0!, { sp }");
	// restore next task context from the next task stack
	__asm__("POP { r0 }");
	__asm__("MSR cpsr, r0");
	__asm__("POP { r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12 }");
	__asm__("POP { pc }");
}
