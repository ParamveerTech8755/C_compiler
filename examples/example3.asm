.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	movl $4, -8(%rbp)
	movl -8(%rbp), %eax
	movl $1, %ebx
	addl %ebx, %eax
	jo _overflow
	mov %rbp, %rsp
	pop %rbp
	ret
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
