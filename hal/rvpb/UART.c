#include "stdint.h"
#include "UART.h"
#include "HALUART.h"
#include "HALInterrupt.h"
#include "kernel.h"

extern volatile PL011_t *UART;

static void interrupt_handler(void);

void hal_uart_init(void)
{
	// Enable UART
	UART->uartcr.bits.UARTEN = 0;
	UART->uartcr.bits.TXE = 1;
	UART->uartcr.bits.RXE = 1;
	UART->uartcr.bits.UARTEN = 1;

	// Enable input interrupt
	UART->uartimsc.bits.RXIM = 1;

	// Register UART interrupt handler
	hal_interrupt_enable(UART_INTERRUPT0);
	hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void hal_uart_put_char(uint8_t ch)
{
	while (UART->uartfr.bits.TXFF);		// wait for tx buffer is empty
	UART->uartdr.all = (ch & 0xFF);
}

uint8_t hal_uart_get_char(void)
{
	uint32_t data;

	while (UART->uartfr.bits.RXFE);		// wait for rx buffer is not empty

	data = UART->uartdr.all;

	// Check for an error flag
	if (data & 0xFFFFFF00) {
		UART->uartrsr.all = 0xFF;
		return 0;
	}
	
	return (uint8_t)(data & 0xFF);
}

static void interrupt_handler(void)
{
	uint8_t ch = hal_uart_get_char();
	hal_uart_put_char(ch);

	kernel_send_msg(kernelmq_task0, &ch, 1);
	kernel_send_events(kernelevent_flag_uartin);
}
