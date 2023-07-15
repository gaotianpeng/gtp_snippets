start:
	mov ax, 378
	mov bl, 37
	div bl		;AL=商(10), AH=余数(8)
	
current:
	times 510 - (current - start) db 0
	
	db 0x55,0xaa