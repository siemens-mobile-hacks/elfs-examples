PROJECT = hello-world

OPT := -Os
LDLIBS += -lcrt -lcrt_helper -lgcc
DEFINES += -D__NO_LIBC
CPPFLAGS += -Wno-unused-variable

SOURCES += main.c

SDK_PATH ?= ../../sdk
include $(SDK_PATH)/multi-target.mk
