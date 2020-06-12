#ifndef KERNEL_SYNC_H_
#define KERNEL_SYNC_H_

#include "stdbool.h"

typedef struct kernelmutex_t
{
	uint32_t owner;
	bool lock;
} kernelmutex_t;

void kernel_sem_init(int32_t max);
bool kernel_sem_test(void);
void kernel_sem_release(void);

void kernel_mutex_init(void);
bool kernel_mutex_lock(uint32_t owner);
bool kernel_mutex_unlock(uint32_t owner);

#endif
