#include "test_cmds.h"

#include "lib\temp_sensor\temperature.h"
#include "lib\timer\timer1.h"

#include "terminal_config.h"
#include "terminal.h"

uint8_t _t1_cmd();
uint8_t _t2_cmd();

void TestCmds_Init()
{
	CLI_AddCmd("t1", _t1_cmd, 1, 0, "");
	CLI_AddCmd("t2", _t2_cmd, 0, 0, "");
}

uint8_t _t1_cmd()
{
	uint32_t decVal = CLI_GetArgDec(0);
	uint32_t hexVal = 0;

	CLI_GetArgHexByFlag("-h", &hexVal);

	CLI_Printf("\r\nDec: %d\r\nHex: 0x%04X%04X", (int) decVal, 
			(unsigned int) (hexVal >> 16),
			(unsigned int) hexVal);

	if (CLI_IsArgFlag("-f"))
		CLI_Printf("\r\n-f: TRUE")
	else
		CLI_Printf("\r\n-f: FALSE");
	
	return TE_OK;
}

uint8_t _t2_cmd()
{
	uint32_t cnt = 1;
	uint32_t i = 0;

	uint8_t id = TMP_ReadReg(0x03);
	uint8_t ctrl = TMP_ReadReg(0x00);
	
	CLI_Printf("\r\nTemp CTRL: 0x%02X", ctrl);
	TMP_WriteReg(0x80, 0x00);
	
	ctrl = TMP_ReadReg(0x00);

	CLI_Printf("\r\nTemp ID: 0x%02X", id);
	CLI_Printf("\r\nTemp CTRL: 0x%02X", ctrl);

	CLI_GetArgDecByFlag("-c", &cnt);

	uint16_t secTmt = TMR1_GetSec();
	while(i < cnt)
	{
		CLI_CheckAbort();
		
		uint16_t newSecTmt = TMR1_GetSec();
		if (secTmt != newSecTmt)
		{
			uint8_t tl = TMP_ReadReg(0x01);
			uint8_t th = TMP_ReadReg(0x02);
			uint16_t t = ((uint16_t) th << 8) | tl;

			uint16_t s = t / 64;
			uint16_t temp = s * 0.25;
			CLI_Printf("\r\nTemperature: %d (0x%04X)", temp, t);

			secTmt = newSecTmt;
			i++;
		}
		 
	}
	

	return TE_OK;
}

