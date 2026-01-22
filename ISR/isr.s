	.cpu arm7tdmi
	.arch armv4t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"isr.c"
	.text
	.align	2
	.global	isr
	.syntax unified
	.arm
	.type	isr, %function
isr:
	@ Interrupt Service Routine.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r2, r3, fp}
	add	fp, sp, #8
	sub	sp, sp, #12
	ldr	r3, .L9
	ldr	r3, [r3]
	strb	r3, [fp, #-17]
	ldrb	r3, [fp, #-17]	@ zero_extendqisi2
	and	r3, r3, #1
	cmp	r3, #0
	beq	.L2
	ldr	r3, .L9+4
	str	r3, [fp, #-16]
	b	.L3
.L4:
	.syntax divided
@ 24 "isr.c" 1
	nop
@ 0 "" 2
	.arm
	.syntax unified
.L3:
	ldr	r3, [fp, #-16]
	sub	r2, r3, #1
	str	r2, [fp, #-16]
	cmp	r3, #0
	bne	.L4
.L2:
	ldrb	r3, [fp, #-17]	@ zero_extendqisi2
	and	r3, r3, #2
	cmp	r3, #0
	beq	.L8
	ldr	r3, .L9+8
	str	r3, [fp, #-16]
	b	.L6
.L7:
	.syntax divided
@ 30 "isr.c" 1
	nop
@ 0 "" 2
	.arm
	.syntax unified
.L6:
	ldr	r3, [fp, #-16]
	sub	r2, r3, #1
	str	r2, [fp, #-16]
	cmp	r3, #0
	bne	.L7
.L8:
	nop
	sub	sp, fp, #8
	@ sp needed
	pop	{r2, r3, fp}
	subs	pc, lr, #4
.L10:
	.align	2
.L9:
	.word	1611401088
	.word	1000000
	.word	10000
	.size	isr, .-isr
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	bl	isr
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (15:14.2.rel1-1) 14.2.1 20241119"
