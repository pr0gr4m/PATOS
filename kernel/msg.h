#ifndef KERNEL_MSG_H_
#define KERNEL_MSG_H_

#include "stdbool.h"

#define MQ_SIZE_BYTE	512

typedef enum kernelmq_t
{
	kernelmq_task0,
	kernelmq_task1,
	kernelmq_task2,
	kernelmq_num,
} kernelmq_t;

typedef struct kernelqueue_t
{
	uint32_t front;
	uint32_t rear;
	uint8_t queue[MQ_SIZE_BYTE];
} kernelqueue_t;

void kernel_mq_init(void);
bool kernel_mq_is_empty(kernelmq_t qname);
bool kernel_mq_is_full(kernelmq_t qname);
bool kernel_mq_enqueue(kernelmq_t qname, uint8_t data);
bool kernel_mq_dequeue(kernelmq_t qname, uint8_t *data);

#endif
