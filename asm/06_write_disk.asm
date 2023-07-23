
;以下常量为操作硬盘的8个端口及注释
HD_PORT_DATA equ 0x1F0 ;16位 数据端口
HD_PORT_ERROR equ 0x1F1 ;8位 错误提示
HD_PORT_COUNT equ 0x1F2 ;8位 操作扇区数量长度
HD_PORT_LBA28_4 equ 0x1F3 ;8位 LBA28 0~7位
HD_PORT_LBA28_3 equ 0x1F4 ;8位 LAB28 8~15位
HD_PORT_LBA28_2 equ 0x1F5 ;8位 LBA28 16~23位
HD_PORT_LBA28_1 equ 0x1F6 ;8位 LBA28 24~27位 28~31位要求是0xE，代表启动LBA28模式
HD_PORT_STATUS equ 0x1F7 ;8位 显示硬盘的工作状态
HD_PORT_COMMAND equ 0x1F7 ;8位 告诉硬盘读或者写。
HD_READ  equ 0x20 ;读扇区
HD_WRITE equ 0x30 ;写扇区
HD_SECTION_COUNT equ 2;要读写扇区数量的长度

;假设现在要往第1扇区，写入512个字节，并且每个字节都是0x77。


;设置起始扇区
mov al,0x01
mov dx,HD_PORT_LBA28_4
out dx,al

mov al,0x00
mov dx,HD_PORT_LBA28_3
out dx,al

mov dx,HD_PORT_LBA28_2
out dx,al

mov al,0xE0
mov dx,HD_PORT_LBA28_1
out dx,al

;设置要操作几个扇区。
mov al,HD_SECTION_COUNT
mov dx,HD_PORT_COUNT
out dx,al

 

;告诉命令端口我要写硬盘
mov al,HD_WRITE
mov dx,HD_PORT_COMMAND
out dx,al

Waits:  
in al,dx
;0x1F1=0011 0000 写
;为了方便只关注第7位和第3位的状态，我们把其他位都变为0
;0x1F1=1011 0000 忙碌   1011 0000 and 1000 1000 = 1000 0000
;0x1F1=0011 1000 请就发送数据，或者接受数据 0011 1000 and 1000 1000 = 0000 1000

and al,0x88 ;0x88=0x10001000  两种可能性：1000 0000=忙碌  0000 1000=开始工作
cmp al,0x08
jnz Waits


;把要写的数据放入到数据端口中
mov ax,0x1111
mov dx,HD_PORT_DATA

mov cx,HD_SECTION_COUNT * 512 / 2
WriteLoop:
out dx,ax
loop WriteLoop


End:
jmp near End

times 510-($-$$) db 0x00
dw 0xAA55
