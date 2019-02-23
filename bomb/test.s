	.file	"test.c"
	.text
	.section	.rodata
.LC0:
	.string	"HHHH"
	.align 8
.LC1:
	.string	"Bjajdgladgmla;dmalfmamdflkamfdldascmx"
.LC2:
	.string	"jgadmglamdOIJSSFLKSFXM"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	leaq	.LC0(%rip), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC1(%rip), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, -28(%rbp)
	jmp	.L2
.L3:
	addl	$1, -28(%rbp)
.L2:
	cmpl	$100, -28(%rbp)
	jle	.L3
	leaq	.LC2(%rip), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20181127"
	.section	.note.GNU-stack,"",@progbits
