# Template for Makefiles, follow directions scattered throughout
# in the comments for the specific subdirectory

# This directory's location relative to the project root (do not end with '/')
RELATIVE=../..

# The library we are testing
UUT=Utils

# location of makeinclude in the top-level source directory
include ${RELATIVE}/common/makeinclude.host

# List all source-headers pairs with a .o extension (i.e. Class.cpp Class.h --> Class.o)
TEST_UNITS = \
	Angle.test \
	Color.test \
	Distance.test \
	Packable.test \
	TimeStamp.test \
	Net.test

# List all directories where dependent headers are located
INCLUDES = ${SYS_INCLUDES} \
	-I${RELATIVE}/common/${BUILD_VERSION}/include \
	-I${RELATIVE}/common \
	-I${RELATIVE}/${BUILD_VERSION}/include \
	-I${RELATIVE}

# List all directories and library names where dependent libraries
# are located
LIBS = ${SYS_LIBS} \
	-L./ \
	-l${UUT} \
	-lm

##########    If all variables above are correctly filled out,
##########    nothing below should have to be changed.

run_all: ${TEST_UNITS}
	@echo Running all tests...
	@python ${RELATIVE}/common/test/runtests.py ${TEST_UNITS}

default: ${TEST_UNITS}
	@echo Done building tests

%.test: %.cpp
	@echo Building $< ... $@
	${CXX} ${CXXFLAGS} $< -o $@ ${INCLUDES} ${LIBS}
	@echo

clean:
	@echo Cleaning up tests...
	rm -f ${TEST_UNITS} 
	@echo Removing temporary files and core dumps...
	rm -f *~ *core	

very_clean: clean
	@echo Removing test result outputs...
	rm -f results.*.txt passfail.txt
