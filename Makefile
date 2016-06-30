PROJECT_DIR  := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
ARDUINO_MAKEFILE = $(PROJECT_DIR)/arduino-mk/Arduino.mk
USER_LIB_PATH := $(PROJECT_DIR)/lib
ARDUINO_QUIET ?= 1

BOARD_TAG     = nano
BOARD_SUB     = atmega328
MONITOR_PORT  = /dev/ttyUSB0
MONITOR_CMD   = picocom -c -e b
ARDUINO_DIR   = /usr/share/arduino
AVR_TOOLS_DIR = /usr
AVRDUDE_CONF  = /etc/avrdude/avrdude.conf

OBJ_DIR       := $(PROJECT_DIR)/build-$(BOARD_TAG)
ifdef BOARD_SUB
    OBJDIR := $(PROJECT_DIR)/build-$(BOARD_TAG)-$(BOARD_SUB)
endif

CFLAGS_STD = -std=gnu11
CXXFLAGS_STD = -std=gnu++11

OVERRIDE_EXECUTABLES = 1
CC      = /usr/lib64/ccache/$(CC_NAME)
CXX     = /usr/lib64/ccache/$(CXX_NAME)
AS      = /usr/bin/$(AS_NAME)
OBJCOPY = /usr/bin/$(OBJCOPY_NAME)
OBJDUMP = /usr/bin/$(OBJDUMP_NAME)
AR      = /usr/bin/$(AR_NAME)
SIZE    = /usr/bin/$(SIZE_NAME)
NM      = /usr/bin/$(NM_NAME)

SRC_DIR          = $(PROJECT_DIR)/src
LOCAL_C_SRCS    := $(wildcard $(SRC_DIR)/*.c)
LOCAL_H_SRCS    := $(wildcard $(SRC_DIR)/*.h)
LOCAL_CPP_SRCS  := $(wildcard $(SRC_DIR)/*.cpp)
LOCAL_CC_SRCS   := $(wildcard $(SRC_DIR)/*.cc)
LOCAL_PDE_SRCS  := $(wildcard $(SRC_DIR)/*.pde)
LOCAL_INO_SRCS  := $(wildcard $(SRC_DIR)/*.ino)
LOCAL_AS_SRCS   := $(wildcard $(SRC_DIR)/*.S)

include $(ARDUINO_MAKEFILE)

check:
	clang-check $(LOCAL_C_SRCS) $(LOCAL_H_SRCS) \
	    -- -Wno-unknown-attributes \
	    -I/usr/avr/include \
	    -DF_CPU=16000000L -D__AVR_ATmega328P__

.PHONY: check
