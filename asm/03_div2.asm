start:
	mov ax,65535
	mov dx,0
	mov bx,10
	div bx
current:
	times 510 - (current - start) db 0
	
	db 0x55,0xaa