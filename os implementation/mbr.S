%include "boot.inc" 
[BITS 16]
[ORG 0x7C00]
	mov ax,cs 
	mov ds,ax 
	mov es,ax 
	mov ss,ax 
	mov fs,ax 
	mov sp,0x7c00 
	mov ax,0xb800 
	mov gs,ax
	mov ax, 0600h 
	mov bx, 0700h 
	mov cx, 0 ; 左上角: (0, 0) 
	mov dx, 184fh ; 右下角: (80,25), 
	int 10h
	mov byte [gs:0x00],'1' 
	mov byte [gs:0x01],0xA4 ; A 表示绿色背景闪烁，4 表示前景色为红色
	mov byte [gs:0x02],' ' 
	mov byte [gs:0x03],0xA4 
	mov byte [gs:0x04],'M' 
	mov byte [gs:0x05],0xA4 
	mov byte [gs:0x06],'B' 
	mov byte [gs:0x07],0xA4 
	mov byte [gs:0x08],'R' 
	mov byte [gs:0x09],0xA4 
	; --- 在调用 rd_disk_m_16 之前加 ---
	xor ax, ax
	mov ds, ax
	mov es, ax    ; 以防后面要用 ES:DI 的形式
	; -------------------------------
	mov eax, LOADER_START_SECTOR
	mov bx,  LOADER_BASE_ADDR   ; 0x600
	mov cx,  4
	call rd_disk_m_16
	jmp  LOADER_BASE_ADDR       ; 跳到 0x0000:0x0600

rd_disk_m_16:
	mov esi,eax ;备份 eax 
	mov di,cx ;备份 cx 
	mov dx,0x1f2 
	mov al,cl 
	out dx,al ;读取的扇区数
	mov eax,esi ;恢复 ax  

	mov dx,0x1f3 
	out dx,al 
	mov cl,8 
	shr eax,cl 
	mov dx,0x1f4 
	out dx,al 

	shr eax,cl 
	mov dx,0x1f5 
	out dx,al 
	shr eax,cl 
	and al,0x0f ;lba 第 24～27 位
	or al,0xe0 ; 设置 7～4 位为 1110,表示 lba 模式
 	mov dx,0x1f6 
	out dx,al
	mov dx,0x1f7 
	mov al,0x20 
	out dx,al 
	
.not_ready: 
	nop 
	in al,dx 
	and al,0x88 ;第 4 位为 1 表示硬盘控制器已准备好数据传输
 ;第 7 位为 1 表示硬盘忙
	cmp al,0x08 
	jnz .not_ready ;若未准备好,继续等

	 ;第 5 步:从 0x1f0 端口读数据
	mov ax, di 
	mov dx, 256 
	mul dx 
	mov cx, ax 
; di 为要读取的扇区数,一个扇区有 512 字节,每次读入一个字
; 共需 di*512/2 次,所以 di*256 
	mov dx, 0x1f0 
.go_on_read: 
	in ax,dx 
	mov [bx],ax 
	add bx,2 
	loop .go_on_read 
	ret 
	times 510-($-$$) db 0 
	db 0x55,0xaa