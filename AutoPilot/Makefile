###############################################################################
# Makefile for the project IrissPilot
###############################################################################

## General Flags
PROJECT = IrissPilot
MCU = atmega2560
TARGET = IrissPilot.elf
CXX = avr-c++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -DF_CPU=16000000L -O0 -fsigned-char -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d
CFLAGS += -DCONFIG_HAL_BOARD=HAL_BOARD_APM2

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=IrissPilot.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

include libraryIncludes.mk
include libraries.mk

## Include Directories
INCLUDES = \
    -I/usr/lib/avr/include \
	${LIBRARY_INCLUDES}

LIBDIRS = \
	-L/usr/lib/avr/lib/avr6 \
	-L/usr/lib/avr/lib \
	-L../avr/debug/lib

LIBS = \
	${ARDUPILOT_LIBS}

## Objects that must be built in order to link
OBJECTS = main.o Packable.o Command.o Orientation.o

## Objects explicitly added by the user
LINKONLYOBJECTS =

## Build
all: $(TARGET) IrissPilot.hex IrissPilot.eep IrissPilot.lss size

## Compile
%.o: %.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<

##Link
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) -lc -lm $(LIBS) -lc -o $(TARGET) 

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS) $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Program board on /dev/ttyACM0
upload: $(TARGET)
	/usr/bin/avrdude -c stk500v2 -p atmega2560 -P /dev/ttyACM0 -b115200  -U flash:w:IrissPilot.hex:i

## Clean target
.PHONY: clean
clean:
	rm -rf $(OBJECTS) IrissPilot.elf dep/* IrissPilot.hex IrissPilot.eep IrissPilot.lss IrissPilot.map 
