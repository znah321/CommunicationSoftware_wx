#
# file : Makefile (UNIX)
#
#  Example of a stand-alone makefile for compiler-generated M files
#
#  Copyright 1997-2014 The MathWorks, Inc.
#

#
# You can invoke this Makefile using 
#  gmake -f Makefile MATLABROOT=[directory where MATLAB is installed]
#
# If you do not want to specify the MATLABROOT at the gmake command line 
# every single time, you can define it by uncommenting the line below
# and assigning the correct root of MATLAB (with no trailing '/') on
# the right hand side.
#
# MATLABROOT	:=
#

#
# Defaults
#

MCC=$(MATLABROOT)/bin/mcc

STAND_ALONE_TARGETS=hello magicsquare flames ex_loadsave


# Use the following target to build and test all the executables
bnt_exes: build_all
	@echo "*********** testing hello ***********"
	./hello 
	@echo "*********** testing flames ***********"
	./flames 
	@echo "*********** testing matrixdriver ***********"
	./matrixdriver 
	@echo "*********** testing ex_loadsave ***********"
	./ex_loadsave



# Use this target to only build all the targets
build_all: $(STAND_ALONE_TARGETS)


# The following are the definitions for each target individually.

hello:  hello.m 
	$(MCC) -m $^ -o $@
	@rm -f *.o*

flames:  flames.m flames.mat
	$(MCC) -m flames.m -a flames.mat
	@rm -f *.o*

magicsquare:  magicsquare.m
	$(MCC) -m $^ -o magicsquare
	@rm -f *.o*

ex_loadsave:  
	$(MCC) -m "./Data_Handling/ex_loadsave.m" -a "./Data_Handling/user_data.mat" -a "./Data_Handling/userdata/extra_data.mat" -a "./externdata/extern_data.mat" 
	@rm -f *.o*

clean:
	rm -f $(STAND_ALONE_TARGETS)

reallyclean:
	rm -rf $(STAND_ALONE_TARGETS)				\
		*.xml *.ctf *_mcr  		\
		mccExcludedFIles.log  \
		output	
