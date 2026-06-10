MODULES = hal src

INCLUDES = inc

BOARD ?= edu-ciaa-nxp
VERBOSE = n
MUJU ?= ./muju
DOC_DIR = ./build/doc

include $(MUJU)/module/base/makefile

doc:
	@mkdir -p $(DOC_DIR)
	doxygen doxyfile