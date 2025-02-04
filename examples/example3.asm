.section .text
	.globl main
main:
	pushq %rbp
	movq %rsp, %rbp
	subq $-8, %rsp
	movl $1, -4(%rbp)
	movl -4(%rbp), %eax
	movl $1, %ebx
	cmpl %ebx, %eax
	movl $0, %eax
	sete %al
	jo _overflow
	cmpl $0, %eax
	jz false2
	movl $10, %eax
	jmp return1
	jmp end2
false2:
	movl -4(%rbp), %eax
	movl $2, %ebx
	cmpl %ebx, %eax
	movl $0, %eax
	sete %al
	jo _overflow
	cmpl $0, %eax
	jz false3
	movl -4(%rbp), %eax
	jmp return1
false3:
end2:
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
