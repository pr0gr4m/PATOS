#include "stdint.h"

void main(void)
{
	uint32_t *dummy_addr = (uint32_t *)(1024 * 1024 * 100);
	*dummy_addr = sizeof(long);
}
