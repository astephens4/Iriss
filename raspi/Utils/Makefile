# Template for Makefiles, follow directions scattered throughout
# in the comments for the specific subdirectory

# This directory's location relative to the project root (do not end with '/')
RELATIVE=..

# This directory's name + parents inside of PROJECT_ROOT (ex. if this is ${PROJECT_ROOT}/libA/module1, then ME=libA/module1)
ME=Utils

# location of makeinclude in the top-level source directory
include ${RELATIVE}/common/makeinclude

# name of output library
TYPE = lib
TARGET = Utils
OUTPUT = lib${TARGET}.a

# List all source-headers pairs with a .o extension (i.e. Class.cpp Class.h --> Class.o)
OBJS = \
	Color.o \
	Angle.o \
	Degrees.o \
	Radians.o \
	Distance.o \
	Feet.o \
	Inches.o

# List all Template class files with the .s extension
TEMPLATES = \

# List all directories where dependent headers are located
INCLUDES = ${INCLUDES} \
	-I${RELATIVE}/common/${BUILD_VERSION}/include \

# List all directories and library names where dependent libraries
# are located
# Do _not_ link against other libraries when making a static library

##########    If all variables above are correctly filled out,
##########    nothing below should have to be changed.

${TARGET}: ${TEMPLATES} ${OBJS}
	@echo Linking ${TARGET} ...
	${AR} rcs ${OUTPUT} ${OBJS}
	${INSTALL} ${OUTPUT} ${RELATIVE}/${BUILD_VERSION}/${TYPE}
	@echo

%.o: %.cpp %.h
	@echo Compiling $< ... $@
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}
	@echo

%.s: %.h
	@echo Checking syntax of template $< ...
	${CXX} -c ${CXXFLAGS} $< -o $@ ${INCLUDES}
	@echo

clean:
	@echo Cleaning up objects...
	rm -f ${OBJS} 
	@echo Cleaning up precomiled templates...
	rm -f ${TEMPLATES}
	@echo Removing temporary files and core dumps...
	rm -f *~ *core	

very_clean: clean
	@echo Removing ${TARGET}
	rm -f ${OUTPUT}
