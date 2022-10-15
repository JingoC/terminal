#include "cli_arguments.h"

#include "../lib/cli_string.h"

extern volatile Terminal_s Terminal;

/// \brief Get index coinciding args string or -1
/// \param flag - searched string
int8_t _IndexOfFlag(const char* flag)
{
	for(uint8_t i = 0; i < Terminal.inputArgs.argc; i++)
	{
		if (cli_strcmp(Terminal.inputArgs.argv[i], flag))
		{
			return i;
		}
	}

	return -1;
}

bool CLI_IsArgFlag(const char* flag)
{
	return _IndexOfFlag(flag) >= 0;
}

uint32_t CLI_GetArgDec(uint8_t index)
{
	return ((uint32_t) strtol((const char*)Terminal.inputArgs.argv[index + 1], NULL, 10));
}

uint32_t CLI_GetArgHex(uint8_t index)
{
	return ((uint32_t) strtol((const char*)Terminal.inputArgs.argv[index + 1], NULL, 16));
}

bool CLI_GetArgDecByFlag(const char* flag, uint32_t* outValue)
{
	int8_t w = _IndexOfFlag(flag);
	if ((w > 0) && (w + 1 < Terminal.inputArgs.argc))
	{
		*outValue = CLI_GetArgDec(w);
		return true;
	}
	
	return false;
}

bool CLI_GetArgHexByFlag(const char* flag, uint32_t* outValue)
{
	int8_t w = _IndexOfFlag(flag);
	if ((w > 0) && (w + 1 < Terminal.inputArgs.argc))
	{
		*outValue = CLI_GetArgHex(w);
		return true;
	}
	
	return false;
}

/// \brief free memory
/// \return none
void CLI_ArgDestroy(Params_s* src)
{
	/*
    for(int8_t i = src->argc - 1; i > 0; i--)
    {
    	free((src->argv[i]));
    }

    free(src->argv);
    */
    src->argc = 0;
}
