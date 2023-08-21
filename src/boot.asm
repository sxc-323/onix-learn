[org 0x7c00]

; 设置屏幕模式为文本模式，清除屏幕
mov ax,3
int 0x10

; 初始化段寄存器
mov ax,0
mov ds,ax
mov es,ax
mov ss,ax
mov sp,0x7c00




mov si,booting
call print

xchg bx,bx;

mov edi,0x1000; 读取的目标内存
mov ecx,0; 起始扇区
mov bl,1; 扇区数量

call read_disk

mov edi,0x1000
mov ecx,1
mov bl,1
;call write_disk

jmp $

read_disk:
    mov dx, 0x1f2
    mov al,bl
    out dx,al

    inc dx  ;0x1f3
    mov al,cl
    out dx,al

    inc dx  ;0x1f4
    shr ecx,8
    mov al,cl
    out dx,al

    inc dx  ;0x1f5
    shr ecx,8
    mov al,cl
    out dx,al

    inc dx  ;0x1f6
    shr ecx,8
    and cl,0b1111;将高四位设为0

    or cl,0b1110_0000
    mov al,cl
    out dx,al;主盘 LBA模式

    inc dx  
    mov al,0x20
    out dx,al

    xor ecx,ecx
    mov cl,bl; 得到读取扇区的数量

    .read:
        push cx
        call .waits
        call .reads
        pop cx
        loop .read
    ret

    .waits:
        mov dx,0x1f7
        .check:
            in al,dx
            jmp $+2
            jmp $+2
            jmp $+2
            and al,0b1000_1000
            cmp al,0b0000_1000
            jnz .check
        ret
    
    .reads
        mov dx,0x1f0
        mov cx,256
        .readw:
            in ax,dx
            jmp $+2
            jmp $+2
            jmp $+2
            mov [edi],ax
            add edi,2
            loop .readw
        ret

write_disk:
    mov dx, 0x1f2
    mov al,bl
    out dx,al

    inc dx  ;0x1f3
    mov al,cl
    out dx,al

    inc dx  ;0x1f4
    shr ecx,8
    mov al,cl
    out dx,al

    inc dx  ;0x1f5
    shr ecx,8
    mov al,cl
    out dx,al

    inc dx  ;0x1f6
    shr ecx,8
    and cl,0b1111;将高四位设为0

    or cl,0b1110_0000
    mov al,cl
    out dx,al;主盘 LBA模式

    inc dx  
    mov al,0x30
    out dx,al

    xor ecx,ecx
    mov cl,bl; 得到读取扇区的数量

    .write:
        push cx
        call .writes
        call .waits
        pop cx
        loop .write
    ret

    .waits:
        mov dx,0x1f7
        .check:
            in al,dx
            jmp $+2
            jmp $+2
            jmp $+2
            and al,0b1000_0000
            cmp al,0b0000_0000
            jnz .check
        ret
    
    .writes
        mov dx,0x1f0
        mov cx,256
        .writew:
            mov ax,[edi]
            out dx,ax
            jmp $+2
            jmp $+2
            jmp $+2
            add edi,2
            loop .writew
        ret

print:
    mov ah,0x0e
.next:
    mov al,[si]
    cmp al,0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

booting:
    db "Booting Onix...",10,13,0 ;\n\r

times 510-($-$$) db 0

db 0x55,0xaa  ;dw 0xaa55