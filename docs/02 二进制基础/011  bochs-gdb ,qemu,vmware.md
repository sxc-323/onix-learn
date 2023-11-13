# bochs-gdb
bochs-gdb 使用c语言调试内核
bochs 使用汇编语言调试内核

```makefile
.PHONY:bochsg
bochsg:$(BUILD)/master.img
	bochs-gdb -q -f ../bochs/bochsrc.gdb

.PHONY:bochs
bochs:$(BUILD)/master.img
	bochs -q -f ../bochs/bochsrc
```
# qemu
```makefile
.PHONY:qemu
qemu:$(BUILD)/master.img
	qemu-system-i386 \
	-m 32M \
	-boot c \
	-hda $<

.PHONY:qemug
qemug:$(BUILD)/master.img
	qemu-system-i386 \
	-s -S \	   #qemug 调试 
	-m 32M \
	-boot c \
	-hda $<
```
# vmware
```makefile
$(BUILD)/master.vmdk:$(BUILD)/master.img
	qemu-img convert -O vmdk $< $@

.PHONY:vmdk
vmdk:$(BUILD)/master.vmdk
```