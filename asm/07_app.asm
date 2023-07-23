section Header vstart=0
Program_Length  dd Program_End ;程序的大小，占4个字节。
Code_Offset  dw 0x0000  ;要执行的第一条指令的偏移地址
Code_Entry  dd section.Code_Init.start;要执行第一条指令的段地址。

;段个数
Section_Length  dw (Header_End-Code_Init_Section)/4

;各个段的段地址，重定位表。

Code_Function_Section dd section.Code_Function.start
Code_Init_Section dd section.Code_Init.start
Code_Main_Section dd section.Code_Main.start
Code_End_Section dd section.Code_End.start
Data_Section  dd section.Data.start
Stack_Section  dd section.Stack.start
Header_End:

;主程序代码区
Section Code_Main align=16 vstart=0
;string的参数
;displayIndex的参数
;call

mov ax,0xB800
mov ds,ax
mov di,0
mov cx,2000

cls:
mov word[ds:di],0x0720
add di,2
loop cls   ;Clean Screen


mov ds,word[es:Data_Section]
push ds
push String
push ds
push DisPlayIndex

mov ax,word[es:Code_Function_Section]
mov word[ds:Destination_Address],PrintfString
mov word[ds:Destination_Address+2],ax

call far [ds:Destination_Address]
;以下代码不能删除，主程序工作完成以后，执行收尾工作。
push word[es:Code_End_Section]
push 0x0000
retf

 

Section Data align=16 vstart=0
Stack_Save:
dw 0x0000,0x0000,0x0000  ; 保存加载器的：ss sp bp

String:
db '1',0x0A
db '11',0x0A
db '111',0x0A
db '1111',0x0A
db '11111',0x0A
db '111111',0x0A
db '1111111',0x0A
db '11111111',0x0A
db '111111111',0x0A
db '1111111111',0x0A
db '11111111111',0x0A
db '111111111111',0x0A
db '1111111111111',0x0A
db '11111111111111',0x0A
db '111111111111111',0x0A
db '1111111111111111',0x0A
db '11111111111111111',0x0A
db '111111111111111111',0x0A
db '1111111111111111111',0x0A
db '11111111111111111111',0x0A
db '111111111111111111111',0x0A
db '1111111111111111111111',0x0A
db '11111111111111111111111',0x0A
db '111111111111111111111111',0x0A
db '1111111111111111111111111',0x00

DisPlayIndex:
dw 0x0000

Destination_Address:
dw 0x0000,0x0000   ;偏移地址，段地址

 


Section Code_Init align=16 vstart=0 ;
;ds和es段寄存器指向Header段的段地址的，也就是0x1000。
push ax
push bx
push cx
push dx
push ds
push es
push si
push di
;ss sp bp


;先把加载器的ss和sp和bp保存好。
mov ax,word[es:Data_Section]
mov ds,ax
mov word[ds:Stack_Save],ss
mov word[ds:Stack_Save+2],sp
mov word[ds:Stack_Save+4],bp

;设置被加载程序的栈内存。
mov ax,word[es:Stack_Section]
mov ss,ax;  设置好了栈段地址
mov sp,Stack_End
mov bp,Stack_End

push word[es:Code_Main_Section]
push 0x0000
retf

 

 

Section Code_End align=16 vstart=0
mov ax,word[es:Data_Section]
mov ds,ax
mov ss,word[ds:Stack_Save]
mov sp,word[ds:Stack_Save+2]
mov bp,word[ds:Stack_Save+4]

pop di ;被加载的程序执行完毕以后，恢复现场。
pop si
pop es
pop ds
pop dx
pop cx
pop bx
pop ax
retf


Section Code_Function align=16 vstart=0
PrintfString:
;接口使用说明如下：

;push 0x07C0;  压入屏幕输出文本所在段寄存器
;push String;  压入屏幕输出文本所在偏移寄存器
;push 0x07C0;  压入DisPlayIndex所在的段寄存器。
;push DisPlayIndex;压入DisPlayIndex所在的偏移寄存器。
;call 0x07C0:PrintfStr  ;调用输出接口
;add sp,8   ;清空栈内垃圾数据

push ds  ;保存ds段寄存器的值
push es  ;保存es段寄存器的值
push ax  ;保存ax寄存器的值
push di  ;保存目标地址偏移寄存器
push si  ;保存原始地址偏移寄存器

mov ds,word[ss:bp-6] ;初始化DisPlayIndex的段寄存器
mov si,word[ss:bp-8] ;初始化DI寄存器，DI寄存器内部存储着屏幕上的输出位置。
mov di,word[ds:si]

mov ds,word[ss:bp-2] ;初始化要输出的文本数据段寄存器
mov si,word[ss:bp-4] ;初始化要输出的文本数据偏移寄存器

mov ax,0xB800
mov es,ax  ;初始化显示段寄存器


PrintfLoop:
mov al,byte [ds:si]
inc si
cmp al,0
jz PrintfEnd

;检测al中的值是不是0x0A，如果是0x0A，就回车换行。
;如果不是0x0A，就正常输出。

cmp al,0x0A
jnz StanderPrint
call NewLine
jmp near @1


StanderPrint:
mov byte[es:di],al
inc di
mov byte[es:di],0x31
inc di


@1:
call TestCursor
jmp near PrintfLoop

PrintfEnd:
mov ds,word[ss:bp-6]
mov si,word[ss:bp-8]
mov word[ds:si],di
pop si
pop di
pop ax
pop es
pop ds
retf


TestCursor:
push dx
push ax
push bx
push cx
push si
push ds

mov ax,di
shr ax,1;  把di 除以2，转换成了光标位置。

cmp ax,1999
jna SetCursor   ;当小于等于1999的时候，设置光标位置。
;开始滚动屏幕
mov bx,0xB800
mov ds,bx

mov di,0x0000
mov si,160
cld  ;把df=0   执行movsw指令后，di和si的值自动会加2
mov cx,1920
rep movsw     ;mov word[es:di],word[ds:si]

mov cx,80
mov di,3840

Cls_Last_Line: ;把最后一行进行清空
mov word[ds:di],0x0720
add di,2
loop  Cls_Last_Line

mov di,3840
mov ax,1920

SetCursor:
mov cx,ax
mov dx,0x3D4
mov al,0x0E
out dx,al

mov dx,0x3D5
mov al,ch ;光标位置的高8位
out dx,al

mov dx,0x3D4
mov al,0x0F
out dx,al

mov dx,0x3D5
mov al,cl ;光标位置的低8位
out dx,al

 

pop ds
pop si
pop cx
pop bx
pop ax
pop dx
ret

 


NewLine:
;DI里面存储了要显示输出的位置。

push dx
push ax
push bx

shr di,1   ;把di的值除以2
mov ax,di
mov dx,0
mov bx,80
div bx;  dx:ax的值 除以 80 。 商就是所在行的行号(ax)，余数是所在列的列号。(dx)。
inc ax ; 切换到下一行了，行号最大肯定不会超过25。
mul bl ; 用al里面的值乘以bl里面的值，结果放到ax中。

shl ax,1 ; 把ax * 2 再放到ax里面。 现在ax的值就是经过回车换行之后的偏移地址
mov di,ax

pop bx
pop ax
pop dx
ret


Section Stack align=16 vstart=0
times 256 db 0x00
Stack_End:

 

 

section End
Program_End:
