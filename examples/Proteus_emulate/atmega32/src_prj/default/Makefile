###############################################################################
# Makefile for the project atmega32_terminal
###############################################################################

## General Flags
PROJECT = atmega32_terminal
MCU = atmega32
TARGET = atmega32_terminal.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=atmega32_terminal.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\gitclone\terminal\examples\Proteus_emulate\atmega32\src_prj\." -I"D:\gitclone\terminal\examples\Proteus_emulate\atmega32\src_prj\..\..\..\.." -I"D:\gitclone\terminal\examples\Proteus_emulate\atmega32\src_prj\..\..\..\..\lib" -I"D:\gitclone\terminal\examples\Proteus_emulate\atmega32\src_prj\..\..\..\..\module" 

## Objects that must be built in order to link
OBJECTS = main.o uart.o terminal.o cli_log.o cli_time.o cli_malloc.o cli_queue.o cli_string.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) atmega32_terminal.hex atmega32_terminal.eep atmega32_terminal.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../lib/uart/uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

terminal.o: ../../../../../terminal.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cli_log.o: ../../../../../module/cli_log.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cli_time.o: ../../../../../module/cli_time.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cli_malloc.o: ../../../../../lib/cli_malloc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cli_queue.o: ../../../../../lib/cli_queue.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

cli_string.o: ../../../../../lib/cli_string.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) atmega32_terminal.elf dep/* atmega32_terminal.hex atmega32_terminal.eep atmega32_terminal.lss atmega32_terminal.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

