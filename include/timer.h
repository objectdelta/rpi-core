#ifndef TIMER_H
#define TIMER_H

void init_timer();
void reset_timer_irq();
void enable_timer_irq();
void disable_timer_irq();
int isTimerMaskedIrq();
int isTimerRawIrq();

#endif
