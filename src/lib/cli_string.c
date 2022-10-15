#include "cli_string.h"
#include "terminal_config.h"

uint16_t _getCountSeparator(const char* str, const char* separator);

/// \brief Splitting a string by a given separator
void cli_split(char* strSrc, const char* separator, Params_s* dst)
{
    uint8_t countSep = _getCountSeparator(strSrc, separator);
    uint16_t lenSep = cli_strlen((char*)separator);

    //dst->argv = (char**) malloc(sizeof(char*) * count_sep);
    dst->argc = countSep;

    int startIndex = 0;
    int sizeWord = 0;

    int co = 0;
    int coArg = 0;
    while(1)
    {
        uint16_t s = 0;

        for(; s < lenSep; s++){
            if((strSrc[co] == separator[s]) || (strSrc[co] == '\0'))
            {
                sizeWord = co - startIndex;

                //dst->argv[co_arg] = malloc(sizeof(char) * (sizeWord + 1));
                dst->argv[coArg][sizeWord] = '\0';
                cli_strcpy(strSrc, startIndex, dst->argv[coArg], 0, sizeWord);

                startIndex = co + 1;
                coArg++;
                break;
            }
        }

        if (strSrc[co] == '\0')
            break;

        co++;
    }
}

void cli_strcpy(const char* src, uint16_t offsetSrc, char* dst, uint16_t offsetDst, uint16_t length)
{
    uint16_t i = 0;
    for(; i < length; i++)
        dst[i + offsetDst] = src[i + offsetSrc];
}

char* cli_trim(const char* src)
{
    int16_t lengthDst;
    char* result;
    int16_t i = 0;
    int16_t len = cli_strlen(src);
    int16_t coStart = 0, coEnd = 0;

    for(; i < len; i++){
    	if (!((src[i] > 0x00) && (src[i] < 0x21)))
    		break;
        coStart++;
    }

    if (coStart == len){
        result = cli_malloc(sizeof(char));
        result[0] = '\0';
        return result;
    }

    for(i = len - 1; i >= 0; i++){
        if (!((src[i] > 0x00) && (src[i] < 0x21)))
            break;
        coEnd++;
    }

    lengthDst = len - coStart - coEnd;
    result = cli_malloc(sizeof(char) * lengthDst);
    cli_strcpy(src, coStart, result, 0, lengthDst);
    return result;
}

uint32_t cli_strlen(const char* strSrc)
{
	return (uint32_t) strlen(strSrc);
}

uint16_t _getCountSeparator(const char* strSrc, const char* separator)
{
    uint8_t i = 0;
    uint16_t result = 0;
    uint16_t count_separator = cli_strlen((char*) separator);

    for(; strSrc[i] != '\0'; i++)
    {
        uint16_t j = 0;
        for(; j < count_separator; j++)
            if (strSrc[i] == separator[j]){
                result++;
                break;
            }
    }

    return result + 1;
}

uint8_t cli_strPartCmp(const char* str1, const char* str2)
{
	uint8_t co = 0;
	while(((str1 + co) != NULL) && (*(str1 + co) != '\0') &&
          ((str2 + co) != NULL) && (*(str2 + co) != '\0')){

        if (str1[co] != str2[co])
            return 0;

        co++;
    }
    
    return 1;
}

uint8_t cli_strcmp(const char* str1, const char* str2)
{
    uint16_t co = 0;

	if (cli_strlen(str1) != cli_strlen(str2))
		return 0;

    while(((str1 + co) != NULL) && (*(str1 + co) != '\0') &&
          ((str2 + co) != NULL) && (*(str2 + co) != '\0')){

        if (str1[co] != str2[co])
            return 0;

        co++;
    }

    if  (
            (((str1 + co) == NULL) && ((str2 + co) != NULL)) ||
            (((str2 + co) == NULL) && ((str1 + co) != NULL)) ||
            (((str1 + co) != NULL) && ((str2 + co) != NULL) && (*(str2 + co) != *(str1 + co)))
         )
        return 0;


    return 1;
}
