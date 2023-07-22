; AX:被除数
; CL:除数
; AL:商  AH:余数
start:
	mov ax, 378
	mov bl, 37
	div bl
	
current:
	times 510 - (current - start) db 0
	
	db 0x55,0xaa