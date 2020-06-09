#include "stdint.h"
#include "UART.h"
#include "Interrupt.h"
#include "Timer.h"

volatile PL011_t *UART = (PL011_t *)UART_BASE_ADDRESS0;
volatile GicCput_t *GicCpu = (GicCput_t *)GIC_CPU_BASE;
volatile GicDist_t *GicDist = (GicDist_t *)GIC_DIST_BASE;
volatile Timer_t *timer = (Timer_t *)TIMER_CPU_BASE;
