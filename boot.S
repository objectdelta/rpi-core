.section ".text.boot"

.globl Start
.globl halt

Start:
	ldr pc, reset_handler
	ldr pc, undefined_handler
	ldr pc, swi_handler
	ldr pc, prefetch_handler
	ldr pc, data_handler
	ldr pc, unused_handler
	ldr pc, irq_handler
	ldr pc, fiq_handler

reset_handler:		.word reset
undefined_handler:	.word undefined
swi_handler:		.word svc_call
prefetch_handler:	.word prefetch
data_handler:		.word data
unused_handler:		.word unused
irq_handler:		.word irq
fiq_handler:		.word fiq

reset:
	mov r0, #0x8000
	mov r1, #0x0000

	ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
	ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

	;@ Stackpointer fuer die einzelnen Modi setzen

	;@ IRQ-MODE
	mov r0,#0xD2
	msr cpsr_c,r0
	mov sp, #0x6000
	;@ FIQ-Mode
	mov r0,#0xD1
	msr cpsr_c,r0
	mov sp,#0x4000
	;@ SVC-Mode (Supervisor/Kernel), IRQ's disabled
	mov r0,#0xD3
	msr cpsr_c, r0
	mov sp,#0x8000

	bl kernel_main
halt:
	b halt


undefined:
	b undefined

svc_call:
	subs pc, lr, #4

prefetch:
	b prefetch
data:
	b data
unused:
	b unused
fiq:
	b fiq
	
irq:
	stmdb sp,{r0-lr}^
	
	sub sp, #60
	mrs r0, SPSR
	stmdb sp, {r0, lr}
	sub sp, #8

	bl c_irq_handler

	mov sp, r0
	ldmia sp, {r0, lr}
	msr spsr_cxsf, r0
	add sp, #8
	ldmia sp, {r0-lr}^ 
	add sp, #60
	
	subs pc, lr, #4


.globl enable_irq

enable_irq:
	mrs r0,cpsr
	bic r0,r0,#0x80
	msr cpsr_c,r0
	bx lr
