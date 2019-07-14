TARGET_MODULE:=Andrej-module

# If we running by kernel building system
ifneq ($(KERNELRELEASE),)
	$(TARGET_MODULE)-objs := main.o device_file.o
	obj-m := $(TARGET_MODULE).o

# If we are running without kernel build system
else
	BUILDSYSTEM_DIR?=/lib/modules/$(shell uname -r)/build
	PWD:=$(shell pwd)


all : 
# run kernel build system to make module
	$(MAKE) -C /usr/src/linux-headers-4.19.50-v7+/  M=$(PWD) modules

clean:
# run kernel build system to cleanup in current directory
	$(MAKE) -C /usr/src/linux-headers-4.19.50-v7+/  M=$(PWD) clean

load:
# inserting module to the system
	sudo insmod ./$(TARGET_MODULE).ko

unload:
# removing module from the system
	sudo rmmod ./$(TARGET_MODULE).ko
mknod(number): 
	sudo mknod /dev/Andrej-driver c number 0
	sudo chmod 666 /dev/Andrej-driver


endif


