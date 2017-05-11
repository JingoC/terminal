
/*
volatile uint8_t rsbuf[50];
volatile uint16_t lengthRsBuf = 0;
volatile bool CmdExec = false;

uint8_t _test_cmd(char** argv, uint8_t argc)
{
	if (argc < 3)
		return TE_ArgErr;

	uint32_t p1 = atoiDec(argv[1]);
	uint32_t p2 = atoiHex(argv[2]);
	
	uint32_t sum = p1 + p2;
	TerminalTx("\r\nSum: %d [0x%08X]", sum, sum);

	return TE_OK;
}

// Function callback event presset Enter key (config in file terminal_config.h)
void RS_RXCallback(uint8_t* buf, uint16_t length)
{
	CmdExec = true;
	memcpy(rsbuf, buf, length);
}

void Init()
{
	TerminalInit();
	TerminalSetEnterCallback(RS_RXCallback);

	TerminalAddCmd("test", _test_cmd, "test command\n\r\t\ttest [arg1]");
}

// uart interrupt
UART_Interrupt()
{
	TerminalPutChar(UART_GetChar());
}

void main()
{
	Init();

	while(1)
	{
		if (CmdExec)
		{
			int8_t result = TerminalExecString((char*) rsbuf);
			TerminalPrintResultExec(result);
			CmdExec = false;
		}
	}
}
*/
