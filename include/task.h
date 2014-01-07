#ifndef TASK_H
#define TASK_H

#include <stdint.h>


enum taskstate {
  TASK_STATE_FREE = 0,
  TASK_STATE_READY = 1,
  TASK_STATE_RUNNING = 2,
  TASK_STATE_PAUSE = 3,
  TASK_STATE_EXIT = 4
};

typedef struct {
  uint32_t spsr,lr,r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14;
} __attribute__((PACKED)) task_pcb_t;


void init_tasking();

int new_task(uint32_t entry);
void resume_task(int index);
void pause_task(int index);
void remove_task(int index);

void schedule_tasks();

uint32_t get_current_task_index();
task_pcb_t *get_current_task_pcb();

#endif
