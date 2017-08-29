/**************************************************************************//**
 * $Id: lpc12xx_gpio.c 519 2010-09-15 01:46:38Z nxp21428 $
 *
 * @file     lpc12xxx_gpio.c
 * @brief    Contains all functions support for GPIO firmware library on LPC12xx.
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
/** @addtogroup GPIO
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc12xx_gpio.h"
#include "lpc12xx_libcfig.h"
/* Private Variables -------------------------------------------------------- */
/** @defgroup   GPIO_Private_Variables
 * @{
 */
/** @defgroup GPIO_port_num
  * @{
  */
#define PARAM_GPIOx(n)  ((((uint32_t *)n)==((uint32_t *)LPC_GPIO0)) || \
                         (((uint32_t *)n)==((uint32_t *)LPC_GPIO1)) || \
                         (((uint32_t *)n)==((uint32_t *)LPC_GPIO2)))

volatile uint32_t GPIOShadowPort[3];


/**
  * @}
  */ 

/**
 * @}
 */

/** @addtogroup GPIO_Private_Functions
 * @{
 */
static uint32_t gpio_getvalue( uint32_t *p, uint8_t bitPosi);
static void gpio_setvalue(uint32_t *p, uint8_t bitPosi, uint32_t value);


/**
  * @brief    get GPIO register value function.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param offset        Register offset value.
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, value is the 32bit value of register 
  * @return Current value of GPIO port.
  *
  * Note: Return value contain state of each port pin (bit) on that GPIO regardless
  * its direction is input or output.
  */
static uint32_t gpio_getvalue( uint32_t *p, uint8_t bitPosi)
{
    uint32_t regVal = 0;
    uint32_t tmp;
	    
    CHECK_PARAM(( bitPosi < 0x20 ) || ( bitPosi == 0xFF )); 

    tmp = *(uint32_t *)p;

    if( bitPosi < 0x20 ){   
        if ( tmp & (0x1<<bitPosi) ){
            regVal = 1;
        }
    }
    else if( bitPosi == 0xFF ){
        regVal = tmp;
    }   

    return ( regVal );
}

/**
  * @brief    Set GPIO register.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param offset        Register offset value.  
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                              - 0: no mask.
  *                              - 1: mask.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return  None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
static void gpio_setvalue(uint32_t *p, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(( bitPosi < 0x20 ) || ( bitPosi == 0xFF )); 
    CHECK_PARAM(( value == 0 ) || ( value == 1 ));
    
    if(bitPosi < 0x20){
        if (value) {
            *(uint32_t *)p |= (0x1<<bitPosi);
        }
        else {
            *(uint32_t *)p &= ~(0x1<<bitPosi);
        }
    }
    else if(bitPosi == 0xFF){
        *(uint32_t *)p = value;
    }
}

/**
  * @brief    mask GPIO port.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                              - 0: no mask.
  *                              - 1: mask.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return  None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetMask(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));
    gpio_setvalue((uint32_t *)&pGPIO->MASK, bitPosi, value);
}

/**
  * @brief       Read Current state on port pin, PIN register value
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, value is the 32bit value of register 
  * @return Current value of GPIO port.
  *
  * Note: Return value contain state of each port pin (bit) on that GPIO regardless
  * its direction is input or output. Read operations are masked by the GPIOnMASK
  * registers.
  */
uint32_t GPIO_GetPinValue( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi)
{   
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    return ( gpio_getvalue((uint32_t *)&pGPIO->PIN,bitPosi) );      
}

/**
  * @brief       Set Direction for GPIO port.
  * @param pGPIO     Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param value       set the all 32-bits of register
  * @return  None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetOutValue(LPC_GPIO_TypeDef* pGPIO, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    pGPIO->OUT = value;

}


/**
  * @brief       Set Value for bits that have output direction on GPIO port.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                              - 0: no effect
  *                              - 1: high level.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return                  None
  *
  * Note:
  * - For all bits that has been set as input direction, this function will
  * not effect.
  * - For all remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetHighLevel(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->SET, bitPosi, value);
}

/**
  * @brief    Clear Value for bits that have output direction on GPIO port.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                              - 0: No effect.
  *                              - 1: low level.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return  None
  *
  * Note:
  * - For all bits that has been set as input direction, this function will
  * not effect.
  * - For all remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetLowLevel(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->CLR, bitPosi, value);   
}

/**
  * @brief       Invert Value for bits that have output direction on GPIO port.
  * @param pGPIO      Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                              - 0: No effect.
  *                              - 1: inverted.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return                None
  *
  * Note:
  * - For all bits that has been set as input direction, this function will
  * not effect.
  * - For all remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetInvert(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->NOT, bitPosi, value);     
}

/**
  * @brief       Set Direction for GPIO port.
  * @param pGPIO    Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31, if it is oxff, set 'value'
  * @param value        if bitPosi is from 0 to 31, Direction value, should be:
  *                             - 0: Input.
  *                             - 1: Output.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return      None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_SetDir(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->DIR, bitPosi, value); 	  
 
}

/**
  * @brief       Get direction for a port pin.
  * @param pGPIO      Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, value is the 32bit value of register 
  * @return                status
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
uint32_t GPIO_GetDir( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    return ( gpio_getvalue((uint32_t *)&pGPIO->DIR, bitPosi) );
}



/**
  * @brief       Set interrupt type.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  * @param type          GPIO_interrupt_type definition
  * @return                  None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_IntSetType(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint8_t type)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));
    CHECK_PARAM(PARAM_GPIO_INTERRUPT(type));
    
    switch ( type )
    {
        case GPIO_INTERRUPT_FALLING:
            pGPIO->IS &= ~(0x1<<bitPosi);
            pGPIO->IBE &= ~(0x1<<bitPosi);
            pGPIO->IEV &= ~(0x1<<bitPosi);
        break;
        
        case GPIO_INTERRUPT_RISING:
            pGPIO->IS &= ~(0x1<<bitPosi);
            pGPIO->IBE &= ~(0x1<<bitPosi);
            pGPIO->IEV |= (0x1<<bitPosi);
        break;
        
        case GPIO_INTERRUPT_BOTH_EDGES:
            pGPIO->IS &= ~(0x1<<bitPosi);
            pGPIO->IBE |= (0x1<<bitPosi);
        break;
        
        case GPIO_INTERRUPT_LOW:
            pGPIO->IS |= (0x1<<bitPosi);
            pGPIO->IBE &= ~(0x1<<bitPosi);
            pGPIO->IEV &= ~(0x1<<bitPosi);
        break;
        
        case GPIO_INTERRUPT_HIGH:
            pGPIO->IS |= (0x1<<bitPosi);
            pGPIO->IBE &= ~(0x1<<bitPosi);
            pGPIO->IEV |= (0x1<<bitPosi);
        break;  
        
        default:
        break;
    }
}


/**
  * @brief       Mask interrupt for GPIO port.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, 'value' is set
  * @param value        Direction value, should be:
  *                              - 0: masked.
  *                              - 1: no mask.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return      None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_IntSetMask(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->IE, bitPosi, value);   
}

/**
  * @brief       Get Interrupt status for a port pin.
  * @param pGPIO      Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, value is the 32bit value of register 
  * @return                status
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
uint32_t GPIO_IntGetRawStatus( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    return ( gpio_getvalue((uint32_t *)&pGPIO->RIS,bitPosi) );
}

/**
  * @brief       Get Interrupt status for a port pin.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, value is the 32bit value of register 
  * @return                  status
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
uint32_t GPIO_IntGetMaskStatus( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi )
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    return ( gpio_getvalue((uint32_t *)&pGPIO->MIS,bitPosi) );
}

/**
  * @brief       Clear Interrupt for a port pin.
  * @param pGPIO       Port Number selection, should be LPC_GPIO0, LPC_GPIO1, LPC_GPIO2
  * @param bitPosi       bit position value, should be from 0 to 31
  *                              if it is oxff, 'value' is set
  * @param value        Direction value, should be:
  *                              - 0: no effect.
  *                              - 1: clear.
  *                             if bitPosi is oxff, set the all 32-bits of register
  * @return                  None
  *
  * Note: All remaining bits that are not activated in bitValue (value '0')
  * will not be effected by this function.
  */
void GPIO_IntClear(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, uint32_t value)
{
    CHECK_PARAM(PARAM_GPIOx(pGPIO));

    gpio_setvalue((uint32_t *)&pGPIO->IC, bitPosi, value); 
}


/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
