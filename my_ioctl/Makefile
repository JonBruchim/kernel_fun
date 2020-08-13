

obj-m += my_ioctl_drv.o
misc_drive-objs := my_ioctl_drv.o 

.PHONY: all clean

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

