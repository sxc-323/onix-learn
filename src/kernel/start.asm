[bits 32]

extern kernel_init ; 调用外部函数

global _start
_start:
    xchg bx,bx
    call kernel_init
    ; mov byte [0xb8000],'k'
    jmp $