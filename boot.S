.section ".text.boot"

.globl Start


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
undefined_handler:	.word halt
swi_handler:		.word halt
prefetch_handler:	.word halt
data_handler:		.word halt
unused_handler:		.word halt
irq_handler:		.word irq
fiq_handler:		.word halt

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
	b reset

irq:
	push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
	bl c_irq_handler
	pop {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
	subs pc, lr, #4


.globl enable_irq

enable_irq:
	mrs r0,cpsr
	bic r0,r0,#0x80
	msr cpsr_c,r0
	bx lr
