#include "stdint.h"
#include "HALUART.h"
#include "stdio.h"

uint32_t putstr(const char *s)
{
	uint32_t len = 0;
	while (*s) {
		hal_uart_put_char(*s++);
		len++;
	}
	return len;
}
