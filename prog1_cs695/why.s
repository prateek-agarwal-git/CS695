	.file	"guest.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello, world!\n"
	.section	.start,"ax",@progbits
	.p2align 4,,15
	.globl	_start
	.type	_start, @function
_start:
.LFB47:
	.cfi_startproc
	leaq	.LC0(%rip), %rdx
	movl	$72, %eax
.L2:
#APP
# 53 "guest.c" 1
	outb %al,$117
# 0 "" 2
#NO_APP
	addq	$1, %rdx
	movzbl	(%rdx), %eax
	testb	%al, %al
	jne	.L2
	movl	$1234, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movl	$4321, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
# 61 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, %ecx
#APP
# 61 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, %edx
	movl	%ecx, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movl	%edx, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movabsq	$8459766130748189032, %rax
	movl	$175403877, 1288
	movb	$0, 1292
	movq	%rax, 1280
#APP
# 61 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, %ecx
	movl	$1280, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
# 61 "guest.c" 1
	in $116, %eax
# 0 "" 2
#NO_APP
	movl	%eax, %edx
	movl	%ecx, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movl	%edx, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movl	$29816, %ecx
	movl	$0, 65540
	movl	$1949184102, 65556
	movw	%cx, 65560
	movb	$0, 65562
	movl	$65536, %eax
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movl	$65576, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
#NO_APP
	movl	$29816, %esi
	movl	$65792, %ecx
	movl	$2, 65796
	movl	$1949184358, 65812
	movw	%si, 65816
	movl	%ecx, %eax
	movb	$0, 65818
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movl	$65832, %edx
	movl	%edx, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
#NO_APP
	movl	$10, 65808
	movl	%ecx, %eax
#APP
# 38 "guest.c" 1
	outl %eax,$118
# 0 "" 2
#NO_APP
	movl	%edx, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
#NO_APP
	movl	$29816, %edi
	movl	$66048, %ecx
	movl	$2, 66052
	movl	$1949184614, 66068
	movw	%di, 66072
	movl	%ecx, %eax
	movb	$0, 66074
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movl	$66088, %edx
	movl	%edx, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
#NO_APP
	movl	$200, 66064
	movl	%ecx, %eax
#APP
# 38 "guest.c" 1
	outl %eax,$118
# 0 "" 2
#NO_APP
	movl	%edx, %eax
#APP
# 65 "guest.c" 1
	outl %eax,$114
# 0 "" 2
#NO_APP
	movl	$29816, %r8d
	movl	$1026, 66308
	movl	$1949184870, 66324
	movw	%r8w, 66328
	movb	$0, 66330
	movl	$66304, %eax
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movl	$200, 66320
	xorl	%eax, %eax
.L3:
	movslq	%eax, %rdx
	addl	$1, %eax
	movzbl	66088(%rdx), %ecx
	cmpl	$200, %eax
	movb	%cl, 66344(%rdx)
	jne	.L3
	movl	$66304, %eax
#APP
# 41 "guest.c" 1
	outl %eax,$119
# 0 "" 2
#NO_APP
	movl	$29816, %eax
	movl	$2, 66564
	movl	$1949185126, 66580
	movw	%ax, 66584
	movb	$0, 66586
	movl	$66560, %eax
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movabsq	$751082403512018785, %rsi
	movabsq	$-4294967294, %rdi
	movb	$0, 66608
	movq	%rsi, 66600
	movq	%rdi, 66568
	movl	$8, 66576
#APP
# 44 "guest.c" 1
	outl %eax,$121
# 0 "" 2
# 41 "guest.c" 1
	outl %eax,$119
# 0 "" 2
#NO_APP
	movl	66048, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movl	$66048, %eax
#APP
# 48 "guest.c" 1
	outl %eax,$128
# 0 "" 2
#NO_APP
	movl	$29816, %edx
	movl	$2, 66820
	movl	$1949185382, 66836
	movw	%dx, 66840
	movb	$0, 66842
	movl	$66816, %eax
#APP
# 35 "guest.c" 1
	outl %eax,$120
# 0 "" 2
#NO_APP
	movl	66816, %eax
#APP
# 57 "guest.c" 1
	outl %eax,$115
# 0 "" 2
#NO_APP
	movq	$42, 1024
	movl	$42, %eax
	.p2align 4,,10
	.p2align 3
.L4:
#APP
# 151 "guest.c" 1
	hlt
# 0 "" 2
#NO_APP
	jmp	.L4
	.cfi_endproc
.LFE47:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
