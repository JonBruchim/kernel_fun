

obj-m += misc_drive.o
misc_drive-y := my_ioctl_drv.o

KDIR ?= /lib/modules/`uname -r`/build

.PHONY: all clean

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

