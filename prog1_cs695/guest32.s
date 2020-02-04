	.file	"guest.c"
	.text
	.type	outb, @function
outb:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$4, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movb	%al, -4(%ebp)
	movzbl	-4(%ebp), %eax
#APP
# 16 "guest.c" 1
	outb %al,$117
# 0 "" 2
#NO_APP
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	outb, .-outb
	.type	printVal, @function
printVal:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
#APP
# 20 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	printVal, .-printVal
	.type	getNumExits, @function
getNumExits:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
#APP
# 24 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
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
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	call	getNumExits
	movl	%eax, -32(%ebp)
	call	getNumExits
	movl	%eax, -28(%ebp)
	call	getNumExits
	movl	%eax, -24(%ebp)
	call	getNumExits
	movl	%eax, -20(%ebp)
	call	getNumExits
	movl	%eax, -16(%ebp)
	call	getNumExits
	movl	%eax, -12(%ebp)
	leal	.LC0@GOTOFF(%ebx), %eax
	movl	%eax, -36(%ebp)
	jmp	.L6
.L7:
	movl	-36(%ebp), %eax
	movzbl	(%eax), %eax
	movzbl	%al, %eax
	pushl	%eax
	call	outb
	addl	$4, %esp
	addl	$1, -36(%ebp)
.L6:
	movl	-36(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L7
	call	getNumExits
	movl	%eax, -8(%ebp)
	pushl	-32(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-28(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-8(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-24(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-20(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-16(%ebp)
	call	printVal
	addl	$4, %esp
	pushl	-12(%ebp)
	call	printVal
	addl	$4, %esp
	movl	$1024, %eax
	movl	$42, (%eax)
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
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB4:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE4:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB5:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE5:
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
