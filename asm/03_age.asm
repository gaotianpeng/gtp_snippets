; 输入一个age,判断是未成年人还是成年人
; 如果是未成年人,输出minor
; 如果是成年人,输出adult

mov ax, 0xb800
mov es, ax

mov ax, 0x7c0
mov ds, ax

mov di, 0

jmp near code

minor:
    db 'minor'

adult:
    db 'adult'

code:
    mov al, 10
    cmp al, 18
    jb  PrintMinor; 小于18岁跳转


PrintAdult:
    mov cx, code - adult
    mov si, adult
    jmp near PrintString

PrintMinor:
    mov cx, adult - minor
    mov si, minor


PrintString:
    mov al, [ds:si]
    mov byte [es:di], al
    inc di
    mov byte [es:di], 7d
    inc di
    inc si
    loop PrintString

end:
    jmp 0x7c0:end

times 500 - ($ - $$) db 0x00
db 0x55, 0xaa
