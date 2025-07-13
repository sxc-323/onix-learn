#include <onix/global.h>
#include <onix/string.h>
#include <onix/debug.h>

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表
pointer_t gdt_ptr;          // 内核全局描述符表指针

void descriptor_init(descriptor_t *desc, u32 base, u32 limit)
{
    desc->base_low = base & 0xffffff;
    desc->base_high = (base >> 24) & 0xff;
    desc->limit_low = limit & 0xffff;
    desc->limit_high = (limit >> 16) & 0xf;
}

// 初始化内核全局描述符表
void gdt_init()
{
    DEBUGK("init gdt!!!\n");

    memset(gdt,0,sizeof(gdt));

    descriptor_t *desc;
    desc =gdt +KERNEL_CODE_IDX;
    descriptor_init(desc,0,0xfffff);
    desc->segment = 1;
    desc->granularity = 1;
    desc->big = 1;
    desc->long_mode = 0;
    desc->present = 1;
    desc->DPL=0;
    desc->type = 0b1010;

    desc=gdt +KERNEL_DATA_IDX;
    descriptor_init(desc,0,0xfffff);
    desc->segment = 1;
    desc->granularity = 1;
    desc->big = 1;
    desc->long_mode = 0;
    desc->present = 1;
    desc->DPL=0;
    desc->type = 0b0010;

    gdt_ptr.base=(u32)&gdt;
    gdt_ptr.limit=sizeof(gdt)-1;
}