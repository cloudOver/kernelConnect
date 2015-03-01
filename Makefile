obj-m += kernelConnect.o
kernelConnect-objs := processbuffer.o clouddev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
