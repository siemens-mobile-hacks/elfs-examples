PROJECT = hello-world

OPT := -Os
LDLIBS += -lcrt -lcrt_helper -lgcc -lc-1.0 -lstdc++-1.2.2 -lsupc++-1.2.2

SOURCES += main.cpp TestClass.cpp

SDK_PATH ?= ../../sdk
include $(SDK_PATH)/multi-target.mk
