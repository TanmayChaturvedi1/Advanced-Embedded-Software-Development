# Makefile by Tanmay Chaturvedi
# Cross compiles a kernel module for beaglebone 


LINUX_DIR := $(HOME)/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220
COMPILE_DIR := $(HOME)/buildroot/output/host/bin/


obj-m += linkedlist_kernel_module.o

all:
	make ARCH=arm CROSS_COMPILE=$(COMPILE_DIR)arm-buildroot-linux-uclibcgnueabihf- -C $(LINUX_DIR) M=$(shell pwd) modules
clean:
	make -C $(LINUX_DIR) M=$(shell pwd) clean
