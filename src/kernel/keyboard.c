#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/assert.h>
#include <onix/debug.h>

#define LOGK(fmt,args...) DEBUGK(fmt,##args)

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CTRL_PORT 0x64

void KEYBOARD_handler(int vector)
{ 
    assert(vector == 0x21);
    send_eoi(vector);                       // 发送中断结束信号
    u16 scancode = inb(KEYBOARD_DATA_PORT); // 读取键盘数据
    LOGK("keyboard input 0x%x\n",scancode);
}

void keyboard_init()
{
    set_interrupt_handler(IRQ_KEYBOARD,KEYBOARD_handler);
    set_interrupt_mask(IRQ_KEYBOARD,true);
}
