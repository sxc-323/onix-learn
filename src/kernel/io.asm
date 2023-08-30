[bits 32]

section .text

global inb ; 将 inb 
inb:
    push ebp
    mov ebp,esp

    xor eax,eax
    mov edx,[ebp+8]
    in al,dx

    jmp $+2
    jmp $+2
    jmp $+2

    leave
    ret

global inw ; 将 inw 
inw:
    push ebp
    mov ebp,esp

    xor eax,eax
    mov edx,[ebp+8]
    in ax,dx

    jmp $+2
    jmp $+2
    jmp $+2

    leave
    ret

global outb; 将 outb 导出
outb:
    push ebp
    mov ebp,esp
    mov eax,[ebp+12]
    mov edx,[ebp+8]
    out dx,al

    jmp $+2
    jmp $+2
    jmp $+2

    leave
    ret


global outw ; 将 outw 导出
outw:
    push ebp
    mov ebp,esp

    mov eax,[ebp+12]
    mov edx,[ebp+8]
    out dx,ax

    jmp $+2
    jmp $+2
    jmp $+2
    
    leave
    ret

