; 计算1 ~ 100 的和,将运算结果打印在屏幕上
mov ax, 0x7c0
mov ds, ax

mov ax, 0x00
mov ss, ax
mov sp, 0xffff
mov bp, 0xffff

jmp near code

start_number:
    dw 1
end_number:
    dw 100
divisor:
    dw 10
count:
    dw 0


code:
    xor ax, ax
    mov ax, [ds:start_number]
    mov cx, ax
    xor ax, ax

sum:
    add ax, cx
    inc cx
    cmp cx, word [ds:end_number]
    jbe sum

; 被除数dx:ax, 除数16bit, 商ax, 余数dx
split:
    xor dx, dx
    div word [ds:divisor]
    add dx, 0x30
    push dx
    inc word [ds:count]
    cmp ax, 0
    jne near split

; 初始化屏幕输出
mov ax, 0xb800
mov es, ax
xor di, di
mov cx, word [ds:count]

print:
    pop ax
    mov byte [es:di], al
    inc di
    mov byte [es:di], 0x07
    inc di
    loop print

times 500 - ($ - $$) db 0x00
db 0x55, 0xaa
