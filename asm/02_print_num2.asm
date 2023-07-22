;打印 100~999
mov ax, 0x7c0
mov ds, ax

jmp 0x7c0:code

number:
    db 0, 0, 0
   
code:
    mov ax, 999D
    mov cl, 10D
    div cl
    mov byte [ds:number+2], ah

    mov ah, 0
    div cl
    mov byte [ds:number+1], ah


    mov ah, 0
    div cl
    mov byte [ds:number], ah
    
    ; print number
    mov ax, 0xb800
    mov es, ax

    mov al, [ds:number+0]
    add al, 48
    mov byte [es:0], al
    mov byte [es:1],7d

    mov al, [ds:number+1]
    add al, 48
    mov byte [es:2],al
    mov byte [es:3],7d

    mov al, [ds:number+2]
    add al, 48
    mov byte [es:4],al
    mov byte [es:5],7d

times 510 - ($ - $$) db 0x00
db 0x55, 0xaa