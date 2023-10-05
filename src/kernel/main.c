#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/printk.h>

void kernel_init()
{
    console_init();
    int cnt=50;
    while (cnt--)
    {
        printk("hello onix %#010x,%d\n",cnt,cnt);
    }
    
    return;
}