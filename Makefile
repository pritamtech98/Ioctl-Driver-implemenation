obj-m := ioctl_drv.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

modules:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean