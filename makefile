ROOT := .
BOARD := edu-ciaa-nxp
MUJU := $(ROOT)/muju
MODULES := hal module/freertos
BUILD_DIR := $(ROOT)/build
include $(MUJU)/module/base/makefile