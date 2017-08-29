/**
 ******************************************************************************
 * @file      startup_lpc11xx.c
 * @author    Coocox
 * @version   V1.0
 * @date      05/25/2011
 * @brief     LPC12XX Devices Startup code.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the vector table entries with the exceptions ISR address
 *                - Initialize data and bss
 *                - Setup the microcontroller system.
 *                - Call the application's entry point.
 *******************************************************************************
 */
 

/*----------Stack Configuration-----------------------------------------------*/  
#define STACK_SIZE       0x00000200      /*!< The Stack size suggest using even number    */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];      

unsigned long stack_size = STACK_SIZE;

/*----------Macro definition--------------------------------------------------*/  
#define WEAK __attribute__ ((weak))           


/*----------Declaration of the default fault handlers-------------------------*/  
/* System exception vector handler */
__attribute__ ((used))
void WEAK  Reset_Handler(void);
void WEAK  NMI_Handler(void);
void WEAK  HardFault_Handler(void);
void WEAK  SVC_Handler(void);
void WEAK  PendSV_Handler(void);
void WEAK  SysTick_Handler(void);
void WEAK  WAKEUP_IRQHandler(void);
void WEAK  SSP_IRQHandler(void);
void WEAK  I2C_IRQHandler(void);
void WEAK  TIMER16_0_IRQHandler(void);
void WEAK  TIMER16_1_IRQHandler(void);
void WEAK  TIMER32_0_IRQHandler(void);
void WEAK  TIMER32_1_IRQHandler(void);
void WEAK  SSP0_IRQHandler(void);
void WEAK  UART0_IRQHandler(void);
void WEAK  UART1_IRQHandler(void);
void WEAK  ADC_IRQHandler(void);
void WEAK  COMP_IRQHandler(void);
void WEAK  WDT_IRQHandler(void);
void WEAK  BOD_IRQHandler(void);
void WEAK  FMC_IRQHandler(void);
void WEAK  PIOINT2_IRQHandler(void);
void WEAK  PIOINT1_IRQHandler(void);
void WEAK  PIOINT0_IRQHandler(void);
void WEAK  PMU_IRQHandler(void);
void WEAK  EDM_IRQHandler(void);
void WEAK  DMA_IRQHandler(void);
void WEAK  RTC_IRQHandler(void);


/*----------Symbols defined in linker script----------------------------------*/  
extern unsigned long _sidata;    	/*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     	/*!< Start address for the .data section     */    
extern unsigned long _edata;     	/*!< End address for the .data section       */    
extern unsigned long _sbss;      	/*!< Start address for the .bss section      */
extern unsigned long _ebss;      	/*!< End address for the .bss section        */      
extern void _eram;               	/*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/  
extern int main(void);           	/*!< The entry point for the application.    */
extern void SystemInit(void);    	/*!< Setup the microcontroller system(CMSIS) */
void Default_Reset_Handler(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */
void HardFault_Handler(void);  /*!< Default exception handler            */
void BusFault_Handler(void);

/**
  *@brief The minimal vector table for a Cortex M0.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.  
  */
__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{	
  /*----------Core Exceptions------------------------------------------------ */
  (void *)&pulStack[STACK_SIZE],  /*!< The initial stack pointer         */  
  Default_Reset_Handler,            /*!< Reset Handler                       */
  NMI_Handler,               		/*!< NMI Handler                                 */
  HardFault_Handler,         		/*!< Hard Fault Handler                          */ 
  0,                         		/*!< MPU Fault Handler                           */ 
  BusFault_Handler,                  /*!< Bus Fault Handler                           */
  0,                         		/*!< Usage Fault Handler                         */
  0,0,0,0,                   		/*!< Reserved                                    */
  SVC_Handler,               		/*!< SVCall Handler                              */
  0,                         		/*!< Debug Monitor Handler                       */
  0,                         		/*!< Reserved                                    */
  PendSV_Handler,            		/*!< PendSV Handler                              */
  SysTick_Handler,           		/*!< SysTick Handler                             */ 
  
  /*----------External Exceptions---------------------------------------------*/                
  WAKEUP_IRQHandler,         		/*!< 16+ 0: Wakeup PIO0.0                        */
  WAKEUP_IRQHandler,         		/*!< 16+ 1: Wakeup PIO0.1                        */
  WAKEUP_IRQHandler,         		/*!< 16+ 2: Wakeup PIO0.2                        */                   
  WAKEUP_IRQHandler,         		/*!< 16+ 3: Wakeup PIO0.2                        */                 
  WAKEUP_IRQHandler,         		/*!< 16+ 4: Wakeup PIO0.2                        */               
  WAKEUP_IRQHandler,         		/*!< 16+ 5: Wakeup PIO0.2                        */
  WAKEUP_IRQHandler,         		/*!< 16+ 6: Wakeup PIO0.2                        */
  WAKEUP_IRQHandler,         		/*!< 16+ 7: Wakeup PIO0.2                        */              
  WAKEUP_IRQHandler,         		/*!< 16+ 8: Wakeup PIO0.2                        */                
  WAKEUP_IRQHandler,         		/*!< 16+ 9: Wakeup PIO0.2                        */               
  WAKEUP_IRQHandler,         		/*!< 16+ 10: Wakeup PIO0.2                       */
  WAKEUP_IRQHandler,         		/*!< 16+ 11: Wakeup PIO0.2                       */
  I2C_IRQHandler,            		/*!< 16+ 12: I2C                                 */
  TIMER16_0_IRQHandler,      		/*!< 16+ 13: 16-bit Timer0                       */
  TIMER16_1_IRQHandler,      		/*!< 16+ 14: 16-bit Timer1                       */
  TIMER32_0_IRQHandler,      		/*!< 16+ 15: 32-bit Timer0                       */ 
  TIMER32_1_IRQHandler,      		/*!< 16+ 16: 32-bit Timer1                       */
  SSP_IRQHandler,            		/*!< 16+ 17: SSP                                 */
  UART0_IRQHandler,         		/*!< 16+ 18: UART0                               */
  UART1_IRQHandler,          		/*!< 16+ 19: UART1                               */
  COMP_IRQHandler,            		/*!< 16+ 20: Comparator                          */
  ADC_IRQHandler,            		/*!< 16+ 21: A/D Converter                       */
  WDT_IRQHandler,            		/*!< 16+ 22: Watchdog timer                      */
  BOD_IRQHandler,            		/*!< 16+ 23: Brown Out Detect                    */
  FMC_IRQHandler,            		/*!< 16+ 24: IP2111 Flash Memory Controller      */
  PIOINT0_IRQHandler,        		/*!< 16+ 25: PIO INT0                            */
  PIOINT1_IRQHandler,        		/*!< 16+ 26: PIO INT1                            */
  PIOINT2_IRQHandler,        		/*!< 16+ 27: PIO INT2                            */
  PMU_IRQHandler,             		/*!< 16+ 28: PWU/Wakeup                          */
  DMA_IRQHandler,             		/*!< 16+ 29: DMA                                 */
  RTC_IRQHandler,            		/*!< 16+ 30: RTC                                 */
  EDM_IRQHandler             		/*!< 16+ 31: Event Driver Micro                  */
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called. 
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &_sidata;

  for(pulDest = &_sdata; pulDest < &_edata; )
  {
    *(pulDest++) = *(pulSrc++);
  }
  
  /* Zero fill the bss segment. */
  for(pulDest = &_sbss; pulDest < &_ebss; )
  {
    *(pulDest++) = 0;
  }

  /* Setup the microcontroller system. */
  SystemInit();
	
  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler. 
  *       As they are weak aliases, any function with the same name will override 
  *       this definition.
  */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler
#pragma weak SVC_Handler = Default_Handler
#pragma weak PendSV_Handler = Default_Handler
//#pragma weak SysTick_Handler = Default_Handler
#pragma weak WAKEUP_IRQHandler = Default_Handler
#pragma weak SSP_IRQHandler = Default_Handler
#pragma weak I2C_IRQHandler = Default_Handler
#pragma weak TIMER16_0_IRQHandler = Default_Handler
#pragma weak TIMER16_1_IRQHandler = Default_Handler
#pragma weak TIMER32_0_IRQHandler = Default_Handler
#pragma weak TIMER32_1_IRQHandler = Default_Handler
#pragma weak SSP0_IRQHandler = Default_Handler
#pragma weak UART0_IRQHandler = Default_Handler
#pragma weak UART1_IRQHandler = Default_Handler
#pragma weak ADC_IRQHandler = Default_Handler
#pragma weak COMP_IRQHandler = Default_Handler
#pragma weak WDT_IRQHandler = Default_Handler
#pragma weak BOD_IRQHandler = Default_Handler
#pragma weak FMC_IRQHandler = Default_Handler
#pragma weak PIOINT2_IRQHandler = Default_Handler
#pragma weak PIOINT1_IRQHandler = Default_Handler
#pragma weak PIOINT0_IRQHandler = Default_Handler
#pragma weak PMU_IRQHandler = Default_Handler
#pragma weak EDM_IRQHandler = Default_Handler
#pragma weak DMA_IRQHandler = Default_Handler
#pragma weak RTC_IRQHandler = Default_Handler

/**
  * @brief  This is the code that gets called when the processor receives an 
  *         unexpected interrupt.  This simply enters an infinite loop, 
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None  
  */
static void Default_Handler(void) 
{
	/* Go into an infinite loop. */
	while (1)
	{
		//rtt_printf("\nDefault Handler");
	}
}

void BusFault_Handler(void)
{
	while(1){}
}

/*********************** (C) COPYRIGHT 2010 Coocox ************END OF FILE*****/
