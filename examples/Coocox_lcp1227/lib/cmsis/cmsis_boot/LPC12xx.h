/**************************************************************************//**
 * $Id: LPC12xx.h 360 2010-08-24 06:26:20Z cnh20509 $
 *
 * @file     LPC12xx.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for 
 *           NXP LPC12xx Device Series
 * @version  1.0
 * @date     26. Sep. 2010
 * @author   NXP MCU Team
 *
 * @note
 * Copyright (C) 2010 NXP Semiconductors(NXP). All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 ******************************************************************************/

#ifndef __LPC12xx_H__
#define __LPC12xx_H__

#ifdef __cplusplus
extern "C" {
#endif 

/** @addtogroup LPC12xx_Definitions LPC12xx Definitions
  This file defines all structures and symbols for LPC12xx:
    - Registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
  @{
*/


/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup LPC12xx_CMSIS LPC12xx CMSIS Definitions
  Configuration of the Cortex-M0 Processor and Core Peripherals
  @{
*/

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,    /*!< 2 Non Maskable Interrupt                           */
  HardFault_IRQn                = -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                   */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                     */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                     */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                 */

/******  LPC12xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_IRQn                  = 0,        /*!< All I/O pins can be used as wakeup source.       */
  WAKEUP1_IRQn                  = 1,        /*!< There are 12 pins in total for LPC12xx           */
  WAKEUP2_IRQn                  = 2,
  WAKEUP3_IRQn                  = 3,
  WAKEUP4_IRQn                  = 4,   
  WAKEUP5_IRQn                  = 5,        
  WAKEUP6_IRQn                  = 6,        
  WAKEUP7_IRQn                  = 7,        
  WAKEUP8_IRQn                  = 8,        
  WAKEUP9_IRQn                  = 9,        
  WAKEUP10_IRQn                 = 10,       
  WAKEUP11_IRQn                 = 11,              
  I2C_IRQn                      = 12,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 13,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 14,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 15,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 16,       /*!< 32-bit Timer1 Interrupt                          */
  SSP_IRQn                      = 17,       /*!< SSP Interrupt                                    */
  UART0_IRQn                    = 18,       /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 19,       /*!< UART1 Interrupt                                  */
  CMP_IRQn                      = 20,       /*!< Comparator Interrupt                             */
  ADC_IRQn                      = 21,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 22,       /*!< Watchdog timer Interrupt                         */  
  BOD_IRQn                      = 23,       /*!< Brown Out Detect(BOD) Interrupt                  */
  FLASH_IRQn                    = 24,       /*!< Flash Interrupt                                  */
  EINT0_IRQn                    = 25,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 26,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 27,       /*!< External Interrupt 2 Interrupt                   */
  PMU_IRQn                      = 28,       /*!< PMU Interrupt                                    */
  DMA_IRQn                      = 29,       /*!< DMA Interrupt                                    */
  RTC_IRQn                      = 30,       /*!< RTC Interrupt                                    */
  EDM_IRQn                      = 31,       /*!< EDT Interrupt                                    */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */

/*@}*/ /* end of group LPC12xx_CMSIS */


#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include "system_LPC12xx.h"                 /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral Registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- System Control (SYSCON) --------------------------------------*/
/** @addtogroup LPC12xx_SYSCON LPC12xx System Control Block 
  @{
*/
typedef struct
{
  __IO uint32_t SYSMEMREMAP;            /*!< Offset: 0x000 System memory remap (R/W) */
  __IO uint32_t PRESETCTRL;             /*!< Offset: 0x004 Peripheral reset control (R/W) */
  __IO uint32_t SYSPLLCTRL;             /*!< Offset: 0x008 System PLL control (R/W) */
  __IO uint32_t SYSPLLSTAT;             /*!< Offset: 0x00C System PLL status (R/ ) */
       uint32_t RESERVED0[4];

  __IO uint32_t SYSOSCCTRL;             /*!< Offset: 0x020 System oscillator control (R/W) */
  __IO uint32_t WDTOSCCTRL;             /*!< Offset: 0x024 Watchdog oscillator control (R/W) */
  __IO uint32_t IRCCTRL;                /*!< Offset: 0x028 IRC control (R/W) */
  __IO uint32_t RTCOSCCTRL;             /*!< Offset: 0x02C RTC oscillator control (R/W) */
  
  __I  uint32_t SYSRESSTAT;             /*!< Offset: 0x030 System reset status Register (R/ ) */
       uint32_t RESERVED1[3];
  __IO uint32_t SYSPLLCLKSEL;           /*!< Offset: 0x040 System PLL clock source select (R/W) */	
  __IO uint32_t SYSPLLCLKUEN;           /*!< Offset: 0x044 System PLL clock source update enable (R/W) */
       uint32_t RESERVED2[10];

  __IO uint32_t MAINCLKSEL;             /*!< Offset: 0x070 Main clock source select (R/W) */
  __IO uint32_t MAINCLKUEN;             /*!< Offset: 0x074 Main clock source update enable (R/W) */
  __IO uint32_t SYSAHBCLKDIV;           /*!< Offset: 0x078 System AHB clock divider (R/W) */
       uint32_t RESERVED3[1];

  __IO uint32_t SYSAHBCLKCTRL;          /*!< Offset: 0x080 System AHB clock control (R/W) */
       uint32_t RESERVED4[4];
  __IO uint32_t SSPCLKDIV;              /*!< Offset: 0x094 SSP clock divider (R/W) */               
  __IO uint32_t UART0CLKDIV;            /*!< Offset: 0x098 UART0 clock divider (R/W) */
  __IO uint32_t UART1CLKDIV;            /*!< Offset: 0x09C UART1 clock divider (R/W) */
  
  __IO uint32_t RTCCLKDIV;              /*!< Offset: 0x0A0 RTC clock divider (R/W) */
       uint32_t RESERVED5[3];       
  __IO uint32_t SYSTICKCLKDIV;          /*!< Offset: 0x0B0 SysTick clock divider (R/W) */          
       uint32_t RESERVED6[11];
            
  __IO uint32_t CLKOUTCLKSEL;           /*!< Offset: 0x0E0 CLKOUT clock source select (R/W) */
  __IO uint32_t CLKOUTUEN;              /*!< Offset: 0x0E4 CLKOUT clock source update enable (R/W) */
  __IO uint32_t CLKOUTDIV;              /*!< Offset: 0x0E8 CLKOUT clock divider (R/W) */       
       uint32_t RESERVED8[5];   

  __I  uint32_t PIOPORCAP0;             /*!< Offset: 0x100 POR captured PIO status 0 (R/) */            
  __I  uint32_t PIOPORCAP1;             /*!< Offset: 0x104 POR captured PIO status 1 (R/) */ 
       uint32_t RESERVED9[11];
       
  __IO uint32_t FILTERCLKCFG6;
  __IO uint32_t FILTERCLKCFG5;
  __IO uint32_t FILTERCLKCFG4;
  __IO uint32_t FILTERCLKCFG3;          /* Offset 0x140 */
  __IO uint32_t FILTERCLKCFG2;
  __IO uint32_t FILTERCLKCFG1;
  __IO uint32_t FILTERCLKCFG0;	
    	  	            
  __IO uint32_t BODCTRL;                 /*!< Offset: 0x150 BOD control (R/W) */
  __IO uint32_t SYSTCKCAL;               /*!< Offset: 0x154 System tick counter calibration (R/W) */              
  __IO uint32_t AHBPRIO;                 /*!< Offset: 0x158 AHB priority setting (R/W) */
  
       uint32_t RESERVED10[5];    
  __IO uint32_t IRQLATENCY;             /*!< Offset: 0x170 IRQ delay (R/W) */
  __IO uint32_t INTNMI;                 /*!< Offset: 0x174 NMI interrupt source configuration control (R/W) */
       uint32_t RESERVED11[34];          

  __IO uint32_t STARTAPRP0;             /*!< Offset: 0x200 Start logic edge control Register 0 (R/W) */   
  __IO uint32_t STARTERP0;              /*!< Offset: 0x204 Start logic signal enable Register 0 (R/W) */  
  __O  uint32_t STARTRSRP0CLR;          /*!< Offset: 0x208 Start logic reset Register 0  ( /W) */
  __I  uint32_t STARTSRP0;              /*!< Offset: 0x20C Start logic status Register 0 (R/) */
  __IO uint32_t STARTAPRP1;             /*!< Offset: 0x210 Start logic edge control Register 1 (R/W) */   
  __IO uint32_t STARTERP1;              /*!< Offset: 0x214 Start logic signal enable Register 1 (R/W) */  
  __O  uint32_t STARTRSRP1CLR;          /*!< Offset: 0x218 Start logic reset Register 1  ( /W) */
  __I  uint32_t STARTSRP1;              /*!< Offset: 0x21C Start logic status Register 1 (R/) */  
       uint32_t RESERVED12[4];

  __IO uint32_t PDSLEEPCFG;             /*!< Offset: 0x230 Power-down states in Deep-sleep mode (R/W) */
  __IO uint32_t PDAWAKECFG;             /*!< Offset: 0x234 Power-down states after wake-up (R/W) */       
  __IO uint32_t PDRUNCFG;               /*!< Offset: 0x238 Power-down configuration Register (R/W) */

       uint32_t RESERVED13[110];
       
  __I  uint32_t DEVICE_ID;              /*!< Offset: 0x3F4 Device ID (R/ ) */
} LPC_SYSCON_TypeDef;
/*@}*/ /* end of group LPC12xx_SYSCON */


/*------------- Pin Connect Block (IOCON) --------------------------------*/
/** @addtogroup LPC12xx_IOCON LPC12xx I/O Configuration Block 
  @{
*/
typedef struct
{
  __IO uint32_t PIO2_28;            /*!< Offset: 0x000  (R/W) */
  __IO uint32_t PIO2_29;            /*!< Offset: 0x004  (R/W) */
  __IO uint32_t PIO0_19;            /*!< Offset: 0x008 Configures pin PIO0_19/ACMP0_I0/CT32B0_1.  (R/W) */
  __IO uint32_t PIO0_20;
  __IO uint32_t PIO0_21;
  __IO uint32_t PIO0_22;
  __IO uint32_t PIO0_23;
  __IO uint32_t PIO0_24;

  __IO uint32_t SWDIO_PIO0_25;     /*!< Offset: 0x020 pin SWDIO/ACMP1_I2/CT32B1_2/CT16B0_RST/PIO0_25.  (R/W) */
  __IO uint32_t SWCLK_PIO0_26;
  __IO uint32_t PIO0_27;
  __IO uint32_t PIO2_12;
  __IO uint32_t PIO2_13;
  __IO uint32_t PIO2_14;
  __IO uint32_t PIO2_15;
  __IO uint32_t PIO0_28;

  __IO uint32_t PIO0_29;            /*!< Offset: 0x040 Configures pin PIO0_29/ROSC/CT16B0_1.  (R/W) */
  __IO uint32_t PIO0_0;
  __IO uint32_t PIO0_1;
  __IO uint32_t PIO0_2;
       uint32_t RESERVED0;
  __IO uint32_t PIO0_3;
  __IO uint32_t PIO0_4;
  __IO uint32_t PIO0_5;

  __IO uint32_t PIO0_6;             /*!< Offset: 0x060 Configures pin PIO0_6/RI0/CT32B1_0.  (R/W) */
  __IO uint32_t PIO0_7;
  __IO uint32_t PIO0_8;
  __IO uint32_t PIO0_9;
  __IO uint32_t PIO2_0;
  __IO uint32_t PIO2_1;
  __IO uint32_t PIO2_2;
  __IO uint32_t PIO2_3;

  __IO uint32_t PIO2_4;             /*!< Offset: 0x080 Configures pin PIO2_4/CT32B0_0/CTS0.  (R/W) */
  __IO uint32_t PIO2_5;
  __IO uint32_t PIO2_6;
  __IO uint32_t PIO2_7;
  __IO uint32_t PIO0_10;
  __IO uint32_t PIO0_11;
  __IO uint32_t PIO0_12;
  __IO uint32_t RESET_P0_13;

  __IO uint32_t PIO0_14;            /*!< Offset: 0x0A0 Configures pin PIO0_14/SSP_CLK.  (R/W) */
  __IO uint32_t PIO0_15;
  __IO uint32_t PIO0_16;
  __IO uint32_t PIO0_17;
  __IO uint32_t PIO0_18;
  __IO uint32_t PIO0_30;
  __IO uint32_t PIO0_31;
  __IO uint32_t PIO1_0;

  __IO uint32_t PIO1_1;                /*!< Offset: 0x0C0 Configures pin R/PIO1_1/AD3.  (R/W) */
  __IO uint32_t PIO1_2;
  __IO uint32_t PIO1_3;
  __IO uint32_t PIO1_4;
  __IO uint32_t PIO1_5;
  __IO uint32_t PIO1_6;
       uint32_t RESERVED1[2];

  __IO uint32_t PIO2_8;             /*!< Offset: 0x0E0 Configures pin PIO2_8/CT32B1_0.  (R/W) */
  __IO uint32_t PIO2_9;
  __IO uint32_t PIO2_10;
  __IO uint32_t PIO2_11;
  
} LPC_IOCON_TypeDef;
/*@}*/ /* end of group LPC12xx_IOCON */

/*------------- Micro DMA controller (DMA) --------------------------*/
/** @addtogroup LPC12xx_DMA LPC12xx Micro DMA controller 
  @{
*/
typedef struct
{
  __I  uint32_t STATUS;             /*!< Offset: 0x000 DMA status register (R/) */
  __O  uint32_t CFG;
  __IO uint32_t CTRL_BASE_PTR;
  __I  uint32_t ALT_CTRL_BASE_PTR;
  __I  uint32_t WAITONREQ_STATUS;
  __O  uint32_t CHNL_SW_REQUEST;
  __IO uint32_t CHNL_USEBURST_SET;
  __O  uint32_t CHNL_USEBURST_CLR;
  
  __IO uint32_t CHNL_REQ_MASK_SET;  /*!< Offset: 0x020 Channel request mask set register (R/W) */
  __O  uint32_t CHNL_REQ_MASK_CLR;
  __IO uint32_t CHNL_ENABLE_SET;
  __O  uint32_t CHNL_ENABLE_CLR;
  __IO uint32_t CHNL_PRI_ALT_SET;
  __O  uint32_t CHNL_PRI_ALT_CLR;
  __IO uint32_t CHNL_PRIORITY_SET;
  __O  uint32_t CHNL_PRIORITY_CLR;
  
	   uint32_t RESERVE0[3];        /* 0x40 */
  __IO uint32_t ERR_CLR;            /*!< Offset: 0x04C Bus error clear register (R/W) */
  	   uint32_t RESERVE1[12];
  	   
  __IO uint32_t CHNL_IRQ_STATUS;    /*!< Offset: 0x080 Channel DMA interrupt status register (R/W) */
  __IO uint32_t IRQ_ERR_ENABLE;
  __IO uint32_t CHNL_IRQ_ENABLE;     
} LPC_DMA_TypeDef;
/*@}*/ /* end of group LPC12xx_DMA */


/*------------- Comparator (COMP) --------------------------*/
/** @addtogroup LPC12xx_COMP LPC12xx Comparator 
  @{
*/
typedef struct
{
  __IO uint32_t CMP;        /*!< Offset: 0x000 Comparator control register (R/W) */
  __IO uint32_t VLAD;       /*!< Offset: 0x004 Voltage ladder register (R/W) */
} LPC_COMP_TypeDef;
/*@}*/ /* end of group LPC12xx_COMP */


/*------------- Real Time Clock (RTC) --------------------------*/
/** @addtogroup LPC12xx_RTC LPC12xx Real Time Clock 
  @{
*/
typedef struct
{    
  __I  uint32_t DR;     /*!< Offset: 0x000 Data register (R/) */
  __IO uint32_t MR;
  __IO uint32_t LR;
  __IO uint32_t CR;
  __IO uint32_t IMSC;
  __I  uint32_t RIS;
  __I  uint32_t MIS;
  __O uint32_t ICR;
} LPC_RTC_TypeDef;
/*@}*/ /* end of group LPC12xx_RTC */


/*------------- Power Management Unit (PMU) --------------------------*/
/** @addtogroup LPC12xx_PMU LPC12xx Power Management Unit 
  @{
*/
typedef struct
{
  __IO uint32_t PCON;                   /*!< Offset: 0x000 Power control Register (R/W) */
  __IO uint32_t GPREG0;                 /*!< Offset: 0x004 General purpose Register 0 (R/W) */
  __IO uint32_t GPREG1;                 /*!< Offset: 0x008 General purpose Register 1 (R/W) */
  __IO uint32_t GPREG2;                 /*!< Offset: 0x00C General purpose Register 2 (R/W) */
  __IO uint32_t GPREG3;                 /*!< Offset: 0x010 General purpose Register 3 (R/W) */
  __IO uint32_t GPREG4;                 /*!< Offset: 0x014 General purpose Register 4 (R/W) */
} LPC_PMU_TypeDef;
/*@}*/ /* end of group LPC12xx_PMU */


/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @addtogroup LPC12xx_GPIO LPC12xx General Purpose Input/Output 
  @{
*/
typedef struct
{    
  __IO uint32_t MASK;       /*!< Offset: 0x000 Pin value mask register (R/W) */
  __I  uint32_t PIN;
  __IO uint32_t OUT;
  __O  uint32_t SET;
  __O  uint32_t CLR;        /*!< Offset: 0x010 Pin output value clear register (R/W) */
  __O  uint32_t NOT;
  	   uint32_t RESERVE[2];
  __IO uint32_t DIR;        /*!< Offset: 0x020 Data direction register (R/W) */
  __IO uint32_t IS;
  __IO uint32_t IBE;
  __IO uint32_t IEV;
  __IO uint32_t IE;
  __I  uint32_t RIS;
  __I  uint32_t MIS;
  __O  uint32_t IC;
} LPC_GPIO_TypeDef;
/*@}*/ /* end of group LPC12xx_GPIO */


/*------------- Timer (TMR) --------------------------------------------------*/
/** @addtogroup LPC12xx_TMR LPC12xx 16/32-bit Counter/Timer 
  @{
*/
typedef struct
{
  __IO uint32_t IR;                     /*!< Offset: 0x000 Interrupt Register (R/W) */
  __IO uint32_t TCR;                    /*!< Offset: 0x004 Timer Control Register (R/W) */
  __IO uint32_t TC;                     /*!< Offset: 0x008 Timer Counter Register (R/W) */
  __IO uint32_t PR;                     /*!< Offset: 0x00C Prescale Register (R/W) */
  __IO uint32_t PC;                     /*!< Offset: 0x010 Prescale Counter Register (R/W) */
  __IO uint32_t MCR;                    /*!< Offset: 0x014 Match Control Register (R/W) */
  __IO uint32_t MR0;                    /*!< Offset: 0x018 Match Register 0 (R/W) */
  __IO uint32_t MR1;                    /*!< Offset: 0x01C Match Register 1 (R/W) */
  __IO uint32_t MR2;                    /*!< Offset: 0x020 Match Register 2 (R/W) */
  __IO uint32_t MR3;                    /*!< Offset: 0x024 Match Register 3 (R/W) */
  __IO uint32_t CCR;                    /*!< Offset: 0x028 Capture Control Register (R/W) */
  __I  uint32_t CR0;                    /*!< Offset: 0x02C Capture Register 0 (R/ ) */
  __I  uint32_t CR1;                    /*!< Offset: 0x030 Capture Register 1 (R/ ) */
  __I  uint32_t CR2;                    /*!< Offset: 0x034 Capture Register 2 (R/ ) */
  __I  uint32_t CR3;                    /*!< Offset: 0x038 Capture Register 3 (R/ ) */
  __IO uint32_t EMR;                    /*!< Offset: 0x03C External Match Register (R/W) */
       uint32_t RESERVED2[12];
  __IO uint32_t CTCR;                   /*!< Offset: 0x070 Count Control Register (R/W) */
  __IO uint32_t PWMC;                   /*!< Offset: 0x074 PWM Control Register (R/W) */  
} LPC_TMR_TypeDef;
/*@}*/ /* end of group LPC12xx_TMR */


/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @addtogroup LPC12xx_UART LPC12xx Universal Asynchronous Receiver/Transmitter 
  @{
*/
typedef struct
{
  union {
  __I  uint32_t  RBR;                /*!< Offset: 0x000 Receiver Buffer  Register (R/ ) */
  __O  uint32_t  THR;               /*!< Offset: 0x000 Transmit Holding Register ( /W) */             
  __IO uint32_t  DLL;                /*!< Offset: 0x000 Divisor Latch LSB (R/W) */            
  };
  union {
  __IO uint32_t  DLM;               /*!< Offset: 0x004 Divisor Latch MSB (R/W) */    
  __IO uint32_t  IER;                /*!< Offset: 0x000 Interrupt Enable Register (R/W) */
  };
  union {
  __I  uint32_t  IIR;                /*!< Offset: 0x008 Interrupt ID Register (R/ ) */      
  __O  uint32_t  FCR;              /*!< Offset: 0x008 FIFO Control Register ( /W) */      
  };
  __IO uint32_t  LCR;               /*!< Offset: 0x00C Line Control Register (R/W) */
  __IO uint32_t  MCR;              /*!< Offset: 0x010 Modem control Register (R/W) */          
  __I  uint32_t  LSR;               /*!< Offset: 0x014 Line Status Register (R/ ) */   
  __I  uint32_t  MSR;              /*!< Offset: 0x018 Modem status Register (R/ ) */      
  __IO uint32_t  SCR;               /*!< Offset: 0x01C Scratch Pad Register (R/W) */             
  __IO uint32_t  ACR;              /*!< Offset: 0x020 Auto-baud Control Register (R/W) */
  __IO uint32_t  ICR;               /*!< Offset: 0x024 IrDA Control Register (R/W) */      
  __IO uint32_t  FDR;               /*!< Offset: 0x028 Fractional Divider Register (R/W) */
       uint32_t  RESERVED0;
  __IO uint32_t  TER;               /*!< Offset: 0x030 Transmit Enable Register (R/W) */    
       uint32_t  RESERVED1[6];
  __IO uint32_t  RS485CTRL;         /*!< Offset: 0x04C RS-485/EIA-485 Control Register (R/W) */
  __IO uint32_t  ADRMATCH;        /*!< Offset: 0x050 RS-485/EIA-485 address match Register (R/W) */
  __IO uint32_t  RS485DLY;          /*!< Offset: 0x054 RS-485/EIA-485 direction control delay Register (R/W) */
  __I  uint32_t  FIFOLVL;           /*!< Offset: 0x058 FIFO Level Register (R/ ) */
} LPC_UART_TypeDef;
/*@}*/ /* end of group LPC12xx_UART */


/*------------- Synchronous Serial Communication (SSP) -----------------------*/
/** @addtogroup LPC12xx_SSP LPC12xx Synchronous Serial Port 
  @{
*/
typedef struct
{
  __IO uint32_t CR0;                    /*!< Offset: 0x000 Control Register 0 (R/W) */
  __IO uint32_t CR1;                    /*!< Offset: 0x004 Control Register 1 (R/W) */
  __IO uint32_t DR;                     /*!< Offset: 0x008 Data Register (R/W) */
  __I  uint32_t SR;                     /*!< Offset: 0x00C Status Registe (R/ ) */
  __IO uint32_t CPSR;                   /*!< Offset: 0x010 Clock Prescale Register (R/W) */
  __IO uint32_t IMSC;                   /*!< Offset: 0x014 Interrupt Mask Set and Clear Register (R/W) */
  __IO uint32_t RIS;                    /*!< Offset: 0x018 Raw Interrupt Status Register (R/W) */
  __IO uint32_t MIS;                    /*!< Offset: 0x01C Masked Interrupt Status Register (R/W) */
  __IO uint32_t ICR;                    /*!< Offset: 0x020 Interrupt Clear Register (R/W) */
  __IO uint32_t DMACR;                  /*!< Offset: 0x024 DMA Control Register (R/W) */
} LPC_SSP_TypeDef;
/*@}*/ /* end of group LPC12xx_SSP */


/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
/** @addtogroup LPC12xx_I2C LPC12xx I2C-Bus Interface 
  @{
*/
typedef struct
{
  __IO uint32_t CONSET;                 /*!< Offset: 0x000 I2C Control Set Register (R/W) */
  __I  uint32_t STAT;                   /*!< Offset: 0x004 I2C Status Register (R/ ) */
  __IO uint32_t DAT;                    /*!< Offset: 0x008 I2C Data Register (R/W) */
  __IO uint32_t ADR0;                   /*!< Offset: 0x00C I2C Slave Address Register 0 (R/W) */
  __IO uint32_t SCLH;                   /*!< Offset: 0x010 SCH Duty Cycle Register High Half Word (R/W) */
  __IO uint32_t SCLL;                   /*!< Offset: 0x014 SCL Duty Cycle Register Low Half Word (R/W) */
  __O  uint32_t CONCLR;                 /*!< Offset: 0x018 I2C Control Clear Register ( /W) */
  __IO uint32_t MMCTRL;                 /*!< Offset: 0x01C Monitor mode control register (R/W) */
  __IO uint32_t ADR1;                   /*!< Offset: 0x020 I2C Slave Address Register 1 (R/W) */
  __IO uint32_t ADR2;                   /*!< Offset: 0x024 I2C Slave Address Register 2 (R/W) */
  __IO uint32_t ADR3;                   /*!< Offset: 0x028 I2C Slave Address Register 3 (R/W) */
  __I  uint32_t DATA_BUFFER;            /*!< Offset: 0x02C Data buffer register ( /W) */
  __IO uint32_t MASK0;                  /*!< Offset: 0x030 I2C Slave address mask register 0 (R/W) */
  __IO uint32_t MASK1;                  /*!< Offset: 0x034 I2C Slave address mask register 1 (R/W) */
  __IO uint32_t MASK2;                  /*!< Offset: 0x038 I2C Slave address mask register 2 (R/W) */
  __IO uint32_t MASK3;                  /*!< Offset: 0x03C I2C Slave address mask register 3 (R/W) */
} LPC_I2C_TypeDef;
/*@}*/ /* end of group LPC12xx_I2C */


/*------------- Watchdog Timer (WDT) -----------------------------------------*/
/** @addtogroup LPC12xx_WDT LPC12xx WatchDog Timer 
  @{
*/
typedef struct
{
  __IO uint32_t MOD;                    /*!< Offset: 0x000 Watchdog mode register (R/W) */
  __IO uint32_t TC;                     /*!< Offset: 0x004 Watchdog timer constant register (R/W) */
  __O  uint32_t FEED;                   /*!< Offset: 0x008 Watchdog feed sequence register ( /W) */
  __I  uint32_t TV;                     /*!< Offset: 0x00C Watchdog timer value register (R/ ) */
  __IO uint32_t WDCLKSEL;               /*!< Offset: 0x010 Watchdog clock source selectuin register (R/W ) */ 
  __IO uint32_t WARNINT;                /*!< Offset: 0x014 Watchdog Warning Interrupt compare value register (R/W ) */
  __IO uint32_t WINDOW;                 /*!< Offset: 0x018 Watchdog Window compare value register (R/W ) */  
} LPC_WDT_TypeDef;
/*@}*/ /* end of group LPC12xx_WDT */


/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
/** @addtogroup LPC12xx_ADC LPC12xx Analog-to-Digital Converter 
  @{
*/
typedef struct
{
  __IO uint32_t CR;                     /*!< Offset: 0x000       A/D Control Register (R/W) */
  __IO uint32_t GDR;                    /*!< Offset: 0x004       A/D Global Data Register (R/W) */
       uint32_t RESERVED0;
  __IO uint32_t INTEN;                  /*!< Offset: 0x00C       A/D Interrupt Enable Register (R/W) */
  __IO uint32_t DR[8];                  /*!< Offset: 0x010-0x02C A/D Channel 0..7 Data Register (R/W) */
  __I  uint32_t STAT;                   /*!< Offset: 0x030       A/D Status Register (R/ ) */
} LPC_ADC_TypeDef;
/*@}*/ /* end of group LPC12xx_ADC */


/*------------- CRC Engine (CRC) ----------------------------*/
/** @addtogroup LPC12xx_CRC LPC12xx CRC Engine 
  @{
*/
typedef struct
{
  __IO uint32_t MODE;
  __IO uint32_t SEED;
  union {
  __I  uint32_t SUM;
  __O  uint32_t WR_DATA_DWORD;
  __O  uint16_t WR_DATA_WORD;
       uint16_t RESERVED_WORD;
  __O  uint8_t WR_DATA_BYTE;
       uint8_t RESERVED_BYTE[3];
  };
  __I  uint32_t ID;
} LPC_CRC_TypeDef;
/*@}*/ /* end of group LPC12xx_CRC */


/*------------- Flash Memory Controller (FMC) ----------------------------*/
/** @addtogroup LPC12xx_FMC LPC12xx Flash Memory Controller
  @{
*/
typedef struct
{
  __IO uint32_t TBCFG;            /* Time Base Config register  */
  __IO uint32_t FINSTR;           /* Flash Instruction register */
  __I  uint32_t INSSTA;           /* Raw Instruction Status register */
  __IO uint32_t INSSCLR;          /* Raw Instruction Clear register */
  __IO uint32_t INT_EN;           /* Interrupt Enable register */
  __I  uint32_t INT_STA;          /* Interrupt Status register */
       uint32_t RESERVED0;
  __IO uint32_t ADDRLAT;          /* Address Latch registers */
  __IO uint32_t DATALAT;          /* Data Latch register */
  __IO uint32_t FIMC;             /* Flash Manaul Operation register */
  __IO uint32_t RDCFG;            /* Read Configuration register */
  __IO uint32_t EPPCFG;           /* Flash Programming Permission Cofig register */
  __IO uint32_t EPPAA;            /* Flash Programming Permission Address A register */
  __IO uint32_t EPPAB;            /* Flash Programming Permission Address B register */
} LPC_FMC_TypeDef;
/*@}*/ /* end of group LPC12xx_FMC */


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_AHB_BASE          (0x50000000UL)

/* APB0 peripherals                                                           */
#define LPC_I2C_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x04000)
#define LPC_UART0_BASE        (LPC_APB0_BASE + 0x08000)
#define LPC_UART1_BASE        (LPC_APB0_BASE + 0x0C000)
#define LPC_CT16B0_BASE       (LPC_APB0_BASE + 0x10000)
#define LPC_CT16B1_BASE       (LPC_APB0_BASE + 0x14000)
#define LPC_CT32B0_BASE       (LPC_APB0_BASE + 0x18000)
#define LPC_CT32B1_BASE       (LPC_APB0_BASE + 0x1C000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x20000)
#define LPC_PMU_BASE          (LPC_APB0_BASE + 0x38000)
#define LPC_SSP_BASE          (LPC_APB0_BASE + 0x40000)
#define LPC_IOCON_BASE        (LPC_APB0_BASE + 0x44000)
#define LPC_SYSCON_BASE       (LPC_APB0_BASE + 0x48000)
#define LPC_DMA_BASE          (LPC_APB0_BASE + 0x4C000)
#define LPC_RTC_BASE          (LPC_APB0_BASE + 0x50000)
#define LPC_COMP_BASE         (LPC_APB0_BASE + 0x54000)

/* AHB peripherals                                                            */	
#define LPC_GPIO_BASE         (LPC_AHB_BASE  + 0x00000)
#define LPC_GPIO0_BASE        (LPC_AHB_BASE  + 0x00000)
#define LPC_GPIO1_BASE        (LPC_AHB_BASE  + 0x10000)
#define LPC_GPIO2_BASE        (LPC_AHB_BASE  + 0x20000)
#define LPC_GPIO3_BASE        (LPC_AHB_BASE  + 0x30000)
#define LPC_FMC_BASE          (LPC_AHB_BASE  + 0x60000)
#define LPC_CRC_BASE          (LPC_AHB_BASE  + 0x70000)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_I2C               ((LPC_I2C_TypeDef    *) LPC_I2C_BASE   )
#define LPC_WDT               ((LPC_WDT_TypeDef    *) LPC_WDT_BASE   )
#define LPC_UART0             ((LPC_UART_TypeDef   *) LPC_UART0_BASE)
#define LPC_UART1             ((LPC_UART_TypeDef   *) LPC_UART1_BASE)
#define LPC_TMR16B0           ((LPC_TMR_TypeDef    *) LPC_CT16B0_BASE)
#define LPC_TMR16B1           ((LPC_TMR_TypeDef    *) LPC_CT16B1_BASE)
#define LPC_TMR32B0           ((LPC_TMR_TypeDef    *) LPC_CT32B0_BASE)
#define LPC_TMR32B1           ((LPC_TMR_TypeDef    *) LPC_CT32B1_BASE)
#define LPC_ADC               ((LPC_ADC_TypeDef    *) LPC_ADC_BASE   )
#define LPC_PMU               ((LPC_PMU_TypeDef    *) LPC_PMU_BASE   )
#define LPC_SSP               ((LPC_SSP_TypeDef    *) LPC_SSP_BASE  )
#define LPC_IOCON             ((LPC_IOCON_TypeDef  *) LPC_IOCON_BASE )
#define LPC_SYSCON            ((LPC_SYSCON_TypeDef *) LPC_SYSCON_BASE)
#define LPC_DMA               ((LPC_DMA_TypeDef    *) LPC_DMA_BASE)
#define LPC_RTC               ((LPC_RTC_TypeDef    *) LPC_RTC_BASE)
#define LPC_COMP              ((LPC_COMP_TypeDef   *) LPC_COMP_BASE)
#define LPC_GPIO0             ((LPC_GPIO_TypeDef   *) LPC_GPIO0_BASE )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef   *) LPC_GPIO1_BASE )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef   *) LPC_GPIO2_BASE )
#define LPC_FMC               ((LPC_FMC_TypeDef    *) LPC_FMC_BASE)
#define LPC_CRC               ((LPC_CRC_TypeDef    *) LPC_CRC_BASE)

#ifdef __cplusplus
}
#endif

#endif  /* __LPC12xx_H__ */
