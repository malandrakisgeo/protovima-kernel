
org	0x7c00

	cli					; registers set up, no interruptions
	xor	ax, ax				; set to zero
	mov	ds, ax
	mov	ss, ax
	mov	sp, 0x7c00			; stack at 0000:7c00 (last address)
	mov	ax, 0x1000
	mov	es, ax			
	sti


	cli
	mov	ax, 0x1000
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	mov	sp, 0x1000
	jmp	0x1000
