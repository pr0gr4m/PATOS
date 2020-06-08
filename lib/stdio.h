#ifndef LIB_STDIO_H_
#define LIB_STDIO_H_

#include "stdarg.h"

typedef enum utoa_t {
	utoa_dec = 10,
	utoa_hex = 16,
} utoa_t;

uint32_t putstr(const char *s);
uint32_t d_printf(const char *fmt, ...);
uint32_t vsprintf(char *buf, const char *fmt, va_list arg);
uint32_t utoa(char *buf, uint32_t val, utoa_t base);

#endif
