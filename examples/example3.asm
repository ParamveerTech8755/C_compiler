.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $-8, %rsp
	movl $0, -4(%rbp)
	movl $0, -8(%rbp)
loop2:
	movl -8(%rbp), %eax
	movl $11, %ebx
	cmpl %ebx, %eax
	movl $0, %eax
	setl %al
	jo _overflow
	cmpl $0, %eax
	jz end2
	movl -8(%rbp), %eax
	incl -8(%rbp)
	jo _overflow
	addl %eax, -4(%rbp)
	jo _overflow
	jmp loop2
end2:
	movl -4(%rbp), %eax
	movq %rbp, %rsp
	popq %rbp
	ret
_overflow:
	mov $60, %rax
	mov $2147483647, %rdi
	syscall
