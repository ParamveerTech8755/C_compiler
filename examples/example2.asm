	.globl main
main:
	movl $2, %eax
	movl $5, %ebx
	imull %ebx, %eax
	push %rax
	movl $3, %eax
	push %rax
	movl $32, %eax
	movl $8, %ebx
	cdq
	idivl %ebx
	movl $7, %ebx
	subl %ebx, %eax
	push %rax
	movl $5, %eax
	push %rax
	movl $4, %eax
	movl $1, %ebx
	addl %ebx, %eax
	movl %eax, %ebx
	pop %rax
	cdq
	idivl %ebx
	movl %eax, %ebx
	pop %rax
	addl %ebx, %eax
	movl %eax, %ebx
	pop %rax
	imull %ebx, %eax
	movl %eax, %ebx
	pop %rax
	addl %ebx, %eax
	ret