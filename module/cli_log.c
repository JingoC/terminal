#include "cli_log.h"

struct{
	char cmds[TERM_CMD_LOG_SIZE][TERM_CMD_BUF_SIZE];
	uint32_t _curCmd;
	uint32_t _cntCmd;
}TermLog;

void CLI_LogInit()
{
    CLI_CurReset();
	TermLog._cntCmd = 0;

	for(uint32_t i = 0; i < TERM_CMD_LOG_SIZE; i++)
	{
		TermLog.cmds[i][0] = '\0';
	}

}

void CLI_LogCmdPush(const char* cmd)
{
	if (TermLog._cntCmd < TERM_CMD_LOG_SIZE)
	{
		memcpy(TermLog.cmds[TermLog._cntCmd], cmd, TERM_CMD_BUF_SIZE);
		TermLog._cntCmd++;
	}
	else
	{
		memcpy(&TermLog.cmds[0][0], &TermLog.cmds[1][0], TERM_CMD_BUF_SIZE*(TERM_CMD_LOG_SIZE - 1));
		memcpy(&TermLog.cmds[TermLog._cntCmd-1][0], cmd, TERM_CMD_BUF_SIZE);
		TermLog._cntCmd = TERM_CMD_LOG_SIZE;
	}
}

const char* CLI_LogCmdGet(uint32_t index)
{
	if (index < TERM_CMD_LOG_SIZE)
	{
		return &TermLog.cmds[index][0];
	}

	return NULL;
}

const char* CLI_GetNextCmd()
{
	if (TermLog._curCmd < (TermLog._cntCmd - 1))
	{
		TermLog._curCmd++;
		return &TermLog.cmds[TermLog._curCmd][0];;
	}

	return NULL;
}

const char* CLI_GetLastCmd()
{

	if (TermLog._curCmd > 0)
	{
		TermLog._curCmd--;
		return &TermLog.cmds[TermLog._curCmd][0];
	}

	return NULL;
}

void CLI_CurReset()
{
	TermLog._curCmd = TermLog._cntCmd;
}
