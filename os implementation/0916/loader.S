%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
jmp loader_start	   ;下面存放数据段 构建gdt 跳跃到下面的代码区
GDT_BASE: dd 0x00000000 
	dd 0x00000000 
CODE_DESC: dd 0x0000FFFF 
	dd DESC_CODE_HIGH4 
DATA_STACK_DESC: dd 0x0000FFFF 
	dd DESC_DATA_HIGH4 
VIDEO_DESC: dd 0x80000007;limit=(0xbffff-0xb8000)/4k=0x7 
	dd DESC_VIDEO_HIGH4 ;此时 dpl 为 0 
GDT_SIZE equ $ - GDT_BASE 
GDT_LIMIT equ GDT_SIZE - 1 
times 70 dq 0 ; 此处预留 50 个描述符的空位
SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0 
SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0 ; 同上
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0 ; 同上
;以下是 gdt 的指针，前 2 字节是 gdt 界限，后 4 字节是 gdt 起始地址
gdt_ptr dw GDT_LIMIT 
dd GDT_BASE
loadermsg db   '2 loader in real.'                  ;loadermsg 加载区显示字符
loader_start:
;------------------------------------------------------------ 
;INT 0x10 功能号:0x13 功能描述:打印字符串
;------------------------------------------------------------    
mov sp, LOADER_BASE_ADDR 
mov bp, loadermsg ; ES:BP = 字符串地址
mov cx, 17 ; CX = 字符串长度
mov ax, 0x1301 ; AH = 13, AL = 01h 
mov bx, 0x001f ; 页号为 0(BH = 0) 蓝底粉红字(BL = 1fh) 
mov dx, 0x1800 
int 0x10 ; 10h 号中断