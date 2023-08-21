## 实模式 print 代码如下：

```s
mov si,booting
call print

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
```

- ah为0x0e,al为字符地址
- 仍然是 int 0x10
- 调用循环 判断字符是否为0,如果为0,就跳出循环
- ASCII码 10,13分别代表 换行和回车 \n和\r