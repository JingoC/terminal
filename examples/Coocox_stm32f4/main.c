#include "global_inc.h"

#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"

#include "terminal_config.h"
#include "terminal/terminal.h"

#include "my_test_commands/my_test_commands.h"

char dbgbuffer[256];

#define TERMINAL_USART			USART1

inline void TUSART_PutChar(char c)
{
	USART_SendData(TERMINAL_USART, c);
	while(USART_GetFlagStatus(TERMINAL_USART, USART_FLAG_TXE) != SET){}
}

void TUSART_Print(const char* str)
{
	while(str != NULL) {
		TUSART_PutChar(*str);

		if (*str == '\0')
			break;

		str++;
	}
}

void _InitHW()
{
	SystemInit();
	SystemCoreClockUpdate();

	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_OD;
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpio);

	USART_InitTypeDef usart;
	usart.USART_BaudRate = 115200;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usart);

	SysTick_Config(SystemCoreClock / 100000);

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}

void _InitSW()
{
	CLI_Init(TDC_Time);

	MyTestCmds_Init();
}

int main(void)
{
	_InitHW();
	_InitSW();

    while(1)
    {
    	CLI_Execute();
    }
}

void USART1_IRQHandler()
{
	CLI_EnterChar(USART_ReceiveData(USART1));
}

volatile uint64_t SysTickCtr = 0;
void SysTick_Handler()
{
	SysTickCtr++;
}
