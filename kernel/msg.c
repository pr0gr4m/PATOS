#include "stdint.h"
#include "stdlib.h"
#include "msg.h"

kernelqueue_t mq[kernelmq_num];

void kernel_mq_init(void)
{
	for (uint32_t i = 0; i < kernelmq_num; i++) {
		mq[i].front = 0;
		mq[i].rear = 0;
		memclr(mq[i].queue, MQ_SIZE_BYTE);
	}
}

bool kernel_mq_is_empty(kernelmq_t qname)
{
	if (qname >= kernelmq_num)
		return false;
	if (mq[qname].front == mq[qname].rear)
		return true;
	return false;
}

bool kernel_mq_is_full(kernelmq_t qname)
{
	if (qname >= kernelmq_num)
		return false;
	if (((mq[qname].rear + 1) % MQ_SIZE_BYTE) == mq[qname].front)
		return true;
	return false;
}

bool kernel_mq_enqueue(kernelmq_t qname, uint8_t data)
{
	if (qname >= kernelmq_num)
		return false;

	if (kernel_mq_is_full(qname))
		return false;

	mq[qname].rear++;
	mq[qname].rear %= MQ_SIZE_BYTE;
	
	uint32_t idx = mq[qname].rear;
	mq[qname].queue[idx] = data;
	return true;
}

bool kernel_mq_dequeue(kernelmq_t qname, uint8_t *data)
{
	if (qname >= kernelmq_num)
		return false;

	if (kernel_mq_is_empty(qname))
		return false;

	mq[qname].front++;
	mq[qname].front %= MQ_SIZE_BYTE;
	
	uint32_t idx = mq[qname].front;
	*data = mq[qname].queue[idx];
	return true;
}
