	section .data
two:	dd	2.0
four:	dd	4.0

	section	.text
	global	assembler_mandelbrot_function

assembler_mandelbrot_function:
	; Data in registers given while calling function
	; edi - pixels_array pointer
	; esi - screen size 
	; xmm0 - mandel_x
	; xmm1 - mandel_y
	; xmm2 - mandel_step

	; clear registes and store data if needed
	xor	edx, edx	; outer counter
	xor	ecx, ecx	; inner counter
	xor	r8d, r8d	; black color
	xor	r10d, r10d	; temporary register to store color
	xor	r9d, r9d	; number of mandelbrot iteration

	xorps	xmm5, xmm5

	sub	rsp, 16
	movdqu	[rsp], xmm6
	xorps	xmm6, xmm6
	sub	rsp, 16
	movdqu	[rsp], xmm7
	xorps	xmm7, xmm7
	sub	rsp, 16
	movdqu	[rsp], xmm8
	xorps	xmm8, xmm8
	sub	rsp, 16
	movdqu	[rsp], xmm9
	xorps	xmm9, xmm9

	; loads float 4 to register (2*2)
	movss	xmm8, [four]

	; remembers mandel_x value 
	movss	xmm9, xmm0

	mov	edx, esi	; sets outer loop counter

outer:
	mov	ecx, esi	; sets inner loop counter

inner:
	xorps	xmm3, xmm3	; xmm3 = current x
	xorps	xmm4, xmm4	; xmm4 = current y
	mov	r9d, 500	

compute:
	movss	xmm5, xmm3	; xmm5 = x^2
	mulss	xmm5, xmm5
	movss	xmm6, xmm4	; xmm6 = y^2
	mulss	xmm6, xmm6

	movss	xmm7, xmm5	; xmm7 = x^2 + y^2 
	addss	xmm7, xmm6

	ucomiss	xmm7, xmm8	; check if x^2 + y^2 > 4

	jnb	no_color	; yes - diverge, no color

	mulss	xmm4, xmm3	; current y = new y
	addss	xmm4, xmm4
	addss	xmm4, xmm1

	movss	xmm3, xmm5	; current x = new x
	subss	xmm3, xmm6
	addss	xmm3, xmm0
	

	dec	r9d
	jnz	compute
	
	mov	r10d, 0xaaddaa	; not diverge, color

no_color:
	cmovnz	r10d, r8d	; or, maybe it shouldn't be colored

end_in:
	mov	[rdi], r10d	; load colored pixel to memory

	; gets next pixel in current line
	add		rdi, 4
	addss	xmm0, xmm2
	loop	inner

end_out:
	; gets next pixel in new line
	addss	xmm1, xmm2
	movss	xmm0, xmm9
	dec	edx
	jnz	outer

end:
	; moves back stacked registers
	movdqu	xmm9, [rsp]
	add	rsp, 16
	movdqu	xmm8, [rsp]
	add	rsp, 16
	movdqu	xmm7, [rsp]
	add	rsp, 16
	movdqu	xmm6, [rsp]
	add	rsp, 16

	ret
