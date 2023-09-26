DEVICE ?= NEWSGOLD
BT_ADDR ?= 00:0D:41:14:65:C3

PROJECT = hello-world-$(DEVICE).elf
BUILD_DIR = bin

LDLIBS += -lcrt -lcrt_helper -lgcc -lc -lcrtfix
CFILES += main.c
# conf_loader.c config_data.c

SIEDEV_DIR=../../sie-dev2
include $(SIEDEV_DIR)/rules.mk

install:
	bluetooth-sendto --device=$(BT_ADDR) hello-world-NEWSGOLD.elf
