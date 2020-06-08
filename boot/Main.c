#include "stdint.h"
#include "HALUART.h"
#include "stdio.h"

static void hw_init(void);

void main(void)
{
	hw_init();

	uint32_t i = 100;
	while (i--) {
		hal_uart_put_char('H');
	}
	hal_uart_put_char('\n');
	putstr("Hello World!\n");

	i = 100;
	while (i--) {
		uint8_t ch = hal_uart_get_char();
		hal_uart_put_char(ch);
	}
}

static void hw_init(void)
{
	hal_uart_init();
}
