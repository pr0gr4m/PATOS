#include "stdint.h"
#include "stdbool.h"
#include "ARMv7AR.h"
#include "task.h"

static kerneltcb_t task_list[MAX_TASK_NUM];
static uint32_t allocated_tcb_index;

void kernel_task_init(void)
{
	allocated_tcb_index = 0;

	for (uint32_t i = 0; i < MAX_TASK_NUM; i++) {
		task_list[i].stack_base = (uint8_t *)(TASK_STACK_START +
				(i * USR_TASK_STACK_SIZE));
		task_list[i].sp = (uint32_t)task_list[i].stack_base +
			USR_TASK_STACK_SIZE - 4;

		task_list[i].sp -= sizeof(kerneltask_context_t);
		kerneltask_context_t *ctx = (kerneltask_context_t *)task_list[i].sp;
		ctx->pc = 0;
		ctx->spsr = ARM_MODE_BIT_SYS;
	}
}

uint32_t kernel_task_create(kerneltask_func_t start_func)
{
	kerneltcb_t *new_tcb = &task_list[allocated_tcb_index++];
	if (allocated_tcb_index > MAX_TASK_NUM)
		return NOT_ENOUGH_TASK_NUM;

	kerneltask_context_t *ctx = (kerneltask_context_t *)new_tcb->sp;
	ctx->pc = (uint32_t)start_func;
	return allocated_tcb_index - 1;
}
