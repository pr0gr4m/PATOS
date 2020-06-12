#ifndef KERNEL_SYNC_H_
#define KERNEL_SYNC_H_

#include "stdbool.h"

void kernel_sem_init(int32_t max);
bool kernel_sem_test(void);
void kernel_sem_release(void);

#endif
