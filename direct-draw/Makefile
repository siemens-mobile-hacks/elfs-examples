PROJECT = hello-world

TARGETS := ELKA NSG

OPT := -Os
LDLIBS += -lcrt -lcrt_helper -lgcc -lc
# CFLAGS += -D__NO_LIBC

SOURCES += main.c

SDK_PATH ?= ../../sdk
include $(SDK_PATH)/multi-target.mk
