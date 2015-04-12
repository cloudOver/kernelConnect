obj-m += kernelConnect.o
kernelConnect-objs := processbuffer.o clouddev.o testthread.o syscall.o main.o proto/syscall.o syscalls/open.o

KERNEL_SOURCE=/root/linux-3.19.3/

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

rsync:
	rsync -av --delete . root@10.99.0.102:kernelConnect

mkpatch:
	patch -p1 $(KERNEL_SOURCE)/include/linux/sched.h < patch/linux/sched.patch
	patch -p1 $(KERNEL_SOURCE)/include/linux/syscalls.h < patch/linux/syscalls.patch
	#patch -p1 $(KERNEL_SOURCE)/include/linux/sched.h < patch/linux/sched.patch

unpatch:
	patch -R -p1 $(KERNEL_SOURCE)/include/linux/sched.h < patch/linux/sched.patch
	patch -R -p1 $(KERNEL_SOURCE)/include/linux/syscalls.h < patch/linux/syscalls.patch
	#patch -R -p1 $(KERNEL_SOURCE)/include/linux/syscalls.h < patch/linux/sched.patch
