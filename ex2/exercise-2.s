	.file	"fib.c"
	.text
	.globl	fibonacci
	.type	fibonacci, @function
fibonacci:

        #<*YOUR x86-64 ASSEMBLY CODE HERE!*>
        
        # Compile with: gcc -fPIC -static exercise-2.s
        
        # function prologue
        pushq   %rbp
        movq    %rsp, %rbp
        
        # make space for two 4-byte ints on stack: 
        # -4(%rbp) is curr_num, -8(%rbp) is f(n-1)
        subq    $8, %rsp     
        
        # n is passed in edi, copy to local var
        movl    %edi, -4(%rbp)
        cmp     $3, %edi
        jge     is_two_or_more
        
        # else case: just return 1 (treat negative numbers the same too)
        movl    $1, %eax
        jmp     epilogue
        
is_two_or_more: 
        # f(n) = f(n-1) + f(n-2)
        
        # use edi to pass n
        movl    -4(%rbp), %edi
        subl    $1, %edi
        call    fibonacci
        movl    %eax, -8(%rbp)
        
        movl    -4(%rbp), %edi
        subl    $2, %edi
        call    fibonacci
        addl    -8(%rbp), %eax
        
epilogue: 
        movq    %rbp, %rsp
        popq    %rbp
        ret        
	
	#<*END OF ASSEMBLY CODE!*>
	
.LFE0:
	.size	fibonacci, .-fibonacci
	.section	.rodata
.LC0:
	.string	"Fibonacci of %d is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	atoi
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	fibonacci
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leave
	ret
.LFE1:
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
