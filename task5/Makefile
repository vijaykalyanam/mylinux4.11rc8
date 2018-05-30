obj-m := usbcall2.o
#usbcall-objs := task5_submission1.o
usbcall2-objs := task5.o

ifeq ($(KERNEL_SRC_PATH),)
KERNEL_SRC_PATH = /lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) modules

install:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) modules_install

clean:
	make -C $(KERNEL_SRC_PATH) M=$(PWD) clean
