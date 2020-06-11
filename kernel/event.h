#ifndef KERNEL_EVENT_H_
#define KERNEL_EVENT_H_

#include "stdbool.h"

typedef enum kernelevent_flag_t
{
	kernelevent_flag_uartin		= 0x00000001,
	kernelevent_flag_cmdin		= 0x00000002,
	kernelevent_flag_reserved02	= 0x00000004,
	kernelevent_flag_reserved03	= 0x00000008,
	kernelevent_flag_reserved04	= 0x00000010,
	kernelevent_flag_reserved05	= 0x00000020,
	kernelevent_flag_reserved06	= 0x00000040,
	kernelevent_flag_reserved07	= 0x00000080,
	kernelevent_flag_reserved08	= 0x00000100,
	kernelevent_flag_reserved09	= 0x00000200,
	kernelevent_flag_reserved10	= 0x00000400,
	kernelevent_flag_reserved11	= 0x00000800,
	kernelevent_flag_reserved12	= 0x00001000,
	kernelevent_flag_reserved13	= 0x00002000,
	kernelevent_flag_reserved14	= 0x00004000,
	kernelevent_flag_reserved15	= 0x00008000,
	kernelevent_flag_reserved16	= 0x00010000,
	kernelevent_flag_reserved17	= 0x00020000,
	kernelevent_flag_reserved18	= 0x00040000,
	kernelevent_flag_reserved19	= 0x00080000,
	kernelevent_flag_reserved20	= 0x00100000,
	kernelevent_flag_reserved21	= 0x00200000,
	kernelevent_flag_reserved22	= 0x00400000,
	kernelevent_flag_reserved23	= 0x00800000,
	kernelevent_flag_reserved24	= 0x01000000,
	kernelevent_flag_reserved25	= 0x02000000,
	kernelevent_flag_reserved26	= 0x04000000,
	kernelevent_flag_reserved27	= 0x08000000,
	kernelevent_flag_reserved28	= 0x10000000,
	kernelevent_flag_reserved29	= 0x20000000,
	kernelevent_flag_reserved30	= 0x40000000,
	kernelevent_flag_reserved31	= 0x80000000,

	kernelevent_flag_empty		= 0x00000000,
} kernelevent_flag_t;

void kernel_event_flag_set(kernelevent_flag_t event);
void kernel_event_flag_clear(kernelevent_flag_t event);
bool kernel_event_flag_check(kernelevent_flag_t event);

#endif
