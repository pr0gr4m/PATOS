#include "stdint.h"
#include "HALUART.h"
#include "stdio.h"

static void hw_init(void);
static void printf_test(void);

void main(void)
{
	hw_init();

	uint32_t i = 100;
	while (i--) {
		hal_uart_put_char('H');
	}
	hal_uart_put_char('\n');
	putstr("Hello World!\n");

	printf_test();

	while (true);
}

static void hw_init(void)
{
	// init interrupt -> init uart
	hal_interrupt_init();
	hal_uart_init();
}

static void printf_test(void)
{
	char *str = "test string";
	char *nullptr = 0;
	uint32_t i = 10;

	d_printf("%s\n", "Hello printf");
	d_printf("str: [%s]\n", str);
	d_printf("%s is null pointer\n", nullptr);
	d_printf("%u = %u\n", i, 10);
	d_printf("dec = %u, hex = %x\n", 0xff, 0xff);
	d_printf("print 0 [%u]\n", 0);
}
