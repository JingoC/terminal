/**************************************************************************//**
 * $Id: lpc12xx_uart.h 546 2010-09-16 03:26:59Z cnh20509 $
 *
 * @file     lpc12xx_uart.h
 * @brief    Contains all macro definitions and function prototypes
 *              support for UART firmware library on lpc12xx.
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
/** @defgroup UART
 * @ingroup LPC1200CMSIS_FwLib_Drivers 
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __LPC12xx_UART_H
#define __LPC12xx_UART_H

/* Includes ------------------------------------------------------------------- */
#include "LPC12xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* -------------Public Types -------------------------------------------------- */
/** @defgroup UART_Public_Types
 * @{
 */

/**
  * @brief UART Configuration structure definition
  */

typedef struct {
    uint32_t    baudrate;  /* !< UART baud rate */
    uint8_t     databits;  /* !< Number of data bits 
                                            This parameter can be a value of @ref UART_databit_type */
    uint8_t     stopbits;  /* !< Number of stop bits  
                                            This parameter can be a value of @ref UART_stopbit_type */
    uint8_t     parity;    /* !< Parity selection  
                                            This parameter can be a value of @ref UART_parity_type */
    uint8_t     fifolevel; /* !< Rx FIFO trigger level  
                                            This parameter can be a value of @ref FIFO_level_type */  
    uint8_t     fifodma;   /* !< DMA mode  
                                            This parameter can be a value of @ref FIFO_dmamode_type */
    uint8_t     txdbreak;  /* !< enable: TXD forceed to logic 0 
                                            This parameter can be a value of @ref TXD_break_forced_type */  
} UART_CFG_Type;

/**
  * @brief UART MODEM Configuration structure definition
  */
typedef struct {
    uint8_t   DTRcontrol;  /* !< modem output pin DTR  
                                            This parameter can be a value of @ref UART_modem_DTR_pin_type */
    uint8_t   RTScontrol;  /* !< modem output pin ~RTS  
                                            This parameter can be a value of @ref UART_modem_RTS_pin_type */
    uint8_t   loopback;    /* !< modem loopback mode  
                                            This parameter can be a value of @ref UART_modem_loopback_type */
    uint8_t   RTSen;       /* !< modem RTS control  
                                            This parameter can be a value of @ref UART_modem_autoRTS_type */
    uint8_t   CTSen;       /* !< modem CTS control  
                                            This parameter can be a value of @ref UART_modem_autoCTS_type */
} UART_MODEM_CFG_Type;

/**
  * @brief UART 485 Configuration structure definition
  */
typedef struct {
    uint8_t    nmmen;      /* !< Normal MultiDrop mode State   
                                            This parameter can be a value of @ref UART_485_nmm_type */
    uint8_t    rxdis;      /* !< Receiver State   
                                            This parameter can be a value of @ref UART_485_receiver_type */
    uint8_t    adden;      /* !< Auto Address Detect mode state   
                                            This parameter can be a value of @ref UART_485_add_type */
    uint8_t    dirpinsel;  /* !< Auto Direction Control State  
                                            This parameter can be a value of @ref UART_485_dirpin_ctrl_type */
    uint8_t    autodiren;  /* !< If direction control is enabled, state  
                                            This parameter can be a value of @ref UART_485_autodir_type */
    uint8_t    pinlevel;   /* !< Polarity of the direction control   
                                            This parameter can be a value of @ref UART_485_dirpin_level_type */
    uint8_t    addrress;   /* !< Address match value  */
    uint8_t    delay;      /* !< Delay time  */
} UART_RS485_CFG_Type;

/**
  * @brief UART IRDA Configuration structure definition
  */
typedef struct {
    uint8_t    mode;    /* !< Normal MultiDrop mode State   
                                       This parameter can be a value of @ref UART_irda_mode_type */
    uint8_t    invert;  /* !< Receiver State   
                                       This parameter can be a value of @ref UART_irda_input_invert_type */
    uint8_t    pulse;   /* !< Auto Address Detect mode state   
                                       This parameter can be a value of @ref UART_irda_fixed_pulse_type */
} UART_IRDA_CFG_Type;

/**
  * @}
  */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup UART_Public_Functions
 * @{
 */
void UART_Init(LPC_UART_TypeDef *UARTx);
void UART_SetConfig(LPC_UART_TypeDef *UARTx, UART_CFG_Type *config);
void UART_GetConfig(LPC_UART_TypeDef *UARTx, UART_CFG_Type *config);
void UART_ConfigTXD(LPC_UART_TypeDef *UARTx, FunctionalState NewState);
uint32_t UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, \
                       uint32_t buflen, uint32_t timeout);
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, \
                          uint32_t buflen, uint32_t timeout);

uint8_t UART_GetLineStatus(LPC_UART_TypeDef* UARTx);
#ifdef _UART_INT
void UART_ConfigInts(LPC_UART_TypeDef *UARTx, uint16_t UARTIntCfg,\
                         FunctionalState NewState);
uint16_t UART_GetINTStatus(LPC_UART_TypeDef* UARTx);
void UART_StartAutoBaud(LPC_UART_TypeDef *UARTx, uint8_t mode, uint8_t restart);
void UART_ClearAutoBaud(LPC_UART_TypeDef *UARTx);

#endif
#ifdef _MODEM
void UART_SetConfigModem(LPC_UART_TypeDef *UARTx, UART_MODEM_CFG_Type *config);
void UART_GetConfigModem(LPC_UART_TypeDef *UARTx, UART_MODEM_CFG_Type *config);
uint8_t UART_GetModemStatus(LPC_UART_TypeDef *UARTx);
#endif
#ifdef _RS485
void UART_SetConfigRS485(LPC_UART_TypeDef *UARTx, UART_RS485_CFG_Type *config);
void UART_GetConfigRS485(LPC_UART_TypeDef *UARTx, UART_RS485_CFG_Type *config);
#endif

void UART_SetConfigIRDA(LPC_UART_TypeDef *UARTx, UART_IRDA_CFG_Type *config);
void UART_GetConfigIRDA(LPC_UART_TypeDef *UARTx, UART_IRDA_CFG_Type *config);

/**
  * @}
  */

/* Public Macros -------------------------------------------------------------- */
/** @defgroup UART_Public_Macros
  * @{
  */

/* @brief UART_databit_type definitions  
  * @{
  */
#define UART_CFG_DATABIT_5   ((uint8_t)(0))       /* UART 5 bit data mode */
#define UART_CFG_DATABIT_6   ((uint8_t)(1<<0))    /* UART 6 bit data mode */
#define UART_CFG_DATABIT_7   ((uint8_t)(2<<0))    /* UART 7 bit data mode */
#define UART_CFG_DATABIT_8   ((uint8_t)(3<<0))    /* UART 8 bit data mode */
#define PARAM_UART_DATABIT(databit) ((databit==UART_CFG_DATABIT_5) || \
                                     (databit==UART_CFG_DATABIT_6) || \
                                     (databit==UART_CFG_DATABIT_7) || \
                                     (databit==UART_CFG_DATABIT_8))
/**
  * @}
  */   
  
/*@brief UART_stopbit_type definitions 
  * @{
  */
#define UART_CFG_STOPBIT_1   ((uint8_t)(0))      /* UART 1 Stop Bits Select */
#define UART_CFG_STOPBIT_2   ((uint8_t)(1<<2))   /* UART Two Stop Bits Select */
#define PARAM_UART_STOPBIT(stopbit) ((stopbit==UART_CFG_STOPBIT_1) || \
                                     (stopbit==UART_CFG_STOPBIT_2))
/**
  * @}
  */

/* @brief UART_parity_type definitions 
  * @{
  */
#define UART_CFG_PARITY_NONE   ((uint8_t)(0))     /* No parity */
#define UART_CFG_PARITY_ODD    ((uint8_t)(1<<3))  /* Odd parity */
#define UART_CFG_PARITY_EVEN   ((uint8_t)(3<<3))  /* Even parity */
#define UART_CFG_PARITY_MARK   ((uint8_t)(5<<3))  /* Forced "1" stick parity */
#define UART_CFG_PARITY_SPACE  ((uint8_t)(7<<3))  /* Forced "0" stick parity */
#define PARAM_UART_PARITY(parity)   ((parity==UART_CFG_PARITY_NONE) || \
                                     (parity==UART_CFG_PARITY_ODD)  || \
                                     (parity==UART_CFG_PARITY_EVEN) || \
                                     (parity==UART_CFG_PARITY_MARK) || \
                                     (parity==UART_CFG_PARITY_SPACE))
/**
  * @}
  */

/* @brief FIFO_level_type definitions 
  * @{
  */
#define UART_CFG_FIFOTRG_1   ((uint8_t)(0))    /* UART FIFO trigger level 0: 1 char */
#define UART_CFG_FIFOTRG_4   ((uint8_t)(1<<6)) /* UART FIFO trigger level 1: 4 char */
#define UART_CFG_FIFOTRG_8   ((uint8_t)(2<<6)) /* UART FIFO trigger level 2: 8 char */
#define UART_CFG_FIFOTRG_14  ((uint8_t)(3<<6)) /* UART FIFO trigger level 3: 14 char */
#define PARAM_UART_FIFO_LEVEL(fifo) ((fifo==UART_CFG_FIFOTRG_1) || \
                                     (fifo==UART_CFG_FIFOTRG_4) || \
                                     (fifo==UART_CFG_FIFOTRG_8) || \
                                     (fifo==UART_CFG_FIFOTRG_14))
/**
  * @}
  */

/*@brief FIFO_dmamode_type definitions  
  * @{
  */
#define UART_CFG_DMAMODE_DISABLE  ((uint8_t)(0))     /* UART DMA mode disable */
#define UART_CFG_DMAMODE_ENABLE   ((uint8_t)(1<<3))  /* UART DMA mode enable  */
#define PARAM_UART_FIFO_DMA(fifo)   ((fifo==UART_CFG_DMAMODE_DISABLE) ||\
                                     (fifo==UART_CFG_DMAMODE_ENABLE))
/**
  * @}
  */
                                     
/*@brief TXD_break_forced_type definitions   
  * @{
  */
#define UART_CFG_TXDBREAK_DISABLE  ((uint8_t)(0))      /* TXD Break disable */
#define UART_CFG_TXDBREAK_ENABLE   ((uint8_t)(1<<6))   /* TXD Break enable */
#define PARAM_UART_TXDBREAK(type)   ((type==UART_CFG_TXDBREAK_DISABLE) ||\
                                     (type==UART_CFG_TXDBREAK_ENABLE))
/**
  * @}
  */

/*@brief UART_interrupt_type definitions   
  * @{
  */
#define UART_INTCFG_RBR  ((uint16_t)(1<<0)) /* RBR Interrupt enable*/
#define UART_INTCFG_THRE ((uint16_t)(1<<1)) /* THR Interrupt enable*/
#define UART_INTCFG_RLS  ((uint16_t)(1<<2)) /* RX line status interrupt enable*/
#define UART_MSINT_EN    ((uint32_t)(1<<3)) /*!< Modem status interrupt enable */
#define UART_CTSINT_EN   ((uint32_t)(1<<7)) /*!< CTS1 signal transition interrupt enable */
#define UART_INTCFG_ABEO ((uint16_t)(1<<8)) /* the end of auto-baud interrupt */
#define UART_INTCFG_ABTO ((uint16_t)(1<<9)) /* the auto-baud time-out interrupt */
#define PARAM_UART_INTCFG(intcfg)   ((intcfg == UART_INTCFG_RBR)  ||\
                                     (intcfg == UART_INTCFG_THRE) ||\
                                     (intcfg == UART_INTCFG_RLS)  ||\
                                     (intcfg == UART_MSINT_EN)    ||\
                                     (intcfg == UART_CTSINT_EN)   ||\
                                     (intcfg == UART_INTCFG_ABEO) ||\
                                     (intcfg == UART_INTCFG_ABTO))
/**
  * @}
  */
  
/*@brief UART_int_status_type definitions   
  * @{
  */

#define UART_INTSTAT_PEND              ((uint16_t)(1<<0))  /* Active low */
#define UART_INTSTAT_RX_LINE_STAT      ((uint16_t)(3<<1))  /* Receive line status*/
#define UART_INTSTAT_RX_DATA_AVAILABLE ((uint16_t)(2<<1))  /* Receive data available*/
#define UART_INTSTAT_RX_TIMEOUT        ((uint16_t)(6<<1))  /* Character time-out indicator*/
#define UART_INTSTAT_TX_READY          ((uint16_t)(1<<1))  /* THRE interrupt*/
#define UART_INTSTAT_MODEM             ((uint16_t)(0<<1))  /* Modem interrupt*/
#define UART_INTSTAT_AUTO_END          ((uint16_t)(1<<8))  /* End of auto-baud interrupt */
#define UART_INTSTAT_AUTO_TIMOUT       ((uint16_t)(1<<9))  /* Autobaud time-out interrupt */

/**
  * @}
  */
  
/*@brief UART_line_status_type definitions   
  * @{
  */
#define UART_LS_RX_DATA_READY ((uint8_t)(1<<0)) /*Receive data ready*/
#define UART_LS_RX_FIFO_FULL  ((uint8_t)(1<<1)) /*Overrun error: RX fifo overflow*/
#define UART_LS_PARITY_ERROR  ((uint8_t)(1<<2)) /*Parity error*/
#define UART_LS_FRAMING_ERROR ((uint8_t)(1<<3)) /*Framing error*/
#define UART_LS_BREAK_INT     ((uint8_t)(1<<4)) /*Break interrupt*/
#define UART_LS_TX_EMPTY      ((uint8_t)(1<<5)) /*Transmit holding register THR empty*/
#define UART_LS_TX_EMPTY_ALL  ((uint8_t)(1<<6)) /*Transmitter THR and TSR all empty*/
#define UART_LS_RX_FIFO_ERROR ((uint8_t)(1<<7)) /*Error in RX FIFO or others*/

/**
  * @}
  */
  
/*@brief UART_auto_baudrate_mode_type definitions   
  * @{
  */
#define UART_CFG_AUTOBAUD_MODE0  ((uint8_t)(0))     /* UART Auto baudrate Mode 0 */
#define UART_CFG_AUTOBAUD_MODE1  ((uint8_t)(1<<1)) /* UART Auto baudrate Mode 1 */

#define PARAM_UART_AB_MODE(ABmode) ((ABmode==UART_CFG_AUTOBAUD_MODE0) ||\
                                    (ABmode==UART_CFG_AUTOBAUD_MODE1))

/**
  * @}
  */
  
/*@brief UART_auto_baudrate_restart_type definitions   
  * @{
  */
#define UART_CFG_ABRESTART_DISABLE  ((uint8_t)(0))     /* Auto baudrate disable */
#define UART_CFG_ABRESTART_ENABLE   ((uint8_t)(1<<2)) /* Auto baudrate enable */

#define PARAM_UART_AB_RESTART(type) ((type==UART_CFG_ABRESTART_DISABLE) ||\
                                     (type==UART_CFG_ABRESTART_ENABLE))


/**
  * @}
  */
  
/*@brief UART_modem_status_type definitions   
  * @{
  */
#define UART_MODEM_STAT_DELTA_CTS  ((uint8_t)(1<<0))   /* State change of CTS */
#define UART_MODEM_STAT_DELTA_DSR  ((uint8_t)(1<<1))   /* State change of DSR */
#define UART_MODEM_STAT_LO2HI_RI   ((uint8_t)(1<<2))   /* Low to high transition of RI */
#define UART_MODEM_STAT_DELTA_DCD  ((uint8_t)(1<<3))   /* State change of input DCD */
#define UART_MODEM_STAT_CTS        ((uint8_t)(1<<4))   /* Clear To Send State */
#define UART_MODEM_STAT_DSR        ((uint8_t)(1<<5))   /* Data Set Ready State */
#define UART_MODEM_STAT_RI         ((uint8_t)(1<<6))   /* Ring Indicator State */
#define UART_MODEM_STAT_DCD        ((uint8_t)(1<<7))   /* Data Carrier Detect State */

/**
  * @}
  */
  
/*@brief UART_modem_DTR_pin_type definitions   
  * @{
  */
#define UART_MODEM_PIN_DTR_DISABLE ((uint8_t)(0)) 
#define UART_MODEM_PIN_DTR_ENABLE  ((uint8_t)(1<<0)) /* Modem output pin DTR */
#define PARAM_MODEM_PIN_DTR(x) ((x==UART_MODEM_PIN_DTR_DISABLE) || \
                                (x==UART_MODEM_PIN_DTR_ENABLE))

/**
  * @}
  */
  
/*@brief UART_modem_RTS_pin_type definitions   
  * @{
  */

#define UART_MODEM_PIN_RTS_DISABLE ((uint8_t)(0)) 
#define UART_MODEM_PIN_RTS_ENABLE  ((uint8_t)(1<<1)) /* Modem output pin RTS */  
#define PARAM_MODEM_PIN_RTS(x) ((x==UART_MODEM_PIN_RTS_DISABLE) || \
                                (x==UART_MODEM_PIN_RTS_ENABLE))

/**
  * @}
  */
  
/*@brief UART_modem_loopback_type definitions   
  * @{
  */

#define UART_MODEM_MODE_LOOPBACK_DISABLE ((uint8_t)(0)) 
#define UART_MODEM_MODE_LOOPBACK_ENABLE  ((uint8_t)(1<<4))  /* Loop back mode select */ 
#define PARAM_MODEM_MODE_LOOPBACK(x) ((x==UART_MODEM_MODE_LOOPBACK_DISABLE) || \
                                      (x==UART_MODEM_MODE_LOOPBACK_ENABLE))

/**
  * @}
  */
  
/*@brief UART_modem_autoRTS_type definitions   
  * @{
  */
#define UART_MODEM_MODE_AUTO_RTS_DISABLE ((uint8_t)(0))
#define UART_MODEM_MODE_AUTO_RTS_ENABLE  ((uint8_t)(1<<6)) /* Enable AutoRTS flow-control*/
#define PARAM_MODEM_MODE_AUTO_RTS(x) ((x==UART_MODEM_MODE_AUTO_RTS_DISABLE) || \
                                      (x==UART_MODEM_MODE_AUTO_RTS_ENABLE))

/**
  * @}
  */
  
/*@brief UART_modem_autoCTS_type definitions   
  * @{
  */
#define UART_MODEM_MODE_AUTO_CTS_DISABLE ((uint8_t)(0))
#define UART_MODEM_MODE_AUTO_CTS_ENABLE  ((uint8_t)(1<<7)) /* Enable AutoCTS flow-control*/
#define PARAM_MODEM_MODE_AUTO_CTS(x) ((x==UART_MODEM_MODE_AUTO_CTS_DISABLE) || \
                                      (x==UART_MODEM_MODE_AUTO_CTS_ENABLE))

/**
  * @}
  */
  
/*@brief UART_485_nmm_type definitions   
  * @{
  */
#define UART_RS485_NORMAL_MULTIDROP_DISABLE ((uint8_t)(0))
#define UART_RS485_NORMAL_MULTIDROP_ENABLE  ((uint8_t)(1<<0))  /* Normal Multi-drop Mode */ 
#define PARAM_UART_RS485_NORMAL_MULTIDROP(x) ((x==UART_RS485_NORMAL_MULTIDROP_DISABLE) || \
                                             (x==UART_RS485_NORMAL_MULTIDROP_ENABLE))

/**
  * @}
  */
  
/*@brief UART_485_receiver_type definitions   
  * @{
  */
#define UART_RS485_RECEIVER_ENABLE   ((uint8_t)(0))
#define UART_RS485_RECEIVER_DISABLE  ((uint8_t)(1<<1))  /* The receiver is disabled */
#define PARAM_UART_RS485_RECEIVER(x) ((x==UART_RS485_RECEIVER_ENABLE) || \
                                 (x==UART_RS485_RECEIVER_DISABLE))

/**
  * @}
  */
  
/*@brief UART_485_add_type definitions   
  * @{
  */

#define UART_RS485_AUTO_ADDRRESS_DISABLE ((uint8_t)(0)) 
#define UART_RS485_AUTO_ADDRRESS_ENABLE  ((uint8_t)(1<<2))  /* Auto Address Detect */ 
#define PARAM_UART_RS485_AUTO_ADDRRESS(x) ((x==UART_RS485_AUTO_ADDRRESS_DISABLE) || \
                                      (x==UART_RS485_AUTO_ADDRRESS_ENABLE))

/**
  * @}
  */
  
/*@brief UART_485_dirpin_ctrl_type definitions   
  * @{
  */
#define UART_RS485_DIRPIN_RTS  ((uint8_t)(0))
#define UART_RS485_DIRPIN_DTR  ((uint8_t)(1<<3))  /* Direction control */
#define PARAM_UART_RS485_DIRPIN_SELECT(x) ((x==UART_RS485_DIRPIN_RTS) || \
                                      (x==UART_RS485_DIRPIN_DTR))

/**
  * @}
  */
  
/*@brief UART_485_autodir_type definitions   
  * @{
  */
#define UART_RS485_AUTO_DIRECTION_DISABLE ((uint8_t)(0))
#define UART_RS485_AUTO_DIRECTION_ENABLE  ((uint8_t)(1<<4)) /* Auto Direction Control */
#define PARAM_UART_RS485_AUTO_DIRECTION(x) ((x==UART_RS485_AUTO_DIRECTION_DISABLE) || \
                                           (x==UART_RS485_AUTO_DIRECTION_ENABLE))

/**
  * @}
  */
  
/*@brief UART_485_dirpin_level_type definitions   
  * @{
  */
#define UART_RS485_DIRPIN_LOW   ((uint8_t)(0))
#define UART_RS485_DIRPIN_HIGH  ((uint8_t)(1<<5))  /* Reverses the polarity */
#define PARAM_UART_RS485_DIRPIN_LEVEL(x) ((x==UART_RS485_DIRPIN_LOW) || \
                                          (x==UART_RS485_DIRPIN_HIGH))

/**
  * @}
  */
  
/*@brief UART_irda_mode_type definitions   
  * @{
  */
#define UART_IRDA_DISABLE       ((uint8_t)(0))    /* IrDA mode disable */
#define UART_IRDA_ENABLE        ((uint8_t)(1<<0)) /* IrDA mode enable */
#define PARAM_UART_IRDA_MODE(x) ((x==UART_IRDA_DISABLE) || \
                                 (x==UART_IRDA_ENABLE))

/**
  * @}
  */
  
/*@brief UART_irda_input_invert_type definitions   
  * @{
  */
#define UART_IRDA_INPUTINVERT_NOT   ((uint8_t)(0))    /* IrDA serial not input inverted */
#define UART_IRDA_INPUTINVERTED     ((uint8_t)(1<<1)) /* IrDA serial input inverted */
#define PARAM_UART_IRDA_INPUT(x) ((x==UART_IRDA_INPUTINVERT_NOT) || \
                                  (x==UART_IRDA_INPUTINVERTED))

/**
  * @}
  */
  
/*@brief UART_irda_fixed_pulse_type definitions   
  * @{
  */
#define UART_IRDA_PULSEDIV_NORMAL     ((uint8_t)(0))  
#define UART_IRDA_PULSEDIV_2          ((uint8_t)(0x1<<2))
#define UART_IRDA_PULSEDIV_4          ((uint8_t)(0x3<<2))
#define UART_IRDA_PULSEDIV_8          ((uint8_t)(0x5<<2))
#define UART_IRDA_PULSEDIV_16         ((uint8_t)(0x7<<2)) 
#define UART_IRDA_PULSEDIV_32         ((uint8_t)(0x9<<2))
#define UART_IRDA_PULSEDIV_64         ((uint8_t)(0xB<<2))
#define UART_IRDA_PULSEDIV_128        ((uint8_t)(0xD<<2))
#define UART_IRDA_PULSEDIV_256        ((uint8_t)(0xF<<2))
#define PARAM_UART_IRDA_PULSEDIV(x) ((x==UART_IRDA_PULSEDIV_NORMAL) || \
                                     (x==UART_IRDA_PULSEDIV_2)      || \
                                     (x==UART_IRDA_PULSEDIV_4)      || \
                                     (x==UART_IRDA_PULSEDIV_8)      || \
                                     (x==UART_IRDA_PULSEDIV_16)     || \
                                     (x==UART_IRDA_PULSEDIV_64)     || \
                                     (x==UART_IRDA_PULSEDIV_128)    || \
                                     (x==UART_IRDA_PULSEDIV_256))
/**
  * @}
  */

/*@brief UART_process_time_delay definitions   
  * @{
  */

#define UART_BLOKING_TIMEOUT          (0xFFFFFFFFUL)
#define UART_NO_TIMEOUT               (0x1UL)
#define PARAM_UART_PROCESS_DELAY(x) ((x==UART_BLOKING_TIMEOUT) || \
                                     (x==UART_NO_TIMEOUT))

/**
  * @}
  */

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
#endif /* __LPC12xx_UART_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
