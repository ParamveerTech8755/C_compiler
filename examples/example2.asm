.section .text
	.globl main
main:
	movl $2, %eax
	movl $5, %ebx
	imull %ebx, %eax
	jo _overflow
	push %rax
	movl $3, %eax
	push %rax
	movl $32, %eax
	movl $8, %ebx
	cdq
	idivl %ebx
	jo _overflow
	movl $7, %ebx
	subl %ebx, %eax
	jo _overflow
	push %rax
	movl $5, %eax
	push %rax
	movl $4, %eax
	movl $1, %ebx
	addl %ebx, %eax
	jo _overflow
	movl %eax, %ebx
	pop %rax
	cdq
	idivl %ebx
	jo _overflow
	pop %rbx
	addl %ebx, %eax
	jo _overflow
	pop %rbx
	imull %ebx, %eax
	jo _overflow
	pop %rbx
	addl %ebx, %eax
	jo _overflow
	mov %rax, %rdi
	mov $60, %rax
	syscall
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
