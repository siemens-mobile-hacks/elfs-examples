PROJECT = hello-world

OPT := -Os
LDLIBS += -lcrt -lcrt_helper -lgcc
CFLAGS += -D__NO_LIBC

SOURCES += main.c

SDK_PATH ?= ../sdk
include $(SDK_PATH)/multi-target.mk
