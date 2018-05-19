obj-m := panic_hung.o

KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
VERBOSE = 0

default:
	$(MAKE) -C $(KDIR) M=$(PWD) KBUILD_VERBOSE=$(VERBOSE) modules

clean:
	rm -f *.ko *.o *.lo *.mod.c Module.symvers modules.order 

