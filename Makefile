obj-m += kernelConnect.o
kernelConnect-objs := processbuffer.o clouddev.o testthread.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

rsync:
	rsync -av --delete . root@10.99.0.101:kernelConnect
