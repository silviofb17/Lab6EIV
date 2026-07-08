MODULES = hal
BOARD ?= edu-ciaa-nxp
VERBOSE=n
MUJU ?= ./muju
DOC_DIR = ./build/doc

include $(MUJU)/module/base/makefile

# Documentar el programa
doc:
	@mkdir -p $(DOC_DIR)
	doxygen doxyfile