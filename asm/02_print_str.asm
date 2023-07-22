; print string
xor ax, ax

mov ax, 0xb800
mov es, ax

mov cx, loop_print - string
mov ax, 0x7c0
mov ds, ax

mov si, 0D
mov di, 0D

jmp loop_print

string:
    db 'I miss you!'

loop_print:
    mov byte al,[string + si]
    mov byte [es:di], al
    inc di
    mov byte [es:di], 7D
    inc di
    inc si

    loop loop_print

end:
    jmp 0x7c0:end

times 510 - ($ - $$) db 0x00
db 0x55, 0xaa
