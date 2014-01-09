#include <stdint.h>
#include <uart.h>
#include <timer.h>
#include <task.h>
#include <rpicore.h>

extern void enable_irq();

#define UNUSED(x) (void)(x)

static uint32_t system_ticks = 0;

const char hello[] = "\r\nThis is >>> rpi-core <<< by ObjectDelta.com, 2014\r\n";

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  UNUSED(r0);
  UNUSED(r1);
  UNUSED(atags);

  uart_init();
  uart_puts(hello);

  init_tasking();
  init_timer();

  enable_irq();

  for (;;);

}

void *c_irq_handler() {

      
  system_ticks+=1;

  schedule_tasks();

  reset_timer_irq();

  return (void*)get_current_task_pcb();
}


uint32_t get_system_ticks()
{
  return system_ticks;
}
