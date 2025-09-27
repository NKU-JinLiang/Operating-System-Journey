;------------------- 进入loader所需要的宏 --------------------------
LOADER_BASE_ADDR equ 0x7E00
LOADER_START_SECTOR equ 0x2

;-------------------- gdt描述符属性 --------------------------------

DESC_G_4K equ (1 << 23)  ; 第 23 位 = 1
DESC_D_32 equ (1 << 22)  ; 段描述符的D/B 字段 22 位 = 1
DESC_L equ 0  ; 段描述符的L字段,64位代码标记,21位，标记为0
DESC_AVL equ 0  ; AVL字段,20位，标记为0
DESC_LIMIT_CODE equ 1111_0000000000000000b ;各个段的段界限，16~19位
DESC_LIMIT_DATA equ DESC_LIMIT_CODE  ;各个段的段界限，16~19位
DESC_LIMIT_VIDEO equ 0 ;各个段的段界限，16~19位
DESC_P equ (1 << 15)  ; P字段，15位
DESC_DPL_0 equ 00_0_0000_0000_0000b   ; DPL字段，13~14位
DESC_DPL_1 equ 01_0_0000_0000_0000b   ; DPL字段，13~14位
DESC_DPL_2 equ 10_0_0000_0000_0000b   ; DPL字段，13~14位
DESC_DPL_3 equ 11_0_0000_0000_0000b   ; DPL字段，13~14位
DESC_S_CODE equ 1_0000_0000_0000b ; S字段，12位
DESC_S_DATA equ DESC_S_CODE 
DESC_S_sys equ 0_0000_0000_0000b 
DESC_TYPE_CODE equ 1000_0000_0000b ; type字段，8~11位 
;x=1,c=0,r=0,a=0 代码段是可执行的，非一致性，不可读，已访问位 a 清 0 
DESC_TYPE_DATA equ 0010_0000_0000b 
;x=0,e=0,w=1,a=0 数据段是不可执行的，向上扩展的，可写，已访问位 a 清 0 
DESC_CODE_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_CODE + DESC_P+DESC_DPL_0 + DESC_S_CODE +DESC_TYPE_CODE + 0x00

DESC_DATA_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 +DESC_L + DESC_AVL + DESC_LIMIT_DATA + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00 

DESC_VIDEO_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 +DESC_L + DESC_AVL + DESC_LIMIT_VIDEO + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x0B 
;-------------- 选择子属性 --------------- 
RPL0 equ 00b 
RPL1 equ 01b 
RPL2 equ 10b 
RPL3 equ 11b 
TI_GDT equ 000b 
TI_LDT equ 100b