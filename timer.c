#include <stdint.h>
#include <timer.h>
#include <mmio.h>

#define SYSTEM_TIMER_HZ 1000000

enum {
  INT_BASE = 0x2000B000,
  INT_IRQ_BASIC_ENABLE = INT_BASE + 0x218,
  INT_IRQ_BASIC_PENDING = INT_BASE + 0x200,
  INT_IRQ_BASIC_DISABLE = INT_BASE + 0x224,

  TIMER_BASE = 0x2000B400,
  TIMER_LOAD = TIMER_BASE,
  TIMER_VALUE = TIMER_BASE + 0x04,
  TIMER_CONTROL = TIMER_BASE + 0x08,
  TIMER_IRQ_CLEAR = TIMER_BASE + 0x0C,
  TIMER_RAW_IRQ = TIMER_BASE + 0x10,
  TIMER_MASKED_IRQ = TIMER_BASE + 0x14,
  TIMER_RELOAD = TIMER_BASE + 0x18,
  TIMER_PREDIV = TIMER_BASE + 0x1C,
  TIMER_FREERUNCTR = TIMER_BASE + 0x20
};

void init_timer()
{
  mmio_write(INT_IRQ_BASIC_DISABLE, 1);
  mmio_write(TIMER_CONTROL, 0x3E0000);
  mmio_write(TIMER_PREDIV, 0xF9);
  mmio_write(TIMER_LOAD, 1000-1);
  mmio_write(TIMER_RELOAD, 1000-1);
  mmio_write(TIMER_IRQ_CLEAR, 0);
  mmio_write(TIMER_CONTROL, 0x003E00A2);
  mmio_write(INT_IRQ_BASIC_ENABLE, 1);
}

void reset_timer_irq()
{
  mmio_write(TIMER_IRQ_CLEAR, 1);
}

void enable_timer_irq()
{
  mmio_write(INT_IRQ_BASIC_ENABLE, (1 << 0));
}

void disable_timer_irq()
{
  mmio_write(INT_IRQ_BASIC_DISABLE, (1 << 0));
}

int isTimerMaskedIrq()
{
  return mmio_read(TIMER_MASKED_IRQ);
}

int isTimerRawIrq()
{
  return mmio_read(TIMER_RAW_IRQ);
}
