PROJECT = hello-world
DEBUG := 1

OPT := -Os
LDLIBS += -lcrt-static
DEFINES += -D__NO_LIBC
CPPFLAGS += -mthumb -Wno-unused-variable
SOURCES += main.c

SDK_PATH ?= ../../sdk
include $(SDK_PATH)/multi-target.mk
