;在屏幕上打印一个字符串

mov ax, 0x7c0
mov ds, ax

mov ax, 0x070
mov ss, ax
mov bp, 0xffff
mov sp, 0xffff

jmp near code

string:
    db 'I miss you!', 0x00
display_index:
    dw 0x0000

; 输入参数, 字符串数据地址:段地址,偏移地址
print_string:
    push ds  ;保存ds段寄存器的值
    push es  ;保存es段寄存器的值
    push ax  ;保存ax寄存器的值
    push di  ;保存目标地址偏移寄存器
    push si  ;保存原始地址偏移寄存器

    mov ds,word[ss:bp-2] ;初始化要输出的文本数据段寄存器
    mov si,word[ss:bp-4] ;初始化要输出的文本数据偏移寄存器

    mov ax,0xB800
    mov es,ax  ;初始化显示段寄存器
    mov di,word[ds:display_index]

printf:
    mov al,byte [ds:si]
    cmp al,0
    jz print_end
    mov byte[es:di],al
    inc di
    mov byte[es:di],0x07
    inc di
    inc si
    jmp near printf

print_end:
    mov word[ds:display_index],di
    pop si
    pop di
    pop ax
    pop es
    pop ds
    retf

 
code:
    push 0x7c0
    push string
    call 0x7c0:print_string
    add sp, 4

end:
    jmp near end

times 500 - ($ - $$) db 0x00
db 0x55, 0xaa
