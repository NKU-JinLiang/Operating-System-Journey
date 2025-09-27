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
	dd DESC_VIDEO_HIGH4 ;此时 dpl 为 0 
GDT_SIZE equ $ - GDT_BASE 
GDT_LIMIT equ GDT_SIZE - 1 
times 60 dq 0 ; 此处预留 60 个描述符的空位
SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0 
SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0 ; 同上
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0 ; 同上
;以下是 gdt 的指针，前 2 字节是 gdt 界限，后 4 字节是 gdt 起始地址
total_mem_bytes dd 0
gdt_ptr dw GDT_LIMIT 
    dd GDT_BASE

ards_buf times 244 db 0
ards_nr dw 0 ;用于记录 ARDS 结构体数量

loader_start:
xor ebx, ebx ;第一次调用时，ebx 值要为 0 
mov edx, 0x534d4150 ;edx 只赋值一次，循环体中不会改变
mov di, ards_buf ;ards 结构缓冲区
.e820_mem_get_loop: ;循环获取每个 ARDS 内存范围描述结构
    mov eax,0x0000e820 ;执行 int 0x15 后，eax 值变为 0x534d4150，
    ;所以每次执行 int 前都要更新为子功能号
    mov ecx,20 ;ARDS 地址范围描述符结构大小是 20 字节
    int 0x15
    ;若 cf 位为 1 则有错误发生，尝试 0xe801 子功能
    add di, cx ;使 di 增加 20 字节指向缓冲区中新的 ARDS 结构位置
    inc word [ards_nr] ;记录 ARDS 数量
    cmp ebx, 0 ;若 ebx 为 0 且 cf 不为 1，这说明 ards 全部返回
    ; 当前已是最后一个
    jnz .e820_mem_get_loop
    ;在所有 ards 结构中
    ;找出(base_add_low + length_low)的最大值，即内存的容量
    mov cx, [ards_nr]        ; ARDS 数量
    mov ebx, ards_buf        ; EBX 指向第一个 ARDS 结构
    xor edx, edx             ; EDX = 总内存大小，初始化为 0

.sum_mem_loop:
    mov eax, [ebx+16]        ; 取 Type 字段 (DWORD)
    cmp eax, 1               ; 是否等于 1 (可用内存)
    jne .skip_this           ; 不是可用内存则跳过

    mov eax, [ebx+8]         ; length_low
    add edx, eax             ; 累加到总内存大小

.skip_this:
    add ebx, 20              ; EBX 指向下一个 ARDS
    loop .sum_mem_loop

    mov [total_mem_bytes], edx
    jmp .mem_get_ok

.mem_get_ok:
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
; 创建页目录及页表并初始化页内存位图
call setup_page 

    ;要将描述符表地址及偏移量写入内存 gdt_ptr，一会儿用新地址重新加载
    sgdt [gdt_ptr] ; 存储到原来 gdt 所有的位置

    ;将 gdt 描述符中视频段描述符中的段基址+0xc0000000 
    mov ebx, [gdt_ptr + 2] 
    or dword [ebx + 0x18 + 4], 0xc0000000 
    ;视频段是第 3 个段描述符，每个描述符是 8 字节，故 0x18 
    ;段描述符的高 4 字节的最高位是段基址的第 31～24 位

    ;将 gdt 的基址加上 0xc0000000 使其成为内核所在的高地址
    add dword [gdt_ptr + 2], 0xc0000000 

    add esp, 0xc0000000 ; 将栈指针同样映射到内核地址

    ; 把页目录地址赋给 cr3 
    mov eax, PAGE_DIR_TABLE_POS 
    mov cr3, eax 

    ; 打开 cr0 的 pg 位（第 31 位）
    mov eax, cr0 
    or eax, 0x80000000 
    mov cr0, eax 

    ;在开启分页后，用 gdt 新的地址重新加载
    lgdt [gdt_ptr] ; 重新加载

    mov byte [gs:160], 'V' 
;视频段段基址已经被更新，用字符 v 表示 virtual addr 

    jmp $
;------------- 创建页目录及页表 --------------- 
setup_page: 
    ;先把页目录占用的空间逐字节清 0 
    mov ecx, 4096 
    mov esi, 0 
.clear_page_dir: 
    mov byte [PAGE_DIR_TABLE_POS + esi], 0 
    inc esi 
    loop .clear_page_dir
;开始创建页目录项(PDE) 
.create_pde: ; 创建 Page Directory Entry 
    mov eax, PAGE_DIR_TABLE_POS 
    add eax, 0x1000 ; 此时 eax 为第一个页表的位置及属性
    mov ebx, eax ; 此处为 ebx 赋值，是为.create_pte 做准备，ebx 为基址
; 下面将页目录项 0 和 0xc00 都存为第一个页表的地址，每个页表表示 4MB 内存
; 这样 0xc03fffff 以下的地址和 0x003fffff 以下的地址都指向相同的页表
; 这是为将地址映射为内核地址做准备
    or eax, PG_US_U | PG_RW_W | PG_P 
; 页目录项的属性 RW 和 P 位为 1，US 为 1，表示用户属性，所有特权级别都可以访问
    mov [PAGE_DIR_TABLE_POS + 0x0], eax ; 第 1 个目录项
;在页目录表中的第 1 个目录项写入第一个页表的位置(0x101000)及属性(7) 
    mov [PAGE_DIR_TABLE_POS + 0xc00], eax 
; 一个页表项占用 4 字节
; 0xc00 表示第 768 个页表占用的目录项，0xc00 以上的目录项用于内核空间
;也就是页表的 0xc0000000～0xffffffff 共计 1G 属于内核
; 0x0～0xbfffffff 共计 3G 属于用户进程
    sub eax, 0x1000 
    mov [PAGE_DIR_TABLE_POS + 4092], eax ; 使最后一个目录项指向页目录表自己的地址
;下面创建页表项(PTE) 
    mov ecx, 256 ; 1M 低端内存 / 每页大小 4k = 256 
    mov esi, 0 
    mov edx, PG_US_U | PG_RW_W | PG_P ; 属性为 7，US=1，RW=1，P=1 
.create_pte: ; 创建 Page Table Entry 
    mov [ebx+esi*4],edx ; 此时的 ebx 已经在上面通过 eax 赋值为 0x101000，也就是第一个页表的地址
    add edx,4096 
    inc esi 
    loop .create_pte
;创建内核其他页表的 PDE 
    mov eax, PAGE_DIR_TABLE_POS 
    add eax, 0x2000 ; 此时 eax 为第二个页表的位置
    or eax, PG_US_U | PG_RW_W | PG_P ; 页目录项的属性 US､ RW 和 P 位都为 1 
    mov ebx, PAGE_DIR_TABLE_POS 
    mov ecx, 254 ; 范围为第 769～1022 的所有目录项数量
    mov esi, 769 
.create_kernel_pde: 
    mov [ebx+esi*4], eax 
    inc esi 
    add eax, 0x1000 
    loop .create_kernel_pde 
    ret