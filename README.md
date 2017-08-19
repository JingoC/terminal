Command Line Interface for microcontrollers
===========================================

Flexible terminal settings allow you to integrate it with any microcontroller, without much effort.

#### Features:
+ Flexible setup
+ Platform detachment
+ Logging commands
+ The mechanism for aborting execution
+ Available keys KeyLeft, KeyRight, Delete, BackSpace, Home, End, Tab

Required space in the program memory with a minimum working configuration of ~ 10 KB

INSTALLATION
------------
All that is required to work with the terminal, copy all the sources to a shared folder:
        
        lib/
        module/             
        terminal.h
        terminal.c
        
Copy the configuration file to the project folder, configure the configuration file according to the required parameters:

        def_config/terminal_config.h
        
CONFIG FILE
-----------

In the configuration file, you must configure the terminal output:

        TERM_TX_RX_EN - the macro controls the inclusion of the terminal output

Override output functions:
        
        CLI_Printf
        CLI_PutChar

Next, you must specify the basic parameters of the terminal:

        TERM_SIZE_TASK
        TERM_CMD_BUF_SIZE
        TERM_CMD_LOG_SIZE
        TERM_ARGS_BUF_SIZE
        TERM_ARG_SIZE

After the above, additional terminal components can be configured.

QUICK START
-----------

There are examples of working with a terminal for Windows (you need to install a bundle of virtual COM ports COM1 <-> COM2) and for microcontrollers.

        examples/
