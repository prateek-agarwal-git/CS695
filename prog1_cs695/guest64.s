	.file	"guest.c"
	.text
	.type	outb, @function
outb:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	movzbl	-4(%rbp), %eax
#APP
# 16 "guest.c" 1
	outb %al,$117
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	outb, .-outb
	.type	printVal, @function
printVal:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
#APP
# 20 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	printVal, .-printVal
	.type	getNumExits, @function
getNumExits:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 24 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	getNumExits, .-getNumExits
	.section	.rodata
.LC0:
	.string	"Balle world\n"
	.section	.start,"ax",@progbits
	.globl	_start
	.type	_start, @function
_start:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -36(%rbp)
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -32(%rbp)
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -28(%rbp)
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -24(%rbp)
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -20(%rbp)
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -16(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -8(%rbp)
	jmp	.L6
.L7:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	movl	%eax, %edi
	call	outb
	addq	$1, -8(%rbp)
.L6:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L7
	movl	$0, %eax
	call	getNumExits
	movl	%eax, -12(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-32(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-12(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-28(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-24(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	-16(%rbp), %eax
	movl	%eax, %edi
	call	printVal
	movl	$1024, %eax
	movq	$42, (%rax)
.L8:
	movl	$42, %eax
#APP
# 88 "guest.c" 1
	hlt
# 0 "" 2
#NO_APP
	jmp	.L8
	.cfi_endproc
.LFE3:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
