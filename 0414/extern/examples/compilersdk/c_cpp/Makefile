#
#  Builds all MATLAB Compiler SDK example files (Windows, Linux, Mac)
#  Copyright 1997-2019 The MathWorks, Inc.
#
# You can invoke this Makefile using 
#  gmake [MATLABROOT=dir_where_MATLAB_or_runtime_is_installed] [c|mwarray_cpp|mda_cpp]
#
# By default, mda_cpp is chosen. 
# See README in this directory for details.
#
# MATLABROOT := [placeholder]

DIRECTORIES = $(sort $(dir $(wildcard */.)))

all c mwarray_cpp mda_cpp: $(DIRECTORIES)

$(DIRECTORIES):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: all $(DIRECTORIES)

