#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

enum timer {
  TIMER_1 = 0,
  TIMER_2 = 1,
  TIMER_3 = 2,
  TIMER_4 = 3
};

void init_system_timer();
void set_system_timer(enum timer timer, uint32_t ticks);

#endif
