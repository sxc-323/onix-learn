[bits 32]

exit:
    ret

global main
main:
    ; push 5
    ; push eax

    ; pop ebx
    ; pop ecx

    ; pusha
    ; popa
    
    push 0 ;传递参数
    call exit
    mov eax,5