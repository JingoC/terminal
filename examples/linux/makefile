CC = gcc -c
LD = gcc
#OC = objcopy
#OS = size

TARGET = terminal

OPT = -O0
DIR_TEST_COMMANDS = ./test_commands
DIR_TERMINAL = ../..
DIR_TERMINAL_lib = $(DIR_TERMINAL)/lib
DIR_TERMINAL_module = $(DIR_TERMINAL)/module
DIR_OBJ = ./.obj
DEBUG = -g

VPATH = $(DIR_TERMINAL) $(DIR_TERMINAL_lib) $(DIR_TERMINAL_module) $(DIR_TEST_COMMANDS) ./

DIR_FLAGS = -I. -I$(DIR_TERMINAL) -I$(DIR_TERMINAL_lib) -I$(DIR_TERMINAL_module) -I$(DIR_TEST_COMMANDS)

CFLAGS += $(OPT) $(DEBUG)
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -MMD -MP

PREPFLAGS = -MD -Wall -Wundef $(DEFS)

OBJS = $(DIR_OBJ)/main.o \
		$(DIR_OBJ)/cli_queue.o \
		$(DIR_OBJ)/cli_string.o \
		$(DIR_OBJ)/cli_input.o \
		$(DIR_OBJ)/cli_log.o \
		$(DIR_OBJ)/cli_time.o \
		$(DIR_OBJ)/terminal.o \
		$(DIR_OBJ)/test_commands.o \
		$(DIR_OBJ)/Time.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(LD) $(CFLAGS) $(OBJS) -o $(TARGET)
	
$(DIR_OBJ)/%.o : %.c
	mkdir -p $(DIR_OBJ)
	$(CC) $(CFLAGS) $(DIR_FLAGS) $(PREPFLAGS) $< -o $@
	
clean :
	rm -rf $(DIR_OBJ) $(TARGET)
	
-include $(OBJS:.o=.d)
