#include <stdint.h>
#include <uart.h>
#include <timer.h>

extern void enable_irq();

#define UNUSED(x) (void)(x)

const char hello[] = "\r\nThis is >>> rpi-core <<< by ObjectDelta.com, 2014\r\n";

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  UNUSED(r0);
  UNUSED(r1);
  UNUSED(atags);

  uart_init();
  uart_puts(hello);

  init_timer();

  enable_irq();

  for (;;);

}

void c_irq_handler() {
  
  static volatile int irq_count = 0;

  irq_count++;
  uart_putc('0'+(irq_count % 10));

  reset_timer_irq();
}
