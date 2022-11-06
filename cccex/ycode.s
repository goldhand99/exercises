	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 3
	.globl	_rproduct                       ## -- Begin function rproduct
	.p2align	4, 0x90
_rproduct:                              ## @rproduct
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	cmpq	$1, -24(%rbp)
	jg	LBB0_2
## %bb.1:
	movq	$1, -8(%rbp)
	jmp	LBB0_3
LBB0_2:
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rcx
	addq	$8, %rcx
	movq	-24(%rbp), %rdx
	subq	$1, %rdx
	movq	%rcx, %rdi
	movq	%rdx, %rsi
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	callq	_rproduct
	movq	-32(%rbp), %rcx                 ## 8-byte Reload
	imulq	%rax, %rcx
	movq	%rcx, -8(%rbp)
LBB0_3:
	movq	-8(%rbp), %rax
	addq	$32, %rsp
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
	xorl	%eax, %eax
	movl	$0, -4(%rbp)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
