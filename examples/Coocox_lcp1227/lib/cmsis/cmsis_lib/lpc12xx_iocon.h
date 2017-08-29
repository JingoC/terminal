/**************************************************************************//**
 * $Id: lpc12xx_iocon.h 541 2010-09-15 09:04:20Z nxp21428 $
 *
 * @file     lpc12xx_iocon.h
 * @brief    Contains all macro definitions and function prototypes
 *              support for IOCON firmware library on lpc12xx.
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
/** @defgroup IOCON
 * @ingroup LPC1200CMSIS_FwLib_Drivers
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __LPC12xx_IOCON_H
#define __LPC12xx_IOCON_H

/* Includes ------------------------------------------------------------------- */
#include "lpc12xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types ------------------------------------------------------------- */
/** @defgroup IOCON_Public_Types
  * @{
  */

/**
  * @brief IOCON Config structure definition
  */

typedef struct {
    uint16_t    type;     /*!< low 8 bits is address offset, other is func >
                                                 This parameter can be a value of @ref PIO_type */
    uint8_t     pinmode;  /*!<  Pin mode >
                                                 This parameter can be a value of @ref PIO_pin_mode */
    uint8_t     invert;   /*!<  Inverted function >
                                                 This parameter can be a value of @ref Invert_input_mode */
    uint8_t     ad;       /*!<  analog/digital mode >
                                                 This parameter can be a value of @ref Analog_digital_mode */
    uint16_t    pmode;    /*!<  Push/pull mode for I2C >
                                                 This parameter can be a value of @ref Push_Pull_mode */
    uint16_t    od;       /*!<  Open drive >
                                                 This parameter can be a value of @ref Open_drain_mode */
    uint16_t    drive;    /*!<  Pin driver function >
                                                 This parameter can be a value of @ref Drive_current_mode */
    uint16_t    sm;       /*!<  Sample mode >
                                                 This parameter can be a value of @ref Sample_mode */
    uint32_t    cd;       /*!<  Clock Divider >
                                                 This parameter can be a value of @ref clock_divider_num */
} IOCON_PIO_CFG_Type;

/**
  * @}
  */

/* Public Macros -------------------------------------------------------------- */
/** @defgroup IOCON_Public_Macros
  * @{
  */

/**
  * @brief IOCON PIO_type definition
  * @{
  */
#define IOCON_PIO_0_0     ((uint16_t)0x0440 )  /* PIO port0 type definition */
#define IOCON_PIO_0_1     ((uint16_t)0x0480 )
#define IOCON_PIO_0_2     ((uint16_t)0x04C0 )
#define IOCON_PIO_0_3     ((uint16_t)0x0540 )
#define IOCON_PIO_0_4     ((uint16_t)0x0580 )
#define IOCON_PIO_0_5     ((uint16_t)0x05C0 )
#define IOCON_PIO_0_6     ((uint16_t)0x0600 )
#define IOCON_PIO_0_7     ((uint16_t)0x0640 )
#define IOCON_PIO_0_8     ((uint16_t)0x0680 )
#define IOCON_PIO_0_9     ((uint16_t)0x06C0 )
#define IOCON_PIO_0_10    ((uint16_t)0x0900 )
#define IOCON_PIO_0_11    ((uint16_t)0x0940 )
#define IOCON_PIO_0_12    ((uint16_t)0x0980 )
#define IOCON_PIO_0_13    ((uint16_t)0x09C1 )
#define IOCON_PIO_0_14    ((uint16_t)0x0A00 )
#define IOCON_PIO_0_15    ((uint16_t)0x0A40 )
#define IOCON_PIO_0_16    ((uint16_t)0x0A80 )
#define IOCON_PIO_0_17    ((uint16_t)0x0AC0 )
#define IOCON_PIO_0_18    ((uint16_t)0x0B00 )
#define IOCON_PIO_0_19    ((uint16_t)0x0080 )
#define IOCON_PIO_0_20    ((uint16_t)0x00C0 )
#define IOCON_PIO_0_21    ((uint16_t)0x0100 )
#define IOCON_PIO_0_22    ((uint16_t)0x0140 )
#define IOCON_PIO_0_23    ((uint16_t)0x0180 )
#define IOCON_PIO_0_24    ((uint16_t)0x01C0 )
#define IOCON_PIO_0_25    ((uint16_t)0x0206 )
#define IOCON_PIO_0_26    ((uint16_t)0x0246 )
#define IOCON_PIO_0_27    ((uint16_t)0x0280 )
#define IOCON_PIO_0_28    ((uint16_t)0x03C0 )
#define IOCON_PIO_0_29    ((uint16_t)0x0400 )
#define IOCON_PIO_0_30    ((uint16_t)0x0B41 )
#define IOCON_PIO_0_31    ((uint16_t)0x0B81 )
#define IOCON_PIO_1_0     ((uint16_t)0x0BC1 )   /* PIO port1 type definition */
#define IOCON_PIO_1_1     ((uint16_t)0x0C01 )
#define IOCON_PIO_1_2     ((uint16_t)0x0C40 )
#define IOCON_PIO_1_3     ((uint16_t)0x0C80 )
#define IOCON_PIO_1_4     ((uint16_t)0x0CC0 )
#define IOCON_PIO_1_5     ((uint16_t)0x0D00 )
#define IOCON_PIO_1_6     ((uint16_t)0x0D40 )
#define IOCON_PIO_2_0     ((uint16_t)0x0700 )   /* PIO port2 type definition */
#define IOCON_PIO_2_1     ((uint16_t)0x0740 )
#define IOCON_PIO_2_2     ((uint16_t)0x0780 )
#define IOCON_PIO_2_3     ((uint16_t)0x07C0 )
#define IOCON_PIO_2_4     ((uint16_t)0x0800 )
#define IOCON_PIO_2_5     ((uint16_t)0x0840 )
#define IOCON_PIO_2_6     ((uint16_t)0x0880 )
#define IOCON_PIO_2_7     ((uint16_t)0x08C0 )
#define IOCON_PIO_2_8     ((uint16_t)0x0E00 )
#define IOCON_PIO_2_9     ((uint16_t)0x0E40 )
#define IOCON_PIO_2_10    ((uint16_t)0x0E80 )
#define IOCON_PIO_2_11    ((uint16_t)0x0EC0 )
#define IOCON_PIO_2_12    ((uint16_t)0x02C0 )
#define IOCON_PIO_2_13    ((uint16_t)0x0300 )
#define IOCON_PIO_2_14    ((uint16_t)0x0340 )
#define IOCON_PIO_2_15    ((uint16_t)0x0380 )
#define IOCON_ACOMP_RST_LOC0   ((uint16_t)0x09C3 )/* Analog Comparator type definition */
#define IOCON_ACOMP_RST_LOC1   ((uint16_t)0x0CC2 )
#define IOCON_ACOMP_RST_LOC2   ((uint16_t)0x0EC4 )
#define IOCON_ACOMP_ROSC       ((uint16_t)0x0402 )
#define IOCON_ACOMP0_IN0       ((uint16_t)0x0082 )
#define IOCON_ACOMP0_IN1       ((uint16_t)0x00C2 )
#define IOCON_ACOMP0_IN2       ((uint16_t)0x0102 )
#define IOCON_ACOMP0_IN3       ((uint16_t)0x0142 )
#define IOCON_ACOMP1_IN0       ((uint16_t)0x0182 )
#define IOCON_ACOMP1_IN1       ((uint16_t)0x01C2 )
#define IOCON_ACOMP1_IN2       ((uint16_t)0x0202 )
#define IOCON_ACOMP1_IN3       ((uint16_t)0x0242 )
#define IOCON_ACOMP0_OUT       ((uint16_t)0x0282 )
#define IOCON_ACOMP1_OUT       ((uint16_t)0x03C2 )
#define IOCON_ADC_AD0       ((uint16_t)0x0B43 )   /* ADC type definition */
#define IOCON_ADC_AD1       ((uint16_t)0x0B83 )
#define IOCON_ADC_AD2       ((uint16_t)0x0BC2 )
#define IOCON_ADC_AD3       ((uint16_t)0x0C02 )
#define IOCON_ADC_AD4       ((uint16_t)0x0C42 )
#define IOCON_ADC_AD5       ((uint16_t)0x0C81 )
#define IOCON_ADC_AD6       ((uint16_t)0x0CC1 )
#define IOCON_ADC_AD7       ((uint16_t)0x0D01 )
#define IOCON_CT16_B0_CAP0_LOC0  ((uint16_t)0x0943 ) /* TIMER16 B0 type definition */
#define IOCON_CT16_B0_CAP1_LOC0  ((uint16_t)0x0983 )
#define IOCON_CT16_B0_MAT0_LOC0  ((uint16_t)0x0944 )
#define IOCON_CT16_B0_MAT1_LOC0  ((uint16_t)0x0984 )
#define IOCON_CT16_B0_RST_LOC0   ((uint16_t)0x0205 )
#define IOCON_CT16_B0_CAP0_LOC1  ((uint16_t)0x03C3 )
#define IOCON_CT16_B0_CAP1_LOC1  ((uint16_t)0x0403 )
#define IOCON_CT16_B0_MAT0_LOC1  ((uint16_t)0x03C4 )
#define IOCON_CT16_B0_MAT1_LOC1  ((uint16_t)0x0404 )
#define IOCON_CT16_B0_RST_LOC1   ((uint16_t)0x0343 )
#define IOCON_CT16_B0_CAP0_LOC2  ((uint16_t)0x0702 )
#define IOCON_CT16_B0_CAP1_LOC2  ((uint16_t)0x0742 )
#define IOCON_CT16_B0_MAT0_LOC2  ((uint16_t)0x0703 )
#define IOCON_CT16_B0_MAT1_LOC2  ((uint16_t)0x0743 )
#define IOCON_CT16_B1_CAP0_LOC0  ((uint16_t)0x0A43 ) /* TIMER16 B1 type definition */
#define IOCON_CT16_B1_CAP1_LOC0  ((uint16_t)0x0A83 )
#define IOCON_CT16_B1_MAT0_LOC0  ((uint16_t)0x0A44 )
#define IOCON_CT16_B1_MAT1_LOC0  ((uint16_t)0x0A84 )
#define IOCON_CT16_B1_RST_LOC0   ((uint16_t)0x0245 )
#define IOCON_CT16_B1_CAP0_LOC1  ((uint16_t)0x0D02 )
#define IOCON_CT16_B1_CAP1_LOC1  ((uint16_t)0x0D41 )
#define IOCON_CT16_B1_MAT0_LOC1  ((uint16_t)0x0D03 )
#define IOCON_CT16_B1_MAT1_LOC1  ((uint16_t)0x0D42 )
#define IOCON_CT16_B1_RST_LOC1   ((uint16_t)0x0383 )
#define IOCON_CT16_B1_CAP0_LOC2  ((uint16_t)0x0782 )
#define IOCON_CT16_B1_CAP1_LOC2  ((uint16_t)0x07C2 )
#define IOCON_CT16_B1_MAT0_LOC2  ((uint16_t)0x0783 )
#define IOCON_CT16_B1_MAT1_LOC2  ((uint16_t)0x07C3 )
#define IOCON_CT32_B0_CAP0_LOC0  ((uint16_t)0x0483 ) /* TIMER32 B0 type definition */
#define IOCON_CT32_B0_CAP1_LOC0  ((uint16_t)0x04C3 )
#define IOCON_CT32_B0_CAP2_LOC0  ((uint16_t)0x0543 )
#define IOCON_CT32_B0_CAP3_LOC0  ((uint16_t)0x0583 )
#define IOCON_CT32_B0_MAT0_LOC0  ((uint16_t)0x0484 )
#define IOCON_CT32_B0_MAT1_LOC0  ((uint16_t)0x04C4 )
#define IOCON_CT32_B0_MAT2_LOC0  ((uint16_t)0x0544 )
#define IOCON_CT32_B0_MAT3_LOC0  ((uint16_t)0x0584 )
#define IOCON_CT32_B0_RST_LOC0   ((uint16_t)0x0185 )
#define IOCON_CT32_B0_CAP0_LOC1  ((uint16_t)0x0B03 )
#define IOCON_CT32_B0_CAP1_LOC1  ((uint16_t)0x0083 )
#define IOCON_CT32_B0_CAP2_LOC1  ((uint16_t)0x00C3 )
#define IOCON_CT32_B0_CAP3_LOC1  ((uint16_t)0x0103 )
#define IOCON_CT32_B0_MAT0_LOC1  ((uint16_t)0x0B04 )
#define IOCON_CT32_B0_MAT1_LOC1  ((uint16_t)0x0084 )
#define IOCON_CT32_B0_MAT2_LOC1  ((uint16_t)0x00C4 )
#define IOCON_CT32_B0_MAT3_LOC1  ((uint16_t)0x0104 )
#define IOCON_CT32_B0_RST_LOC1   ((uint16_t)0x02C2 )
#define IOCON_CT32_B0_CAP0_LOC2  ((uint16_t)0x0802 )
#define IOCON_CT32_B0_CAP1_LOC2  ((uint16_t)0x0842 )
#define IOCON_CT32_B0_CAP2_LOC2  ((uint16_t)0x0882 )
#define IOCON_CT32_B0_CAP3_LOC2  ((uint16_t)0x08C2 )
#define IOCON_CT32_B0_MAT0_LOC2  ((uint16_t)0x0803 )
#define IOCON_CT32_B0_MAT1_LOC2  ((uint16_t)0x0843 )
#define IOCON_CT32_B0_MAT2_LOC2  ((uint16_t)0x0883 )
#define IOCON_CT32_B0_MAT3_LOC2  ((uint16_t)0x08C3 )
#define IOCON_CT32_B1_CAP0_LOC0  ((uint16_t)0x0603 ) /* TIMER32 B1 type definition */
#define IOCON_CT32_B1_CAP1_LOC0  ((uint16_t)0x0643 )
#define IOCON_CT32_B1_CAP2_LOC0  ((uint16_t)0x0683 )
#define IOCON_CT32_B1_CAP3_LOC0  ((uint16_t)0x06C3 )
#define IOCON_CT32_B1_MAT0_LOC0  ((uint16_t)0x0604 )
#define IOCON_CT32_B1_MAT1_LOC0  ((uint16_t)0x0644 )
#define IOCON_CT32_B1_MAT2_LOC0  ((uint16_t)0x0684 )
#define IOCON_CT32_B1_MAT3_LOC0  ((uint16_t)0x06c4 )
#define IOCON_CT32_B1_RST_LOC0   ((uint16_t)0x01C5 )
#define IOCON_CT32_B1_CAP0_LOC1  ((uint16_t)0x0183 )
#define IOCON_CT32_B1_CAP1_LOC1  ((uint16_t)0x01C3 )
#define IOCON_CT32_B1_CAP2_LOC1  ((uint16_t)0x0203 )
#define IOCON_CT32_B1_CAP3_LOC1  ((uint16_t)0x0243 )
#define IOCON_CT32_B1_MAT0_LOC1  ((uint16_t)0x0184 )
#define IOCON_CT32_B1_MAT1_LOC1  ((uint16_t)0x01C4 )
#define IOCON_CT32_B1_MAT2_LOC1  ((uint16_t)0x0204 )
#define IOCON_CT32_B1_MAT3_LOC1  ((uint16_t)0x0244 )
#define IOCON_CT32_B1_RST_LOC1   ((uint16_t)0x0302 )
#define IOCON_CT32_B1_CAP0_LOC2  ((uint16_t)0x0E02 )
#define IOCON_CT32_B1_CAP1_LOC2  ((uint16_t)0x0E42 )
#define IOCON_CT32_B1_CAP2_LOC2  ((uint16_t)0x0E82 )
#define IOCON_CT32_B1_CAP3_LOC2  ((uint16_t)0x0EC2 )
#define IOCON_CT32_B1_MAT0_LOC2  ((uint16_t)0x0E03 )
#define IOCON_CT32_B1_MAT1_LOC2  ((uint16_t)0x0E43 )
#define IOCON_CT32_B1_MAT2_LOC2  ((uint16_t)0x0E83 )
#define IOCON_CT32_B1_MAT3_LOC2  ((uint16_t)0x0EC3 )
#define IOCON_UART_RTS0_LOC0  ((uint16_t)0x0442 )  /* UART0 loc0 type definition */
#define IOCON_UART_RXD0_LOC0  ((uint16_t)0x0482 )
#define IOCON_UART_TXD0_LOC0  ((uint16_t)0x04C2 )
#define IOCON_UART_DTR0_LOC0  ((uint16_t)0x0542 )
#define IOCON_UART_DSR0_LOC0  ((uint16_t)0x0582 )
#define IOCON_UART_DCD0_LOC0  ((uint16_t)0x05C2 )
#define IOCON_UART_RI0_LOC0   ((uint16_t)0x0602 )
#define IOCON_UART_CTS0_LOC0  ((uint16_t)0x0642 )
#define IOCON_UART_RTS0_LOC1  ((uint16_t)0x0704 )   /* UART0 loc1 type definition */
#define IOCON_UART_RXD0_LOC1  ((uint16_t)0x0744 )
#define IOCON_UART_TXD0_LOC1  ((uint16_t)0x0784 )
#define IOCON_UART_DTR0_LOC1  ((uint16_t)0x07C4 )
#define IOCON_UART_DSR0_LOC1  ((uint16_t)0x08C4 )
#define IOCON_UART_DCD0_LOC1  ((uint16_t)0x0884 )
#define IOCON_UART_RI0_LOC1   ((uint16_t)0x0844 )
#define IOCON_UART_CTS0_LOC1  ((uint16_t)0x0804 )
#define IOCON_UART_RXD1_LOC0  ((uint16_t)0x0682 )  /* UART1 type definition */
#define IOCON_UART_TXD1_LOC0  ((uint16_t)0x06C2 )
#define IOCON_UART_RXD1_LOC1  ((uint16_t)0x0E85 )
#define IOCON_UART_TXD1_LOC1  ((uint16_t)0x0EC5 )
#define IOCON_UART_RXD1_LOC2  ((uint16_t)0x02C3 )
#define IOCON_UART_TXD1_LOC2  ((uint16_t)0x0303 )
#define IOCON_SSP_SCK   ((uint16_t)0x0A02 )        /* SSP type definition */
#define IOCON_SSP_SSEL  ((uint16_t)0x0A42 )
#define IOCON_SSP_MISO  ((uint16_t)0x0A82 )
#define IOCON_SSP_MOSI  ((uint16_t)0x0AC2 )
#define IOCON_I2C_SCL  ((uint16_t)0x0902  )         /* l2C type definition */
#define IOCON_I2C_SDA  ((uint16_t)0x0942  )
#define IOCON_SWD_SWCLK_LOC0   ((uint16_t)0x0240 )  /* SWD type definition */
#define IOCON_SWD_SWDIO_LOC0   ((uint16_t)0x0200 )
#define IOCON_SWD_SWCLK_LOC1   ((uint16_t)0x0B01 )
#define IOCON_SWD_SWDIO_LOC1   ((uint16_t)0x0C41 )
#define IOCON_RESET_PIN  ((uint16_t)0x09C0  )       /* RESET type definition */
#define IOCON_CLOCKOUT_PIN  ((uint16_t)0x0982  )    /* Clock out type definition */

#define PARAM_IOCON_PIO_TYPE(type)  (((type >> 4) <= 0xEC) &&\
                                     ((type & 0xF) <= 0x6))

/**
  * @}
  */

/**
  * @brief IOCON PIO_pin_mode definition
  * @{
  */
#define IOCON_PIO_MODE_INACTIVE  ((uint8_t)0x00  )   /* no pull resistor enabled */
#define IOCON_PIO_MODE_PULLUP    ((uint8_t)0x10  )   /* Pull-up resistor enabled */

#define PARAM_IOCON_PIO_MODE(mode)  ((  mode == IOCON_PIO_MODE_INACTIVE)|| \
                                     (  mode == IOCON_PIO_MODE_PULLUP))

/**
  * @}
  */


/**
  * @brief IOCON Invert_input_mode definition
  * @{
  */
#define IOCON_PIO_INV_NOT       ((uint8_t)0x00  )
#define IOCON_PIO_INV_INVERTED  ((uint8_t)0x40  )

#define PARAM_IOCON_PIO_INV(loc)   ((  loc == IOCON_PIO_INV_NOT)|| \
                                    (  loc == IOCON_PIO_INV_INVERTED))

/**
  * @}
  */



/**
  * @brief IOCON Analog_digital_mode definition
  * @{
  */
#define IOCON_PIO_AD_ANALOG       ((uint8_t)0x00  )
#define IOCON_PIO_AD_DIGITAL      ((uint8_t)0x80  )

#define PARAM_IOCON_PIO_AD(loc)   ((  loc == IOCON_PIO_AD_ANALOG)|| \
                                   (  loc == IOCON_PIO_AD_DIGITAL))
/**
  * @}
  */

/**
  * @brief IOCON PIO Push_Pull_mode definition
  * @{
  */
#define IOCON_PIO_PMODE_DISABLE  ((uint16_t)0x0000  )   /* push/pull disabled */
#define IOCON_PIO_PMODE_ENABLE   ((uint16_t)0x0100  )   /* Push/pull enabled */

#define PARAM_IOCON_PIO_PMODE(mode)  ((  mode == IOCON_PIO_PMODE_DISABLE)|| \
                                      (  mode == IOCON_PIO_PMODE_ENABLE))
/**
  * @}
  */

/**
  * @brief IOCON PIO Open_drain_mode definition
  * @{
  */
#define IOCON_PIO_OD_DISABLE  ((uint16_t)0x0000  )
#define IOCON_PIO_OD_ENABLE   ((uint16_t)0x0400  )

#define PARAM_IOCON_PIO_OD(loc)   ((  loc == IOCON_PIO_OD_DISABLE)|| \
                                   (  loc == IOCON_PIO_OD_ENABLE))
/**
  * @}
  */

/**
  * @brief IOCON PIO Drive_current_mode definition
  * @{
  */
#define IOCON_PIO_DRV_2MA_12MA    ((uint16_t)0x0000  )   /* Drive current */
#define IOCON_PIO_DRV_4MA_16MA    ((uint16_t)0x1000  )   /* Drive current */

#define PARAM_IOCON_PIO_DRV(hys)    ((  hys == IOCON_PIO_DRV_2MA_12MA)|| \
                                     (  hys == IOCON_PIO_DRV_4MA_16MA))
/**
  * @}
  */

/**
  * @brief IOCON Sample_mode definition
  * @{
  */
#define IOCON_PIO_SMODE_BYPASS          ((uint16_t)0x0000  )
#define IOCON_PIO_SMODE_ONE_CLOCK       ((uint16_t)0x2000  )
#define IOCON_PIO_SMODE_TWO_CLOCK       ((uint16_t)0x4000  )
#define IOCON_PIO_SMODE_THREE_CLOCK     ((uint16_t)0x6000  )

#define PARAM_IOCON_PIO_SMODE(loc)   ((  loc == IOCON_PIO_SMODE_BYPASS)|| \
                                      (  loc == IOCON_PIO_SMODE_ONE_CLOCK)|| \
                                      (  loc == IOCON_PIO_SMODE_TWO_CLOCK)|| \
                                      (  loc == IOCON_PIO_SMODE_THREE_CLOCK))
/**
  * @}
  */

/**
  * @brief IOCON clock_divider_num definition
  * @{
  */
#define IOCON_PIO_CLKDIV_0       ((uint32_t)0x00000  )
#define IOCON_PIO_CLKDIV_1       ((uint32_t)0x08000  )
#define IOCON_PIO_CLKDIV_2       ((uint32_t)0x10000  )
#define IOCON_PIO_CLKDIV_3       ((uint32_t)0x18000  )
#define IOCON_PIO_CLKDIV_4       ((uint32_t)0x20000  )
#define IOCON_PIO_CLKDIV_5       ((uint32_t)0x28000  )
#define IOCON_PIO_CLKDIV_6       ((uint32_t)0x30000  )

#define PARAM_IOCON_PIO_CLKDIV(loc)  ((  loc == IOCON_PIO_CLKDIV_0)|| \
                                      (  loc == IOCON_PIO_CLKDIV_1)|| \
                                      (  loc == IOCON_PIO_CLKDIV_2)|| \
                                      (  loc == IOCON_PIO_CLKDIV_3)|| \
                                      (  loc == IOCON_PIO_CLKDIV_4)|| \
                                      (  loc == IOCON_PIO_CLKDIV_5)|| \
                                      (  loc == IOCON_PIO_CLKDIV_6))

/**
  * @}
  */

/**
  * @}
  */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup IOCON_Public_Functions
  * @{
  */
void IOCON_SetFunc ( IOCON_PIO_CFG_Type *mode);
void IOCON_StructInit ( IOCON_PIO_CFG_Type *mode);


/**
  * @}
  */


#ifdef __cplusplus
}
#endif
#endif /* __LPC12xx_IOCON_H */


/**
 * @}
 */


/* --------------------------------- End Of File ------------------------------ */

