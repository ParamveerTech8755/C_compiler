.section .text
	.globl main
main:
	movl $16, %eax
	neg %eax
	jo _overflow
	mov %rax, %rdi
	mov $60, %rax
	syscall
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
