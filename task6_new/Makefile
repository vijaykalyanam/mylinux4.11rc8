obj-m := task_misc.o
task_misc-objs := task6.o
//task_misc-objs := open.o
//task_misc-objs := semaphore.o
//task_misc-objs := mutex.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
