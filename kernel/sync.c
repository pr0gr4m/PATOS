#include "stdint.h"
#include "sync.h"

#define DEF_SEM_MAX	8

static int32_t sem_max;
static int32_t sem;

void kernel_sem_init(int32_t max)
{
	sem_max = (max <= 0) ? DEF_SEM_MAX : max;
	sem = sem_max;
}

bool kernel_sem_test(void)
{
	if (sem <= 0)
		return false;
	sem--;
	return true;
}

void kernel_sem_release(void)
{
	sem++;
	if (sem >= sem_max)
		sem = sem_max;
}
