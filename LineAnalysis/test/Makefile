# Template for Makefiles, follow directions scattered throughout
# in the comments for the specific subdirectory

# This directory's location relative to the project root (do not end with '/')
RELATIVE=../..

# The library we are testing
UUT=LineAnalysis

# location of makeinclude for the host system in the top-level source directory
include ${RELATIVE}/common/makeinclude.host

# List all test modules, ending in .test (ex: Module1.cpp --> Module1.test)
TEST_UNITS = \
	Line.test \
	LineDetector.test

# List all directories where dependent headers are located
INCLUDES = ${SYS_INCLUDES} \
	-I${RELATIVE}/common \
	-I${RELATIVE}/host/${BUILD_VERSION}/include \
	-I${RELATIVE}

# List all directories and library names where dependent libraries
# are located. The library under test for the host system should be placed in
# the unit under test's directory
LIBS = ${SYS_LIBS} \
	-L./ \
	-L${RELATIVE}/host/${BUILD_VERSION}/lib \
	-l${UUT} \
	-lopencv_core \
	-lopencv_imgproc \
	-lopencv_highgui \
	-lUtils

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
