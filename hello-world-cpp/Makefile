PROJECT = hello-world

OPT := -Os
LDLIBS += -lcrt -lcrt_helper -lgcc -lc-1.0 -luclibc++-1.0 -lucsupc++

SOURCES += main.cpp TestClass.cpp

SDK_PATH := ../../sdk
include $(SDK_PATH)/multi-target.mk
