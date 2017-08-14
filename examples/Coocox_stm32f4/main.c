#include "global_inc.h"

#include "stm32f4xx.h"

#include "core_cm4.h"
#include "core_cmFunc.h"

#include "system_stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"

#include "terminal_config.h"
#include "terminal/terminal.h"

#include "my_test_commands/my_test_commands.h"

char dbgbuffer[256];

#define TERMINAL_USART			USART3
#define TERMINAL_GPIO			GPIOD

void _reset_fcn()
{
	NVIC_SystemReset();
}

inline void TUSART_PutChar(char c)
{
	while (!USART_GetFlagStatus(TERMINAL_USART, USART_FLAG_TXE));
	USART_SendData(TERMINAL_USART, c);
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
	SystemCoreClockUpdate();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(TERMINAL_GPIO, &gpio);

	GPIO_PinAFConfig(TERMINAL_GPIO, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(TERMINAL_GPIO, GPIO_PinSource9, GPIO_AF_USART3);

	USART_InitTypeDef usart;
	usart.USART_BaudRate = 9600;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_WordLength = USART_WordLength_8b;

	USART_Init(TERMINAL_USART, &usart);

	USART_Cmd(TERMINAL_USART, ENABLE);
	USART_ITConfig(TERMINAL_USART, USART_IT_RXNE, ENABLE);

	SysTick_Config(SystemCoreClock / 100000);

	NVIC_EnableIRQ(USART3_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);

	__enable_irq();
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

void USART3_IRQHandler()
{
	USART_ClearITPendingBit(TERMINAL_USART, USART_IT_RXNE);
	CLI_EnterChar(USART_ReceiveData(TERMINAL_USART));
}

volatile uint64_t SysTickCtr = 0;
void SysTick_Handler()
{
	SysTickCtr++;
}
