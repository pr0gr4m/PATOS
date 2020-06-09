#ifndef HAL_HALINTERRUPT_H_
#define HAL_HALINTERRUPT_H_

#define INTERRUPT_HANDLER_NUM	255

typedef void (* interrupt_handler_t)(void);

void hal_interrupt_init(void);
void hal_interrupt_enable(uint32_t interrupt_num);
void hal_interrupt_disable(uint32_t interrupt_num);
void hal_interrupt_register_handler(interrupt_handler_t handler, uint32_t interrupt_num);
void hal_interrupt_run_handler(void);

#endif
