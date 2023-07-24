section Header vstart=0
Program_Length  dd Program_End ;程序的大小，占4个字节。
Code_Offset  dw 0x0000  ;要执行的第一条指令的偏移地址
Code_Entry  dd section.Code_Init.start;要执行第一条指令的段地址。

;段个数
Section_Length  dw (Header_End-Code_Init_Section)/4

;各个段的段地址，重定位表。

Code_Init_Section dd section.Code_Init.start
Code_Main_Section dd section.Code_Main.start
Code_End_Section dd section.Code_End.start
Code_Int70_Section dd section.Code_Int70.start
Data_Section  dd section.Data.start
Stack_Section  dd section.Stack.start


Header_End:

;主程序代码区
Section Code_Main align=16 vstart=0
cli;  把IF标志位=0    CPU就屏蔽么INTR中断屏蔽了
mov bx,0x70  ; 0x70是RTC的中断信号。 
shl bx,2;  中断号是0x70，乘以4算出了IP的偏移地址。
mov ax,0x0000
mov ds,ax

mov word[ds:bx],0x0000
mov ax,word[es:Code_Int70_Section]
mov word[ds:bx+2],ax   ;把中断处理程序的偏移地址和段地址写到中断向量表里面。

;接下来要设置RTC芯片的工作状态，告诉RTC你可以产生中断信号给8259中断控制器。
;RTC到8259芯片的线路只有一根，而RTC芯片可以产生多种中断。 比如闹钟中断，更新结束中断和周期性中断。
;我们为RTC要设置成更新周期结束中断。  每隔一秒钟，RTC芯片会修改一下CMOS中的时间，就发出一次中断。


mov al,0x0B
out 0x70,al

mov al,0x12 ;0001 0010
out 0x71,al  ;把cmos中寄存器B的值修改了。   开启更新结束中断，以BDC模式编码存储，24小时格式。


in al,0xa1
and al,0xFE ; 1111 1110  ;把al的最后一位，也就是对应的从片的IRO改成0，IMR阻断关闭了，RTC芯片的从片IRO就可以正常接收中断信号了。
out 0xA1,al

 

sti ;  把IF标志位=1  CPU开始受理INTR发来中断信号了，也就是8259芯片中的所有中断信号。

@1:
hlt
jmp near @1


;以下代码不能删除，主程序工作完成以后，执行收尾工作。
push word[es:Code_End_Section]
push 0x0000
retf

Section Code_Int70 align=16 vstart=0  ;中断号0x70对应的处理程序的入口地址
push ax
push bx
push cx
push dx
push es

mov al,0x00
out 0x70,al  ;定位到CMOS中的0x00
in al,0x71  ;读RTC当前时间（秒）
push ax

mov al,0x02
out 0x70,al  ;定位到CMOS中的0x02
in al,0x71  ;读RTC当前时间（分）
push ax

mov al,0x04
out 0x70,al  ;定位到CMOS中的0x02
in al,0x71  ;读RTC当前时间（时）
push ax

;秒，分，时，都压到栈中了。

mov al,0x0C
out 0x70,al
in al,0x71   ;读一下RTC芯片的寄存器C，否则只会发生一次中断。

mov ax,0xB800
mov es,ax

pop ax
call Bcd_To_Ascii
mov byte[es:0x0000],ah
mov byte[es:0x0001],0x07

mov byte[es:0x0002],al
mov byte[es:0x0003],0x07


mov byte[es:0x0004],':'
mov byte[es:0x0005],0x07


pop ax
call Bcd_To_Ascii
mov byte[es:0x0006],ah
mov byte[es:0x0007],0x07

mov byte[es:0x0008],al
mov byte[es:0x0009],0x07


mov byte[es:0x000A],':'
mov byte[es:0x000B],0x07

pop ax
call Bcd_To_Ascii
mov byte[es:0x000C],ah
mov byte[es:0x000D],0x07

mov byte[es:0x000E],al
mov byte[es:0x000F],0x07


mov al,0x20; 0x20代表我把中断处理完了。
out 0xa0,al; 告诉8259从片，我把这个中断处理完了。
out 0x20,al; 告诉8259主片，我把这个中断处理完了。

;因为bochs2.6.8有BUG，这是原作者设计的BUG，我无能为力。


push es
push dx
push cx
push bx
push ax

iret

 

Bcd_To_Ascii:  ;BCD转asc编码
;参数肯定是ax
mov ah,al
and al,0x0F
add al,0x30  ;转换成数字8对应的ascii编码了。

shr ah,4
add ah,0x30
ret 

Section Data align=16 vstart=0
Stack_Save:
dw 0x0000,0x0000,0x0000  ; 保存加载器的：ss sp bp


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

 

 


Section Stack align=16 vstart=0
times 256 db 0x00
Stack_End:

 

 

section End
Program_End:
