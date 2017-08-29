/**************************************************************************//**
 * $Id: lpc12xx_sysctrl.h 457 2010-09-08 06:01:37Z cnh82208 $
 *
 * @file     lpc12xx_sysctrl.h
 * @brief    Contains all macro definitions and function prototypes
 *           support for SysCtrl firmware library on LPC12xx.
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

/* Peripheral group --------------------------------------------------------- */
/** @defgroup SYSCTRL
 * @ingroup LPC1200CMSIS_FwLib_Drivers 
 * @{
 */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LPC12xx_SYSCTRL_H
#define __LPC12xx_SYSCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "LPC12xx.h"
#include "lpc_types.h"


/* Public Macros -------------------------------------------------------------- */
/** @defgroup SYSCTRL_Public_Macros
 * @{
 */

/** @defgroup SYS_remap_mode 
  * @{
  */
  
#define SYS_BOOTLOADERMODE		0x00	/**< Interrupt vectors are re-mapped to Boot ROM */
#define SYS_USERRAMMODE			0x01	/**< Interrupt vectors are re-mapped to Static RAM */
#define SYS_USERFLASHMODE		0x02 	/**< Interrupt vectors are not re-mapped and reside in Flash */

/**
  * @}
  */


/** @defgroup SYS_clock_source 
  * @{
  */
    
#define SYS_IRCOSC				0x00	/**< IRC oscillator */
#define	SYS_SYSOSC				0x01	/**< System oscillator */
#define SYS_SYSPLLCLKIN			0x01	/**< Input clock to system PLL */
#define SYS_WDTOSC				0x02	/**< WDT oscillator */
#define SYS_SYSPLLCLKOUT		0x03	/**< System PLL clock out */
#define SYS_MAINCLK				0x01	/**< Main clock */
#define SYS_MAINCLKOUT			0x03	/**< Main clock */

/**
  * @}
  */


/** @defgroup SYS_RTCOSC_control 
  * @{
  */
#define SYS_RTC_1HZ				0x00	/**< 1 Hz clock */
#define SYS_RTC_DELAY1HZ		0x01	/**< delayed 1 Hz clock */
#define SYS_RTC_1KHZ			0x03	/**< 1 KHz clock */
#define SYS_RTC_PCLK			0x07	/**< RTC PCLK */

/**
  * @}
  */


/** @defgroup SYS_AHBCLK_control 
  * @{
  */

#define SYS_AHBCLKCTRL_SYS		(1<<0)	/**< Enables clock for AHB to APB bridge, to AHB matrix,
												to Cortex-M3 FCLK and HCLK, to SysCon, and to
												PMU. This bit is read only */
#define SYS_AHBCLKCTRL_ROM		(1<<1)	/**< Clock for ROM */
#define SYS_AHBCLKCTRL_RAM		(1<<2)	/**< Clock for RAM */
#define SYS_AHBCLKCTRL_FLASH1	(1<<3)	/**< Clock for Flash 1 */
#define SYS_AHBCLKCTRL_FLASH2	(1<<4)	/**< Clock for Flash 2 */
#define SYS_AHBCLKCTRL_I2C		(1<<5)	/**< Clock for I2C */
#define SYS_AHBCLKCTRL_CRC		(1<<6)	/**< Clock for CRC */
#define SYS_AHBCLKCTRL_CT16B0	(1<<7)	/**< Clock for 16 bit counter/timer 0 */
#define SYS_AHBCLKCTRL_CT16B1	(1<<8)	/**< Clock for 16 bit counter/timer 1 */
#define SYS_AHBCLKCTRL_CT32B0	(1<<9)	/**< Clock for 32 bit counter/timer 0 */
#define SYS_AHBCLKCTRL_CT32B1	(1<<10)	/**< Clock for 32 bit counter/timer 1 */
#define SYS_AHBCLKCTRL_SSP		(1<<11)	/**< Clock for SSP */
#define SYS_AHBCLKCTRL_UART0	(1<<12)	/**< Clock for UART0 */
#define SYS_AHBCLKCTRL_UART1	(1<<13)	/**< Clock for UART1 */
#define SYS_AHBCLKCTRL_ADC		(1<<14)	/**< Clock for ADC */
#define SYS_AHBCLKCTRL_WDT		(1<<15)	/**< Clock for WDT */
#define SYS_AHBCLKCTRL_IOCON	(1<<16)	/**< Clock for IO configuration block */
#define SYS_AHBCLKCTRL_DMA		(1<<17)	/**< Clock for DMA */
#define SYS_AHBCLKCTRL_EZH		(1<<18)	/**< Clock for EZH */
#define SYS_AHBCLKCTRL_RTC		(1<<19)	/**< Clock for RTC */
#define SYS_AHBCLKCTRL_CMP		(1<<20)	/**< Clock for CMP */
//#define SYS_AHBCLKCTRL_GPIO3	(1<<28)	/**< Clock for GPIO3 */
#define SYS_AHBCLKCTRL_GPIO2	(1<<29)	/**< Clock for GPIO2 */
#define SYS_AHBCLKCTRL_GPIO1	(1<<30)	/**< Clock for GPIO1 */
#define SYS_AHBCLKCTRL_GPIO0	(1UL<<31)	/**< Clock for GPIO0 */

/**
  * @}
  */


/** @defgroup SYS_peripheralRST_control 
  * @{
  */
/* Macro defines for Peripheral reset control register */
#define SYS_PRESETCTRL_SSP_RST		(1<<0)	/**< SSP reset disabled */
#define SYS_PRESETCTRL_I2C_RST		(1<<1)	/**< I2C reset disabled */
#define SYS_PRESETCTRL_UART0_RST		(1<<2)	/**< UART0 reset disabled */
#define SYS_PRESETCTRL_UART1_RST		(1<<3)	/**< UART1 reset disabled */
#define SYS_PRESETCTRL_CT16B0_RST	(1<<4)	/**< CT16B0 reset disabled */
#define SYS_PRESETCTRL_CT16B1_RST	(1<<5)	/**< CT16B1 reset disabled */
#define SYS_PRESETCTRL_CT32B0_RST	(1<<6)	/**< CT32B0 reset disabled */
#define SYS_PRESETCTRL_CT32B1_RST	(1<<7)	/**< CT32B1 reset disabled */
#define SYS_PRESETCTRL_CMP_RST		(1<<8)	/**< CMP reset disabled */
#define SYS_PRESETCTRL_CRC_RST		(1<<9)	/**< CRC reset disabled */
#define SYS_PRESETCTRL_DMA_RST		(1<<10)	/**< DMA reset disabled */

/**
  * @}
  */


/** @defgroup SYS_NMI_source_control 
  * @{
  */

#define SYS_NMI_I2C				0	/**< I2C interrupt */
#define SYS_NMI_CT16B1			1	/**< CT16B1 interrupt */
#define SYS_NMI_CT16B0			2	/**< CT16B0 interrupt */
#define SYS_NMI_CT32B0			3	/**< CT32B0 interrupt */
#define SYS_NMI_CT32B1			4	/**< CT32B1 interrupt */
#define SYS_NMI_SSP				5	/**< SSP interrupt */
#define SYS_NMI_UART0			6	/**< UART0 interrupt */
#define SYS_NMI_UART1			7	/**< UART1 interrupt */
#define SYS_NMI_COMP			8	/**< COMP interrupt */
#define SYS_NMI_ADC				9	/**< ADC interrupt */
#define SYS_NMI_WDT				10	/**< WDT interrupt */
#define SYS_NMI_BOD				11	/**< BOD interrupt */
#define SYS_NMI_FLASH			12	/**< FLASH interrupt */
#define SYS_NMI_EINT0			13	/**< EINT0 interrupt */
#define SYS_NMI_EINT1			14	/**< EINT1 interrupt */
#define SYS_NMI_EINT2			15	/**< EINT2 interrupt */
#define SYS_NMI_PMU				16	/**< PMU interrupt */
#define SYS_NMI_DMA				17	/**< DMA interrupt */
#define SYS_NMI_RTC				18	/**< RTC interrupt */
#define SYS_NMI_DISABLE			24	/**< NMI disabled */

/**
  * @}
  */


/** @defgroup SYS_startlogic0_control 
  * @{
  */

#define SYS_APR0_0				(1<<0)	/**< PIO0_0 */
#define SYS_APR0_1				(1<<1)	/**< PIO0_1 */
#define SYS_APR0_2				(1<<2)	/**< PIO0_2 */
#define SYS_APR0_3				(1<<3)	/**< PIO0_3 */
#define SYS_APR0_4				(1<<4)	/**< PIO0_4 */
#define SYS_APR0_5				(1<<5)	/**< PIO0_5 */
#define SYS_APR0_6				(1<<6)	/**< PIO0_6 */
#define SYS_APR0_7				(1<<7)	/**< PIO0_7 */
#define SYS_APR0_8				(1<<8)	/**< PIO0_8 */
#define SYS_APR0_9				(1<<9)	/**< PIO0_9 */
#define SYS_APR0_10				(1<<10)	/**< PIO0_10 */
#define SYS_APR0_11				(1<<11)	/**< PIO0_11 */

/**
  * @}
  */

/** @defgroup SYS_startlogic1_control 
  * @{
  */

#define SYS_APR1_I2C				(1<<0)	/**< I2C interrupt */
#define SYS_APR1_CT16B1				(1<<1)	/**< CT16B1 interrupt */
#define SYS_APR1_CT16B0				(1<<2)	/**< CT16B0 interrupt */
#define SYS_APR1_CT32B0				(1<<3)	/**< CT32B0 interrupt */
#define SYS_APR1_CT32B1				(1<<4)	/**< CT32B1 interrupt */
#define SYS_APR1_SSP				(1<<5)	/**< SSP interrupt */
#define SYS_APR1_UART0				(1<<6)	/**< UART0 interrupt */
#define SYS_APR1_UART1				(1<<7)	/**< UART1 interrupt */
#define SYS_APR1_COMP				(1<<8)	/**< COMP interrupt */
#define SYS_APR1_ADC				(1<<9)	/**< ADC interrupt */
#define SYS_APR1_WDT				(1<<10)	/**< WDT interrupt */
#define SYS_APR1_BOD				(1<<11)	/**< BOD interrupt */
#define SYS_APR1_FLASH				(1<<12)	/**< FLASH interrupt */
#define SYS_APR1_EINT0				(1<<13)	/**< EINT0 interrupt */
#define SYS_APR1_EINT1				(1<<14)	/**< EINT1 interrupt */
#define SYS_APR1_EINT2				(1<<15)	/**< EINT2 interrupt */
#define SYS_APR1_PMU				(1<<16)	/**< PMU interrupt */
#define SYS_APR1_DMA				(1<<17)	/**< DMA interrupt */
#define SYS_APR1_RTC				(1<<18)	/**< RTC interrupt */

/**
  * @}
  */


/** @defgroup SYS_edge_control
  * @{
  */

#define SYS_FALLINGEDGE			0x00
#define SYS_RISINGEDGE			0x01

/**
  * @}
  */


/** @defgroup SYS_power_control
  * @{
  */

#define SYS_PDRUNCFG_IRCOUT		(1<<0)	/**< IRC OSC output */
#define SYS_PDRUNCFG_IRC		(1<<1)	/**< IRC OSC */
#define SYS_PDRUNCFG_FLASH		(1<<2)	/**< Flash */
#define SYS_PDRUNCFG_BOD		(1<<3)	/**< BOD */
#define SYS_PDRUNCFG_ADC		(1<<4)	/**< ADC */
#define SYS_PDRUNCFG_SYSOSC		(1<<5)	/**< System OSC */
#define SYS_PDRUNCFG_WDTOSC		(1<<6)	/**< WDT OSC */
#define SYS_PDRUNCFG_SYSPLL		(1<<7)	/**< System PLL */

/**
  * @}
  */



/**
  * @}
  */
  
/* Public Functions ----------------------------------------------------------- */
/** @defgroup SYSCTRL_Public_Functions
 * @{
 */

uint32_t SYS_GetResetStatus(void); 
void SYS_SetMemMap(uint32_t MapValue);
void SYS_SelectSystemPLLCLKSource(uint32_t ClkSrc);
void SYS_ConfigSystemPLL(uint32_t M, uint32_t P, 
			 FunctionalState DirectState, FunctionalState BypassEn);
void SYS_SelectMainCLKSource(uint32_t ClkSrc);
void SYS_SetAHBClockDiv(uint32_t DivNum);
void SYS_ConfigAHBCLK(uint32_t AHBClk_Type, FunctionalState CmdState);
void SYS_ConfigWDTOSC(uint32_t DivNum, uint32_t FreqSel);
void SYS_ResetPeripheral(uint32_t RSTBlock, FunctionalState CmdState);
void SYS_SetSSPClockDiv(uint32_t DivNum);
void SYS_SetUART0ClockDiv(uint32_t DivNum);
void SYS_SetUART1ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter0ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter1ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter2ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter3ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter4ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter5ClockDiv(uint32_t DivNum);
void SYS_SetGlitchFilter6ClockDiv(uint32_t DivNum);
void SYS_SetSysTickClockDiv(uint32_t DivNum);
void SYS_ConfigCLKOUTCLK(uint32_t ClkSrc, uint32_t DivNum);
void SYS_ConfigDeepSleep(uint32_t DStype, FunctionalState CmdState);
void SYS_ConfigPowerDown(uint32_t PDtype, FunctionalState CmdState);
void SYS_ConfigWakeUp(uint32_t WUtype, FunctionalState CmdState);
void SYS_NMIInterrupt(uint32_t Peripheral_int);
void SYS_EnableStartLogic0(uint32_t Pin_x, FunctionalState CmdState);
void SYS_SetStartLogic0Edge(uint32_t Pin_x, uint32_t Edge);
void SYS_ResetStartLogic0(uint32_t Pin_x);
void SYS_EnableStartLogic0(uint32_t Peripheral_int, FunctionalState CmdState);
void SYS_SetStartLogic0Edge(uint32_t Peripheral_int, uint32_t Edge);
void SYS_ResetStartLogic0(uint32_t Peripheral_int);
uint32_t SYS_GetDeviceID(void);



/**
 * @}
 */ 

#ifdef __cplusplus
}
#endif

#endif /* __LPC12xx_SSP_H */

/**
 * @}
 */
 
/* --------------------------------- End Of File ------------------------------ */



