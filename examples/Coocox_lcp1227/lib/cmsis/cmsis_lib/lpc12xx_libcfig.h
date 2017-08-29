/*******************************************************************************************
 * $Id: lpc12xx_libcfg_default.h 550 2010-09-20 06:55:52Z cnh20509 $
 *
 * @file     lpc12xx_libcfg.h
 * @brief	 Default Library configuration header file
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
 *******************************************************************************************/
 
/* Library Configuration group ------------------------------------------------------------*/
/** @defgroup LIBCFG_DEFAULT
 * @ingroup LPC1200CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC12xx_LIBCFG_DEFAULT_H
#define __LPC12xx_LIBCFG_DEFAULT_H

/* Includes -------------------------------------------------------------------------------*/
#include "lpc_types.h"

/* Public Macros --------------------------------------------------------------------------*/
/** @defgroup LIBCFG_DEFAULT_Public_Macros
 * @{
 */

/************************** DEBUG MODE DEFINITIONS *****************************************/
/* Un-comment the line below to compile the library in DEBUG mode, this will expanse
   the "CHECK_PARAM" macro in the FW library code */

//#define DEBUG


/******************* PERIPHERAL FW LIBRARY CONFIGURATION DEFINITIONS ***********************/

/* Comment the line below to disable the specific peripheral inclusion */

/* GPIO -------------------------------- */
#define _GPIO	
#define _GPIO_INT

/* UART -------------------------------- */
#define _UART 	
#define _UART_INT
//#define _MODEM
//#define _RS485
//#define _IRDA

/* IOCON ------------------------------- */
#define _IOCON

/* CRC --------------------------------- */
#define _CRC

/* SSP --------------------------------- */
#define _SSP 

/* FLASH ------------------------------- */
#define _FLASH

/* RTC --------------------------------- */
#define _RTC

/* I2C --------------------------------- */
#define _I2C  

/* PMU --------------------------------- */
#define _PMU 

/* TIMER32 ----------------------------- */
#define _TIMER32

/* TIMER16 ----------------------------- */
#define _TIMER16

/* WDT --------------------------------- */
#define _WDT 

/* DMA --------------------------------- */
#define _DMA 

/* SYSCTRL ----------------------------- */
#define _SYSCTRL

/* ADC --------------------------------- */
#define _ADC	

/* comparator -------------------------- */
#define _COMP

/************************** GLOBAL/PUBLIC MACRO DEFINITIONS *********************************/

#ifdef  DEBUG
/*******************************************************************************
* @brief		The CHECK_PARAM macro is used for function's parameters check.
* 				It is used only if the library is compiled in DEBUG mode.
* @param[in]	expr - If expr is false, it calls check_failed() function
*                    	which reports the name of the source file and the source
*                    	line number of the call that failed.
*                    - If expr is true, it returns no value.
* @return		None
*******************************************************************************/
#define CHECK_PARAM(expr) ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
#else
#define CHECK_PARAM(expr)
#endif /* DEBUG */

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup LIBCFG_Public_Functions
 * @{
 */

#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line);
#endif

/**
 * @}
 */

#endif /* __LPC12xx_LIBCFG_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
