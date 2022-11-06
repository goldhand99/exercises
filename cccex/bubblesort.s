	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 3
	.globl	_bubble_a                       ## -- Begin function bubble_a
	.p2align	4, 0x90
_bubble_a:                              ## @bubble_a
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	shlq	$3, %rcx
	addq	%rcx, %rax
	addq	$-8, %rax
	movq	%rax, -24(%rbp)
LBB0_1:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB0_3 Depth 2
	movq	-24(%rbp), %rax
	cmpq	-8(%rbp), %rax
	jbe	LBB0_8
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)
LBB0_3:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	movq	-32(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jae	LBB0_7
## %bb.4:                               ##   in Loop: Header=BB0_3 Depth=2
	movq	-32(%rbp), %rax
	movq	(%rax), %rsi
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movq	-32(%rbp), %rcx
	movq	8(%rcx), %rcx
	movq	-32(%rbp), %rdx
	cmpq	(%rdx), %rcx
	jge	LBB0_6
## %bb.5:                               ##   in Loop: Header=BB0_3 Depth=2
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rcx
	movq	%rax, 8(%rcx)
	movq	-40(%rbp), %rax
	movq	-32(%rbp), %rcx
	movq	%rax, (%rcx)
LBB0_6:                                 ##   in Loop: Header=BB0_3 Depth=2
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	%rax, -32(%rbp)
	jmp	LBB0_3
LBB0_7:                                 ##   in Loop: Header=BB0_1 Depth=1
	movq	-24(%rbp), %rax
	addq	$-8, %rax
	movq	%rax, -24(%rbp)
	jmp	LBB0_1
LBB0_8:
	movq	-8(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$112, %rsp
	leaq	-64(%rbp), %rax
	leaq	l___const.main.a(%rip), %rcx
	movq	___stack_chk_guard@GOTPCREL(%rip), %rdx
	movq	(%rdx), %rdx
	movq	%rdx, -8(%rbp)
	movl	$0, -68(%rbp)
	movl	%edi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%rax, %rdx
	movq	%rdx, %rdi
	movq	%rcx, %rsi
	movl	$56, %edx
	movq	%rax, -104(%rbp)                ## 8-byte Spill
	callq	_memcpy
	movq	-104(%rbp), %rdi                ## 8-byte Reload
	movl	$7, %esi
	callq	_bubble_a
	movq	%rax, -88(%rbp)
	movl	$0, -92(%rbp)
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$7, -92(%rbp)
	jge	LBB1_4
## %bb.2:                               ##   in Loop: Header=BB1_1 Depth=1
	movq	-88(%rbp), %rax
	movslq	-92(%rbp), %rcx
	movq	(%rax,%rcx,8), %rsi
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
## %bb.3:                               ##   in Loop: Header=BB1_1 Depth=1
	movl	-92(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -92(%rbp)
	jmp	LBB1_1
LBB1_4:
	movl	-68(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -108(%rbp)                ## 4-byte Spill
	jne	LBB1_6
## %bb.5:
	movl	-108(%rbp), %eax                ## 4-byte Reload
	addq	$112, %rsp
	popq	%rbp
	retq
LBB1_6:
	callq	___stack_chk_fail
	ud2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%ld"

	.section	__TEXT,__const
	.p2align	4                               ## @__const.main.a
l___const.main.a:
	.quad	1                               ## 0x1
	.quad	4                               ## 0x4
	.quad	6                               ## 0x6
	.quad	10                              ## 0xa
	.quad	2                               ## 0x2
	.quad	3                               ## 0x3
	.quad	5                               ## 0x5

.subsections_via_symbols
