#include <stdint.h>
#include <timer.h>
#include <mmio.h>

enum {
  TIMER_BASE = 0x20203000,
  TIMER_CS = TIMER_BASE + 0x00,
  TIMER_CLO = TIMER_BASE + 0x04,
  TIMER_CHI = TIMER_BASE + 0x08,
  TIMER_C0 = TIMER_BASE + 0x0C,
  TIMER_C1 = TIMER_BASE + 0x10,
  TIMER_C2 = TIMER_BASE + 0x14,
  TIMER_C3 = TIMER_BASE + 0x18,

  ENABLE_BASIC_IRQ = 0x2000B218
};

void set_system_timer(enum timer timer, uint32_t ticks)
{
  uint32_t r;

  // Clear match detect...
  r = mmio_read(TIMER_CS);
  r &= ~(1 << timer);
  mmio_write(TIMER_CS, r);

  // Set Compare Register
  r = mmio_read(TIMER_C0 + timer);
  mmio_write(TIMER_C0 + timer, r + ticks);

  mmio_write(ENABLE_BASIC_IRQ, 1);
}
