/**************************************************************************//**
 * $Id: lpc12xx_gpio.h 519 2010-09-15 01:46:38Z nxp21428 $
 *
 * @file     lpc12xx_gpio.h
 * @brief    Contains all macro definitions and function prototypes
 *              support for GPIO firmware library on lpc12xx.
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

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup GPIO
 * @ingroup LPC1200CMSIS_FwLib_Drivers
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __LPC12xx_GPIO_H
#define __LPC12xx_GPIO_H

/* Includes ------------------------------------------------------------------- */
#include "LPC12xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros --------------------------------------------------------------- */
/** @defgroup GPIO_Public_Types
 * @{
 */ 
   
/** @defgroup GPIO_interrupt_type
  * @{
  */
#define GPIO_INTERRUPT_FALLING     ((uint8_t)(0))
#define GPIO_INTERRUPT_RISING      ((uint8_t)(1))
#define GPIO_INTERRUPT_BOTH_EDGES  ((uint8_t)(2))
#define GPIO_INTERRUPT_LOW         ((uint8_t)(3))
#define GPIO_INTERRUPT_HIGH        ((uint8_t)(4))

#define PARAM_GPIO_INTERRUPT(TYPE)  ((  TYPE == GPIO_INTERRUPT_FALLING)   || \
                                     (  TYPE == GPIO_INTERRUPT_RISING)    || \
                                     (  TYPE == GPIO_INTERRUPT_BOTH_EDGES)|| \
                                     (  TYPE == GPIO_INTERRUPT_LOW)       || \
                                     (  TYPE == GPIO_INTERRUPT_HIGH))

/**
  * @}
  */

/**
  * @}
  */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup GPIO_Public_Functions
 * @{
 */
void GPIO_SetMask(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t mask);
uint32_t GPIO_GetPinValue( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi);
void GPIO_SetOutValue(LPC_GPIO_TypeDef* pGPIO, uint32_t value);
void GPIO_SetHighLevel(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value);
void GPIO_SetLowLevel(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value);
void GPIO_SetInvert(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value);
void GPIO_SetDir(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value);
uint32_t GPIO_GetDir( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi);

#ifdef _GPIO_INT
void GPIO_IntSetType(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint8_t type );
uint32_t GPIO_IntGetRawStatus( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi );
void GPIO_IntSetMask(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t mask);
uint32_t GPIO_IntGetMaskStatus( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi );
void GPIO_IntClear(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value);
#endif

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif /* __LPC12xx_GPIO_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
