.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $-8, %rsp
	movl $1, -4(%rbp)
	movl $0, -8(%rbp)
	movl $98, %eax
	addl %eax, -8(%rbp)
	jo _overflow
	movl -8(%rbp), %eax
	jmp return1
	movl -4(%rbp), %eax
	movl $2, %ebx
	addl %ebx, %eax
	jo _overflow
return1:
	movq %rbp, %rsp
	popq %rbp
	ret
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
