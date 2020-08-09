ifneq ($(KERNELRELEASE),)
	obj-m:=scull.o
else
	KERNELDIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -f *.mod.c *.mod.o *.ko *.o *.tmp_versions
.PHONY:modules clean