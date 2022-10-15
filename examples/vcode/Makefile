# Project: mcu_emulate
# Makefile created by Dev-C++ 5.11

CPP      = g++.exe -D__DEBUG__
CC       = gcc.exe -D__DEBUG__
WINDRES  = windres.exe
OBJ      = main.o ../../src/lib/cli_queue.o ../../src/lib/cli_string.o ../../src/module/cli_log.o ../../src/module/cli_time.o ../../src/terminal.o com/com.o irq/irq.o systimer/systimer.o commands/test_commands.o ../../src/module/cli_input.o ../../src/core/cli_commands.o ../../src/core/cli_arguments.o
LINKOBJ  = main.o ../../src/lib/cli_queue.o ../../src/lib/cli_string.o ../../src/module/cli_log.o ../../src/module/cli_time.o ../../src/terminal.o com/com.o irq/irq.o systimer/systimer.o commands/test_commands.o ../../src/module/cli_input.o ../../src/core/cli_commands.o ../../src/core/cli_arguments.o
LIBS     = -L"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib" -L"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib" -static-libgcc -g3
INCS     = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include" -I"D:/projects/my/c/terminal/src" -I"D:/projects/my/c/terminal/examples/vcode"
CXXINCS  = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include/c++" -I"D:/projects/my/c/terminal/src" -I"D:/projects/my/c/terminal/examples/vcode"
BIN      = mcu_emulate.exe
CXXFLAGS = $(CXXINCS) -std=gnu99 -g3 -std=c99
CFLAGS   = $(INCS) -std=gnu99 -g3 -std=c99
RM       = rm.exe -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)

../../lib/cli_queue.o: ../../lib/cli_queue.c
	$(CC) -c ../../lib/cli_queue.c -o ../../lib/cli_queue.o $(CFLAGS)

../../lib/cli_string.o: ../../lib/cli_string.c
	$(CC) -c ../../lib/cli_string.c -o ../../lib/cli_string.o $(CFLAGS)

../../module/cli_log.o: ../../module/cli_log.c
	$(CC) -c ../../module/cli_log.c -o ../../module/cli_log.o $(CFLAGS)

../../module/cli_time.o: ../../module/cli_time.c
	$(CC) -c ../../module/cli_time.c -o ../../module/cli_time.o $(CFLAGS)

../../terminal.o: ../../terminal.c
	$(CC) -c ../../terminal.c -o ../../terminal.o $(CFLAGS)

com/com.o: com/com.c
	$(CC) -c com/com.c -o com/com.o $(CFLAGS)

irq/irq.o: irq/irq.c
	$(CC) -c irq/irq.c -o irq/irq.o $(CFLAGS)

systimer/systimer.o: systimer/systimer.c
	$(CC) -c systimer/systimer.c -o systimer/systimer.o $(CFLAGS)

commands/test_commands.o: commands/test_commands.c
	$(CC) -c commands/test_commands.c -o commands/test_commands.o $(CFLAGS)

../../module/cli_input.o: ../../module/cli_input.c
	$(CC) -c ../../module/cli_input.c -o ../../module/cli_input.o $(CFLAGS)

../../core/cli_commands.o: ../../core/cli_commands.c
	$(CC) -c ../../core/cli_commands.c -o ../../core/cli_commands.o $(CFLAGS)

../../core/cli_arguments.o: ../../core/cli_arguments.c
	$(CC) -c ../../core/cli_arguments.c -o ../../core/cli_arguments.o $(CFLAGS)
