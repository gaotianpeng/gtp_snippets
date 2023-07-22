mov ax, 0xb800
mov es, ax
mov ax, 0x7c0
mov ds,ax

jmp 0x7c0:code
number:
    db 7,8,9
code:
    mov al, [ds:number + 0]
    add al,48
    mov byte [es:0],al
    mov byte [es:1], 7

    mov al, [ds:number + 1]
    add al,48
    mov byte [es:2],al
    mov byte [es:3], 7

    mov al, [ds:number + 2]
    add al,48
    mov byte [es:4],al
    mov byte [es:5], 7

times 510-($-$$) db 00
db 0x55,0xaa