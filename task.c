#include <task.h>
#include <stdint.h>
#include <uart.h>

#define MAX_TASKS 10
#define STACK_SIZE 1024

struct task_t {
  uint32_t *user_stack;
  uint32_t *kernel_stack;
  task_pcb_t *pcb;
  enum taskstate state;
};

static uint32_t stacks[MAX_TASKS*3*STACK_SIZE] __attribute__((ALIGN (4096)));
static struct task_t tasks[MAX_TASKS];
static int current_task_index;

extern void halt();

void idle()
{
  int i;
  for (;;)
    {
      uart_putc('.');
      for (i=0;i<1000000;i++);
    }
}

void idle2()
{
  int i;
  for (;;)
    {
      uart_putc('+');
      for (i=0;i<1000000;i++);
    }
}

void reset_task()
{
  char c;
  void (*kernel_reset)() = (void(*)())0x00000000;
  void (*boot_reset)() = (void(*)())0x00100000;
  
  for (;;)
    {
      c = uart_getc('+');
      if (c == 'r')
	kernel_reset();
      if (c == 'b')
	boot_reset();
 
    }
}


void init_tasking()
{
  int i, j;
  uint32_t s = 0;

  s = 0;
  for (i=0; i < MAX_TASKS; i++)
    {
      tasks[i].user_stack = (uint32_t*)(stacks + s); s += STACK_SIZE;
      tasks[i].kernel_stack = (uint32_t*)(stacks + s); s += STACK_SIZE;
      for (j = 0; j < STACK_SIZE; j++)
	tasks[i].user_stack[j] = 0x00000000;
      for (j = 0; j < STACK_SIZE; j++)
	tasks[i].kernel_stack[j] = 0x00000000;
      tasks[i].pcb = (task_pcb_t*)(tasks[i].kernel_stack + (STACK_SIZE - 17)); // 17 Register
      tasks[i].state = TASK_STATE_FREE;
    }

  // Idle-Task (Task 0)
  new_task((uint32_t)idle);
  resume_task(0);

  // Idle-Task2 (Task 1)
  resume_task(new_task((uint32_t)idle2));

  // Reset-Task2 (Task 2)
  resume_task(new_task((uint32_t)reset_task));

  current_task_index = 0;
}

int new_task(uint32_t entry)
{
  int task_index;
  //  uint32_t *kernel_stack;
  task_pcb_t *pcb;

  for (task_index = 0; task_index < MAX_TASKS; task_index++)
    {
      if (tasks[task_index].state == TASK_STATE_FREE)
	break;
    }
  if (task_index >= MAX_TASKS)
    return -1;

  pcb = tasks[task_index].pcb;
  pcb->r14 = 0;
  pcb->r13 = (uint32_t)(tasks[task_index].user_stack + STACK_SIZE);
  pcb->r12 = 0;
  pcb->r11 = 0;
  pcb->r10 = 0;
  pcb->r9 = 0;
  pcb->r8 = 0;
  pcb->r7 = 0;
  pcb->r6 = 0;
  pcb->r5 = 0;
  pcb->r4 = 0;
  pcb->r3 = 0;
  pcb->r2 = 0;
  pcb->r1 = 0;
  pcb->r0 = 0;

  pcb->lr = entry + 4;
  pcb->spsr = 0x50;

  tasks[task_index].state = TASK_STATE_PAUSE;

  return task_index;
}

void resume_task(int index)
{
  if ((index >= 0) && (index < MAX_TASKS))
    {
      tasks[index].state = TASK_STATE_READY;
    }
}

void pause_task(int index)
{
  if ((index >= 0) && (index < MAX_TASKS))
    {
      tasks[index].state = TASK_STATE_PAUSE;
    }
}

void remove_task(int index)
{
  if ((index >= 0) && (index < MAX_TASKS))
    {
      tasks[index].state = TASK_STATE_EXIT;
    }
}

void schedule_tasks()
{
  tasks[current_task_index].state = TASK_STATE_READY;
  current_task_index = (current_task_index + 1) % MAX_TASKS;

  while (1)
    {
      if (tasks[current_task_index].state == TASK_STATE_READY)
	break;
      current_task_index = (current_task_index + 1) % MAX_TASKS;
    }

}

uint32_t get_current_task_index()
{
  return current_task_index;
}

task_pcb_t *get_current_task_pcb()
{
  return tasks[current_task_index].pcb;
}
