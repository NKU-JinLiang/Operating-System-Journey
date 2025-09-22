%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ 0x9FC00
jmp loader_start	   ;下面存放数据段 构建gdt 跳跃到下面的代码区
GDT_BASE: dd 0x00000000 
	dd 0x00000000 
CODE_DESC: dd 0x0000FFFF 
	dd DESC_CODE_HIGH4 
DATA_STACK_DESC: dd 0x0000FFFF 
	dd DESC_DATA_HIGH4 
VIDEO_DESC: dd 0x80000007;limit=(0xbffff-0xb8000)/4k=0x7 
	dd DESC_VIDEO_HIGH4
GDT_SIZE equ $ - GDT_BASE 
GDT_LIMIT equ GDT_SIZE - 1 
times 60 dq 0 ; 此处预留 60 个描述符的空位
SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0 
SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0 ; 同上
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0 ; 同上
; total_mem_bytes 用于保存内存容量，以字节为单位
total_mem_bytes dd 0
;以下是 gdt 的指针，前 2 字节是 gdt 界限，后 4 字节是 gdt 起始地址
gdt_ptr dw GDT_LIMIT 
dd GDT_BASE
;人工对齐:total_mem_bytes4+gdt_ptr6+ards_buf244+ards_nr2，共 256 字节
ards_buf times 244 db 0 
ards_nr dw 0 ;用于记录 ARDS 结构体数量
loader_start:
;----------------- 获取内存容量 ----------------
xor ebx, ebx ;第一次调用时，ebx 值要为 0 
mov edx, 0x534d4150 ;edx 只赋值一次，循环体中不会改变
mov di, ards_buf ;ards 结构缓冲区
.e820_mem_get_loop: ;循环获取每个 ARDS 内存范围描述结构
	mov eax,0x0000e820 ;执行 int 0x15 后，eax 值变为 0x534d4150,所以每次执行 int 前都要更新为子功能号
	mov ecx,20 ;ARDS 地址范围描述符结构大小是 20 字节
	int 0x15 
	add di, cx ;使 di 增加 20 字节指向缓冲区中新的 ARDS 结构位置
	inc word [ards_nr] ;记录 ARDS 数量
	cmp ebx, 0 ;若 ebx 为 0 且 cf 不为 1，这说明 ards 全部返回,当前已是最后一个
	jnz .e820_mem_get_loop 
	;在所有 ards 结构中
 	;找出(base_add_low + length_low)的最大值，即内存的容量
	mov cx, [ards_nr] 
	;遍历每一个 ARDS 结构体,循环次数是 ARDS 的数量
	mov ebx, ards_buf 
	xor edx, edx ;edx 为最大的内存容量，在此先清 0 
.find_max_mem_area: 
	;无需判断 type 是否为 1,最大的内存块一定是可被使用的
	mov eax, [ebx] ;base_add_low 
	add eax, [ebx+8] ;length_low 
	add ebx, 20 ;指向缓冲区中下一个 ARDS 结构
	cmp edx, eax 
	;冒泡排序,找出最大,edx 寄存器始终是最大的内存容量
jge .next_ards 
	mov edx, eax ;edx 为总内存大小
	.next_ards:
	loop .find_max_mem_area 
	jmp .mem_get_ok
.mem_get_ok:
	mov [total_mem_bytes], edx
;----------------- 打开 A20 ----------------
in al,0x92
or al,0000_0010B
out 0x92,al
lgdt [gdt_ptr]

;----------------- cr0 第 0 位置 1 ---------------- 
mov eax, cr0 
or eax, 0x00000001 
mov cr0, eax 

jmp dword SELECTOR_CODE:p_mode_start ; 刷新流水线

[bits 32] 
p_mode_start: 
mov ax, SELECTOR_DATA 
mov ds, ax 
mov es, ax 
mov ss, ax 
mov esp,LOADER_STACK_TOP 
mov ax, SELECTOR_VIDEO 
mov gs, ax 

mov byte [gs:160], 'P' 

jmp $