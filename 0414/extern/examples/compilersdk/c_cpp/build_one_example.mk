#
#  Builds MATLAB Compiler SDK example files (Windows, Linux, Mac)
#  Copyright 1997-2019 The MathWorks, Inc.
#
# You can invoke this Makefile using 
#  gmake [MATLABROOT=dir_where_MATLAB_or_runtime_is_installed] [c|mwarray_cpp|mda_cpp]
#
# By default, mda_cpp is chosen. 
# See README in this directory for more details.
#
# MATLABROOT := [placeholder]

ifeq ($(MATLABROOT),)
    $(error MATLABROOT must be defined. See README for details.)
endif

ifeq ($(DRIVER),)
    $(error The make command should only be run from one of the subdirectories of this directory)
endif

C_DRIVER := $(DRIVER).c
MWARRAY_CPP_DRIVER := $(DRIVER)_mwarray.cpp
MDA_CPP_DRIVER := $(DRIVER)_mda.cpp

OUT_DIR = output

ifeq ($(OS),Windows_NT)
	UNSTRIPPED_PATHSEP=\ dummy
	QUOTE="
	SCRIPT_EXT=.bat
else
	UNSTRIPPED_PATHSEP=/ dummy
	QUOTE='
	SCRIPT_EXT=
endif

PATHSEP=$(word 1, $(UNSTRIPPED_PATHSEP))

MCC := $(QUOTE)$(MATLABROOT)$(PATHSEP)bin$(PATHSEP)mcc$(SCRIPT_EXT)$(QUOTE)
MBUILD := $(QUOTE)$(MATLABROOT)$(PATHSEP)bin$(PATHSEP)mbuild$(SCRIPT_EXT)$(QUOTE)

C_OUT_DIR := $(OUT_DIR)$(PATHSEP)c
CPP_OUT_DIR := $(OUT_DIR)$(PATHSEP)cpp
MWARRAY_CPP_OUT_DIR := $(CPP_OUT_DIR)
MDA_CPP_OUT_DIR := $(CPP_OUT_DIR)$(PATHSEP)v2$(PATHSEP)generic_interface
C_OUT_DIR_AS_ARG := $(subst $(PATHSEP),/,$(C_OUT_DIR))
CPP_OUT_DIR_AS_ARG := $(subst $(PATHSEP),/,$(CPP_OUT_DIR))
MWARRAY_CPP_OUT_DIR_AS_ARG := $(subst $(PATHSEP),/,$(MWARRAY_CPP_OUT_DIR))
MDA_CPP_OUT_DIR_AS_ARG := $(subst $(PATHSEP),/,$(MDA_CPP_OUT_DIR))

ifeq ($(OS),Windows_NT)
	ifeq ($(C_SUPPORTED),1)
	C_TARGET := $(C_OUT_DIR)$(PATHSEP)$(DRIVER)_c.exe
	endif

	ifeq ($(MWARRAY_CPP_SUPPORTED),1)
	MWARRAY_CPP_TARGET := $(MWARRAY_CPP_OUT_DIR)$(PATHSEP)$(DRIVER)_mwarray.exe
	endif

	ifeq ($(MDA_CPP_SUPPORTED),1)
	MDA_CPP_TARGET := $(MDA_CPP_OUT_DIR)$(PATHSEP)$(DRIVER)_mda.exe
	endif
	BASE_PATH=
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ifeq ($(C_SUPPORTED),1)
	C_TARGET := $(C_OUT_DIR)$(PATHSEP)$(DRIVER)_c
	endif

	ifeq ($(MWARRAY_CPP_SUPPORTED),1)
	MWARRAY_CPP_TARGET := $(MWARRAY_CPP_OUT_DIR)$(PATHSEP)$(DRIVER)_mwarray
	endif

	ifeq ($(MDA_CPP_SUPPORTED),1)
		MDA_CPP_TARGET := $(MDA_CPP_OUT_DIR)$(PATHSEP)$(DRIVER)_mda
	endif
	INSTALL_NOTE_1 := Before running the executable, verify that the environment variable LD_LIBRARY_PATH is set and contains the following:
	BASE_PATH := $(MATLABROOT)/runtime/glnxa64:$(MATLABROOT)/bin/glnxa64:$(MATLABROOT)/sys/os/glnxa64:$(MATLABROOT)/sys/opengl/lib/glnxa64
endif
ifeq ($(UNAME_S),Darwin)
	ifeq ($(C_SUPPORTED),1)
	C_TARGET := $(C_OUT_DIR)/$(DRIVER)_c.app/Contents/MacOS/$(DRIVER)_c
	endif

	ifeq ($(MWARRAY_CPP_SUPPORTED),1)
	MWARRAY_CPP_TARGET := $(MWARRAY_CPP_OUT_DIR)/$(DRIVER)_mwarray.app/Contents/MacOS/$(DRIVER)_mwarray
	endif

	ifeq ($(MDA_CPP_SUPPORTED),1)
	MDA_CPP_TARGET := $(MDA_CPP_OUT_DIR)/$(DRIVER)_mda.app/Contents/MacOS/$(DRIVER)_mda
	endif

	INSTALL_NOTE_1 := Before running the executable, verify that the environment variable DYLD_LIBRARY_PATH is set and contains the following:
	BASE_PATH := $(MATLABROOT)/runtime/maci64:$(MATLABROOT)/bin/maci64:$(MATLABROOT)/sys/os/maci64
endif
endif

ifeq ($(OS),Windows_NT)
convert-pathsep = $(subst $(PATHSEP),$(PATHSEP)$(PATHSEP)$(PATHSEP)$(PATHSEP),$(1))
else
convert-pathsep = $(1)
endif

ifeq ($(OS),Windows_NT)
	C_TARGET_TO_ECHO := $(subst $(PATHSEP),$(PATHSEP)$(PATHSEP)$(PATHSEP)$(PATHSEP),$(C_TARGET))
	MWARRAY_CPP_TARGET_TO_ECHO := $(subst $(PATHSEP),$(PATHSEP)$(PATHSEP)$(PATHSEP)$(PATHSEP),$(MWARRAY_CPP_TARGET))
	MDA_CPP_TARGET_TO_ECHO := $(subst $(PATHSEP),$(PATHSEP)$(PATHSEP)$(PATHSEP)$(PATHSEP),$(MDA_CPP_TARGET))
else
	C_TARGET_TO_ECHO := $(C_TARGET)
	MWARRAY_CPP_TARGET_TO_ECHO := $(MWARRAY_CPP_TARGET)
	MDA_CPP_TARGET_TO_ECHO := $(MDA_CPP_TARGET)
endif

.PHONY: all c_target mwarray_cpp mda_cpp

ifeq ($(C_SUPPORTED),1)
c: $(C_TARGET)
else
c: 
	@echo No C files are provided for the $(DRIVER) code example. Specify another target.
endif

ifeq ($(MWARRAY_CPP_SUPPORTED),1)
mwarray_cpp: $(MWARRAY_CPP_TARGET)
else
mwarray_cpp: 
	@echo No mwarray C++ files are provided for the $(DRIVER) code example. Specify another target.
endif

ifeq ($(MDA_CPP_SUPPORTED),1)
mda_cpp: $(MDA_CPP_TARGET)
else
mda_cpp: 
	@echo No mda C++ files are provided for the $(DRIVER) code example. Specify another target.
endif

$(C_TARGET): $(MATLAB_FILES) $(C_DRIVER) $(C_OUT_DIR)
	@echo Generating C shared library ...
	$(MCC) -W lib:lib$(DRIVER) -T link:lib $(MATLAB_FILES) -g -d $(C_OUT_DIR_AS_ARG)
	@echo -----
	@echo Building executable...
	$(MBUILD) $(C_DRIVER) -I$(C_OUT_DIR_AS_ARG) -L$(C_OUT_DIR_AS_ARG) -l$(DRIVER) -g -output $(DRIVER)_c -outdir $(C_OUT_DIR_AS_ARG)
	@echo Wrote executable $(call convert-pathsep,$@)
ifneq ($(OS),Windows_NT)
	@echo -----
	@echo $(INSTALL_NOTE_1)
	@echo $(C_OUT_DIR):$(BASE_PATH)
endif

$(MWARRAY_CPP_TARGET): $(MATLAB_FILES) $(MWARRAY_CPP_DRIVER) $(MWARRAY_CPP_OUT_DIR)
	@echo Generating C++ shared mwArray library ...
	$(MCC) -W cpplib:lib$(DRIVER) $(MATLAB_FILES) -g -d $(MWARRAY_CPP_OUT_DIR_AS_ARG)
	@echo -----
	@echo Building executable...
	$(MBUILD) $(MWARRAY_CPP_DRIVER) -I$(MWARRAY_CPP_OUT_DIR_AS_ARG) -L$(MWARRAY_CPP_OUT_DIR_AS_ARG) -l$(DRIVER) -g -outdir $(MWARRAY_CPP_OUT_DIR_AS_ARG)
	@echo Wrote executable $(call convert-pathsep,$@)
ifneq ($(OS),Windows_NT)
	@echo -----
	@echo $(INSTALL_NOTE_1)
	@echo $(CPP_OUT_DIR):$(BASE_PATH)
endif

$(MDA_CPP_TARGET): $(MATLAB_FILES) $(MDA_CPP_DRIVER) $(MDA_CPP_OUT_DIR)
	@echo Generating C++ shared MATLAB Data API library ...
	$(MCC) -W cpplib:lib$(DRIVER) $(MATLAB_FILES) -g -d $(CPP_OUT_DIR_AS_ARG)
	@echo -----
	@echo Building executable...
	$(MBUILD) $(MDA_CPP_DRIVER) -I$(MDA_CPP_OUT_DIR_AS_ARG) -L$(MDA_CPP_OUT_DIR_AS_ARG) -g -outdir $(MDA_CPP_OUT_DIR_AS_ARG)
	@echo Wrote executable $(call convert-pathsep,$@)
ifneq ($(OS),Windows_NT)
	@echo -----
	@echo $(INSTALL_NOTE_1)
	@echo $(CPP_OUT_DIR):$(BASE_PATH)
endif

$(C_OUT_DIR) : $(OUT_DIR)
	-@mkdir $(C_OUT_DIR)

# $(MWARRAY_CPP_OUT_DIR) is equivalent to $(CPP_OUT_DIR).
$(MWARRAY_CPP_OUT_DIR) : $(OUT_DIR)
	-@mkdir $(CPP_OUT_DIR)

$(MDA_CPP_OUT_DIR) : $(OUT_DIR)
	-@mkdir $(CPP_OUT_DIR)
	-@mkdir $(CPP_OUT_DIR)$(PATHSEP)v2
	-@mkdir $(CPP_OUT_DIR)$(PATHSEP)v2$(PATHSEP)generic_interface

$(OUT_DIR):
	-@mkdir $(OUT_DIR)

.DEFAULT_GOAL := mda_cpp

