.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	movl $6, %eax
	mov %rbp, %rsp
	pop %rbp
	ret
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
