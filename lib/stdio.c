#include "stdint.h"
#include "HALUART.h"
#include "stdio.h"

#define PRINTF_BUF_LEN	1024

static char printf_buf[PRINTF_BUF_LEN];

uint32_t putstr(const char *s)
{
	uint32_t len = 0;
	while (*s) {
		hal_uart_put_char(*s++);
		len++;
	}
	return len;
}

uint32_t d_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(printf_buf, fmt, args);
	va_end(args);

	return putstr(printf_buf);
}

uint32_t vsprintf(char *buf, const char *fmt, va_list arg)
{
	char ch;
	char *str;
	uint32_t c = 0;
	uint32_t uint, hex;

	for (uint32_t i = 0; fmt[i]; i++) {
		if (fmt[i] == '%') {
			i++;
			switch (fmt[i]) {
				case 'c':
					ch = (char)va_arg(arg, int32_t);
					buf[c++] = ch;
					break;

				case 's':
					str = (char *)va_arg(arg, char *);
					if (str == NULL)
						str = "(null)";
					while (*str)
						buf[c++] = *str++;
					break;

				case 'u':
					uint = (uint32_t)va_arg(arg, uint32_t);
					c += utoa(&buf[c], uint, utoa_dec);
					break;

				case 'x':
					hex = (uint32_t)va_arg(arg, uint32_t);
					c += utoa(&buf[c], hex, utoa_hex);
					break;
			}
		} else {
			buf[c++] = fmt[i];
		}
	}

	if (c >= PRINTF_BUF_LEN) {
		buf[0] = '\0';
		return 0;
	}

	buf[c] = '\0';
	return c;
}

uint32_t utoa(char *buf, uint32_t val, utoa_t base)
{
	const char asciibase = 'a';
	uint32_t c = 0;
	int32_t idx = 0;
	char tmp[11];

	do {
		uint32_t t = val % (uint32_t)base;
		if (t >= 10)
			t += asciibase - '0' - 10;
		tmp[idx++] = t + '0';
		val /= base;
	} while (val);

	idx--;
	while (idx >= 0)
		buf[c++] = tmp[idx--];
	
	return c;
}
