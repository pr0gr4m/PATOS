#include "stdint.h"
#include "UART.h"
#include "HALUART.h"

extern volatile PL011_t *UART;

void hal_uart_init(void)
{
	UART->uartcr.bits.UARTEN = 0;
	UART->uartcr.bits.TXE = 1;
	UART->uartcr.bits.RXE = 1;
	UART->uartcr.bits.UARTEN = 1;
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
