#include <stdint.h>
#include <uart.h>
#include <timer.h>

extern void enable_irq();

#define UNUSED(x) (void)(x)

const char hello[] = "\r\nHello :)\r\n";

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
  //  int x = 0;

  UNUSED(r0);
  UNUSED(r1);
  UNUSED(atags);

  uart_init();
  uart_puts(hello);
  /*
  while (1)
    {
      uart_putc(uart_getc());
    }
  */

  enable_irq();
  set_system_timer(TIMER_1, 100000);

  for (;;);

  //  x /= x;
}

void c_irq_handler() {
  static volatile int irq_count = 0;

  irq_count++;
  uart_putc('0'+(irq_count % 10));
  set_system_timer(TIMER_1, 1000);  
}
