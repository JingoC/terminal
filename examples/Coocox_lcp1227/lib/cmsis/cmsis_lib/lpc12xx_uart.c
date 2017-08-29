/**************************************************************************//**
 * $Id: lpc12xx_uart.c 541 2010-09-15 09:04:20Z nxp21428 $ 
 *
 * @file     lpc12xx_uart.c
 * @brief    Contains all functions support for UART firmware library on LPC12xx.
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
/** @addtogroup UART
 * @{
 */

#include "lpc12xx_uart.h"
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_libcfig.h"
#ifdef _UART 	

/* Private Types -------------------------------------------------------------- */
/** @defgroup UART_Private_Types
 * @{
 */
/* Macro to determine if it is valid UART port number */ 
#define PARAM_UARTx(x)     ((((uint32_t *)x)==((uint32_t *)LPC_UART0)) || \
                            (((uint32_t *)x)==((uint32_t *)LPC_UART1)))
#define PARAM_UART_MODEM(x) (((uint32_t *)x)==((uint32_t *)LPC_UART0))
#define PARAM_UART_485(x)   (((uint32_t *)x)==((uint32_t *)LPC_UART0))
#define PARAM_UART_IRDA(x)  (((uint32_t *)x)==((uint32_t *)LPC_UART1))

/* Macro to define bit mask */ 
#define UART_RBR_MASKBIT    ((uint8_t)0xFF) /* Received Buffer mask bit  */
#define UART_THR_MASKBIT    ((uint8_t)0xFF) /* Transmit Holding mask bit  */ 
#define UART_IER_BITMASK    ((uint32_t)(0x307)) /* IER normal bit mask */
#define UART_IER_BITMASK2   ((uint32_t)(0x38F)) /* IER include modem bit mask */
#define UART_IIR_BITMASK    ((uint32_t)(0x3CF)) /* IIR bit mask */
#define UART_FCR_BITMASK    ((uint8_t)(0xCF))   /* FCR bit mask */
#define UART_LCR_BITMASK    ((uint8_t)(0xFF))   /* LCR bit mask */
#define UART_MCR_BITMASK    ((uint8_t)(0xF3))   /* MCR bit mask */
#define UART_LSR_BITMASK    ((uint8_t)(0xFF))   /* LSR bit mask */
#define UART_MSR_BITMASK    ((uint8_t)(0xFF))   /* MSR register bit mask */
#define UART_SCR_BIMASK     ((uint8_t)(0xFF))   /* UART Scratch Pad bit mask */
#define UART_ACR_BITMASK    ((uint32_t)(0x307)) /* ACR bit mask */
#define UART_ICR_BITMASK    ((uint32_t)(0x3F))  /* IRDA bit mask */
#define UART_FDR_BITMASK    ((uint32_t)(0xFF))  /* FDR bit mask */
#define UART_TER_BITMASK    ((uint8_t)(0x80))   /* TER bit mask */
#define UART_RS485CTRL_BITMASK     ((uint32_t)(0x3F))  /* 485 CTRL bit mask */
#define UART_RS485ADRMATCH_BITMASK ((uint8_t)(0xFF))   /* 485 ADR bit mask */
#define UART_RS485DLY_BITMASK      ((uint8_t)(0xFF))   /* 485 DLY bit mask */
#define UART_FIFOLVL_BITMASK       ((uint32_t)(0x0F0F))/* 485 FIFOLVL bit mask */

/* Macro to define control bit */
#define UART_LCR_DLAB_EN        ((uint8_t)(1<<7))   /* DivisorLatchesAccess enable*/
#define UART_FCR_FIFO_EN        ((uint8_t)(1<<0))   /* FIFO enable */
#define UART_FCR_RX_RS          ((uint8_t)(1<<1))   /* FIFO RX reset */
#define UART_FCR_TX_RS          ((uint8_t)(1<<2))   /* FIFO TX reset */
#define UART_TER_TXEN           ((uint8_t)(1<<7))   /* Transmit enable bit */
#define UART_ACR_START          ((uint32_t)(1<<0))  /* Auto baudrate Start */
#define UART_ACR_ABEOINT_CLR    ((uint32_t)(1<<8))  /* Auto baudrate end INT clear */
#define UART_ACR_ABTOINT_CLR    ((uint32_t)(1<<9))  /* Auto baudrate Ttime-out INT clear */

/* Macro to define caculate */
#define UART_LOAD_DLL(div)        ((div) & 0xFF) 
#define UART_LOAD_DLM(div)        (((div) >> 8) & 0xFF)
#define UART_FDR_DIVADDVAL(n)     ((uint32_t)(n&0x0F))      /* Pre-scaler divisor */
#define UART_FDR_MULVAL(n)        ((uint32_t)((n<<4)&0xF0)) /* Multiplier value */
#define UART_FIFOLVL_RXFIFOLVL(n) ((uint32_t)(n&0x0F))      /*receiver FIFO */
#define UART_FIFOLVL_TXFIFOLVL(n) ((uint32_t)((n>>8)&0x0F)) /*transmitter FIFO */

/* Macro to define parameter */
#define UART_ACCEPTED_BAUDRATE_ERROR  (5) /* Acceptable baudrate error, 1/2(n) percent */
#define UART_TX_FIFO_SIZE             (16)
#define UART_DEVICE_NUMBER            (2)

/* Macro to define Variables */

static UART_CFG_Type         UART_Configration[UART_DEVICE_NUMBER];
#ifdef _MODEM
static UART_MODEM_CFG_Type   UART_ModemConfigration;
#endif
#ifdef _RS485
static UART_RS485_CFG_Type   UART_RS485Configration;
#endif
#ifdef _IRDA
static UART_IRDA_CFG_Type   UART_IRDAConfigration;
#endif

/**
 * @}
 */

/** @defgroup UART_Private_Functions
  * @{
  */
static uint8_t uart_get_num(LPC_UART_TypeDef *UARTx);
static Status uart_set_divisors(LPC_UART_TypeDef *UARTx, uint32_t baudrate);

/*
  * @brief   Get uart number
  * @param UARTx:   Pointer to selected UART peripheral, should be LPC_UART0, LPC_UART1.
  * @return number.
  */
static uint8_t uart_get_num(LPC_UART_TypeDef *UARTx) {
    if (UARTx == LPC_UART0) 
        return (0);
    else 
        return (1);
}


/*
  * @brief   Set uart baudrate
  * @param UARTx     Pointer to selected UART peripheral, should be LPC_UART0, LPC_UART1.
  * @param baudrate  baudrate number
  * @return status       SUCCESS or ERROR
  */
static Status uart_set_divisors(LPC_UART_TypeDef *UARTx, uint32_t baudrate)
{
    Status errorStatus = ERROR;

    uint32_t uClk = 0;
    uint32_t calcBaudrate = 0;
    uint32_t temp = 0;

    uint32_t mulFracDiv, dividerAddFracDiv;
    uint32_t diviser = 0 ;
    uint32_t mulFracDivOptimal = 1;
    uint32_t dividerAddOptimal = 0;
    uint32_t diviserOptimal = 0;

    uint32_t relativeError = 0;
    uint32_t relativeOptimalError = (baudrate >> UART_ACCEPTED_BAUDRATE_ERROR);


    /* get UART block clock */
    if (UARTx == LPC_UART0){
        uClk = (MainClock/LPC_SYSCON->UART0CLKDIV)>>4; /* div by 16 */
    }
    else if (UARTx == LPC_UART1){
        uClk = (MainClock/LPC_SYSCON->UART1CLKDIV)>>4; /* div by 16 */
    }

    /* In the Uart IP block, baud rate is calculated using FDR and DLL-DLM registers
         * The formula is :
         * BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * (DLL)
         * It involves floating point calculations. That's the reason the formulae are adjusted with
         * Multiply and divide method.*/
    /* The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
         * 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15 */
    for (dividerAddFracDiv = 0 ; dividerAddFracDiv < 15 ;dividerAddFracDiv++){       
        for (mulFracDiv = (dividerAddFracDiv + 1) ; mulFracDiv <= 15 ;mulFracDiv++){
            temp = (mulFracDiv * uClk) / ((mulFracDiv + dividerAddFracDiv));
            
            diviser = temp / baudrate;
            if ((temp % baudrate) > (baudrate >> 1))
            diviser++;
            
            if (diviser > 2 && diviser < 65536){
                calcBaudrate = temp / diviser;
                
                if (calcBaudrate <= baudrate)
                  relativeError = baudrate - calcBaudrate;
                else
                  relativeError = calcBaudrate - baudrate;
                
                if ((relativeError < relativeOptimalError)){
                    mulFracDivOptimal = mulFracDiv ;
                    dividerAddOptimal = dividerAddFracDiv;
                    diviserOptimal = diviser;
                    relativeOptimalError = relativeError;
                    errorStatus = SUCCESS;
                }
            } /* End of if */
        } /* end of inner for loop */
    } /* end of outer for loop  */

    if (errorStatus == SUCCESS){
        UARTx->LCR |= UART_LCR_DLAB_EN;
        /* Then reset DLAB bit */
        while(UARTx->LCR & UART_LCR_DLAB_EN){        
           UARTx->DLL = UART_LOAD_DLL(diviserOptimal);
           UARTx->DLM = UART_LOAD_DLM(diviserOptimal);
           UARTx->LCR &= (~UART_LCR_DLAB_EN) & UART_LCR_BITMASK;
           UARTx->FDR = (UART_FDR_MULVAL(mulFracDivOptimal) |\
                         UART_FDR_DIVADDVAL(dividerAddOptimal)) & UART_FDR_BITMASK;

        }
    }

    return errorStatus;
}

/*
  * @brief       Initializes the UARTx peripheral.
  * @param    UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1.
  * @return     None
  */
void UART_Init(LPC_UART_TypeDef *UARTx)
{
    volatile uint32_t tmp;
    CHECK_PARAM(PARAM_UARTx(UARTx));

    UARTx->FDR = 0x10;           //set to default value: 0x10

    /* Empty the registers */
    UARTx->FCR = ( UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS);

    while (UARTx->LSR & UART_LS_RX_DATA_READY){
        tmp = UARTx->RBR;        // Dummy reading
    }

    UARTx->TER = UART_TER_TXEN;
    while (!(UARTx->LSR & UART_LS_TX_EMPTY));  // Wait for current transmit complete

    UARTx->TER = 0;             // Disable Tx
    UARTx->IER = 0;             // Disable interrupt
    UARTx->LCR = 0;             // Set LCR to default state
    UARTx->ACR = 0;             // Set ACR to default state
    
    if (UARTx == LPC_UART0){   
#ifdef _MODEM        
        UARTx->MCR = 0;         // Set Modem Control to default state
        tmp = UARTx->MSR;       // Dummy Reading to Clear Status
#endif        
#ifdef _RS485        
        UARTx->RS485CTRL = 0;   // Set RS485 control to default state
        UARTx->ADRMATCH = 0;    // Set RS485 addr match to default state
        UARTx->RS485DLY = 0;    // Set RS485 delay timer to default state
#endif        
    }
#ifdef _IRDA
    else{
    
        UARTx->ICR = 0;         // Set IrDA to default state
    }
#endif
    tmp = UARTx->LSR;           // Dummy reading

    tmp = uart_get_num(UARTx);

    UART_Configration[tmp].baudrate = 0;
    UART_Configration[tmp].databits = UART_CFG_DATABIT_8;
    UART_Configration[tmp].stopbits = UART_CFG_STOPBIT_1;
    UART_Configration[tmp].parity = UART_CFG_PARITY_NONE;
    UART_Configration[tmp].fifolevel = UART_CFG_FIFOTRG_1;
    UART_Configration[tmp].fifodma = UART_CFG_DMAMODE_DISABLE;
    UART_Configration[tmp].txdbreak = UART_CFG_TXDBREAK_DISABLE;

}

/*
  * @brief   Config the UARTx according to the specified parameters in the UART_CFG_Type.
  * @param UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1.
  * @param config    UART_ConfigStruct Pointer to a UART_CFG_Type structure
  *                          that contains the configuration information for the specified UART periphera.
  * @return  None
  */
void UART_SetConfig(LPC_UART_TypeDef *UARTx, UART_CFG_Type *config)
{
    uint8_t tmp;

    CHECK_PARAM(PARAM_UARTx(UARTx));
    CHECK_PARAM(PARAM_UART_DATABIT(config->databits));
    CHECK_PARAM(PARAM_UART_PARITY(config->parity));
    CHECK_PARAM(PARAM_UART_STOPBIT(config->stopbits));
    CHECK_PARAM(PARAM_UART_FIFO_LEVEL(config->fifolevel));
    CHECK_PARAM(PARAM_UART_FIFO_DMA(config->fifodma));

    tmp = uart_get_num(UARTx);

    if(UART_Configration[tmp].baudrate != (*config).baudrate){

        uart_set_divisors(UARTx, (config->baudrate));
        UART_Configration[tmp].baudrate = (*config).baudrate;       
    }

    UARTx->LCR = (((config->databits)|(config->parity)|(config->stopbits)|\
                 (config->txdbreak)) & UART_LCR_BITMASK);
   
    UARTx->FCR = ((UART_FCR_FIFO_EN | (config->fifodma)|(config->fifolevel))&\
                   UART_FCR_BITMASK);

    UART_Configration[tmp].databits = (*config).databits;
    UART_Configration[tmp].stopbits = (*config).stopbits;
    UART_Configration[tmp].parity = (*config).parity;
    UART_Configration[tmp].txdbreak = (*config).txdbreak;
    UART_Configration[tmp].fifolevel = (*config).fifolevel;
    UART_Configration[tmp].fifodma = (*config).fifodma;

   
}

/*
  * @brief     Get configuration the UARTx according to the specified parameters in the UART_CFG_Type.
  * @param   UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1.
  * @param   config    UART_ConfigStruct Pointer to a UART_CFG_Type structure
  *                             that contains the configuration information for the specified UART periphera.
  * @return    None
  */

void UART_GetConfig(LPC_UART_TypeDef *UARTx, UART_CFG_Type *config)
{
    uint32_t tmp;

    CHECK_PARAM(PARAM_UARTx(UARTx));
    tmp = uart_get_num(UARTx);
    *config = UART_Configration[tmp];
}

/**
  * @brief    Enable/Disable transmission on UART TxD pin
  * @param UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1
  * @param NewState New State of Tx transmission function, should be:
  *                            - ENABLE: Enable this function
  *                            - DISABLE: Disable this function 
  * @return none
  */
void UART_ConfigTXD(LPC_UART_TypeDef *UARTx, FunctionalState NewState)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));
    if (NewState == ENABLE) {   
        UARTx->TER |= UART_TER_TXEN;
    }else{
        UARTx->TER &= (~UART_TER_TXEN) & UART_TER_BITMASK;
    }
}

/*
  * @brief    Send a block of data via UART peripheral
  * @param UARTx   Selected UART used to send data, should be LPC_UART0, LPC_UART1.
  * @param txbuf   Pointer to Transmit buffer
  * @param buflen  Length of Transmit buffer, a multiple of 16 is better.
  *                        16 is the best because the FIFO number is 16.
  * @param timeout     UART_process_time_delay definition 
  * @return  Number of bytes sent.
  */
uint32_t UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf, uint32_t buflen,\
                       uint32_t timeout)
{
    uint32_t bToSend, bSent, timeOut, fifo_cnt;
    uint8_t *pChar = txbuf;

    CHECK_PARAM(PARAM_UARTx(UARTx));
    CHECK_PARAM(PARAM_UART_PROCESS_DELAY(timeout));

    bToSend = buflen;

    bSent = 0;
    while (bToSend){
        timeOut = timeout;
        // Wait for THR empty with timeout
        while (!(UARTx->LSR & UART_LS_TX_EMPTY)) {
            if (timeOut == 0) break;
            timeOut--;
        }
        if(timeOut == 0) break;     // Time out!
        fifo_cnt = UART_TX_FIFO_SIZE;
        while (fifo_cnt && bToSend){
            UARTx->THR = ((*pChar++) & UART_THR_MASKBIT);
            fifo_cnt--;
            bToSend--;
            bSent++;
        }
    }
    return bSent;
}

/*
  * @brief    Receive a block of data via UART peripheral
  * @param UARTx    Selected UART used to send data, should be LPC_UART0, LPC_UART1
  * @param rxbuf      Pointer to Received buffer 
  * @param buflen     Length of Received buffer, a multiple of 16 is better.
  *                           16 is the best because the FIFO number is 16.
  * @param timeout   UART_process_time_delay definition
  * @return  Number of bytes received
  */
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, uint32_t buflen,\
                           uint32_t timeout)
{
    uint32_t bToRecv, bRecv, timeOut;
    uint8_t *pChar = rxbuf;

    CHECK_PARAM(PARAM_UARTx(UARTx));
    CHECK_PARAM(PARAM_UART_PROCESS_DELAY(timeout));

    bToRecv = buflen;															 

    bRecv = 0;
    while (bToRecv){
        timeOut = timeout;
        while (!(UARTx->LSR & UART_LS_RX_DATA_READY)){
            if (timeOut == 0) break;
            timeOut--;
        }
        // Time out!
        if(timeOut == 0) break;
        // Get data from the buffer
        (*pChar++) = (UARTx->RBR & UART_RBR_MASKBIT);
        bToRecv--;
        bRecv++;
    }
    return bRecv;
}

/*
  * @brief     Get current value of Line Status register in UART peripheral.
  * @param  UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1
  * @return   Current value of Line Status register in UART peripheral.
  *               The return value of this function must be ANDed with each member in
  *                UART_line_status_type definitions to determine current flag status
  *                corresponding to each Line status type. Because some flags in
  *                Line Status register will be cleared after reading, the next reading
  *                Line Status register could not be correct. So this function used to
  *                read Line status register in one time only, then the return value
  *                used to check all flags.
  */
uint8_t UART_GetLineStatus(LPC_UART_TypeDef* UARTx)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));

    return ((UARTx->LSR) & UART_LSR_BITMASK);
}

#ifdef _UART_INT
/*
 * @brief   Enable or Disable specified UART interrupt.
 * @param UARTx          UART peripheral selected, should be LPC_UART0, LPC_UART1
 * @param UARTIntCfg  refer to UART_interrupt_type definitions
 * @param NewState      New state of specified UART interrupt type, should be:
 *                                 - ENALBE: Enable this UART interrupt type.
 *                                 - DISALBE: Disable this UART interrupt type.
 * @return      None
 */
void UART_ConfigInts(LPC_UART_TypeDef *UARTx, uint16_t UARTIntCfg,\
                         FunctionalState NewState)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));
    CHECK_PARAM(PARAM_UART_INTCFG(UARTIntCfg));
    CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
    if (NewState == ENABLE){
        UARTx->IER |= UARTIntCfg;
    }else{
        uint32_t bitmask = UART_IER_BITMASK;
        if(uart_get_num(UARTx) == 0){
            bitmask = UART_IER_BITMASK2;
        }
        UARTx->IER &= (~UARTIntCfg) & bitmask;
    }
}

/*
  * @brief    Get current value of Interrupt identification register in UART peripheral.
  * @param  UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1
  * @return  Current value of interrupt Status register in UART peripheral.
  *              The return value of this function must be ANDed with each member in
  *              UART_int_status_type definitions to determine current flag status
  *              corresponding to each Line status type. Because some flags in
  *              Interrupt Identification register will be cleared after reading, the next reading
  *              Interrupt Identification register could not be correct. So this function used to
  *              read Interrupt Identification register in one time only, then the return value
  *              used to check all flags.
  */
uint16_t UART_GetINTStatus(LPC_UART_TypeDef* UARTx)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));

    return ((UARTx->IIR) & UART_IIR_BITMASK);
}

/*
  * @brief     Start Auto Baudrate activity
  * @param  UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1
  * @param  mode    UART_auto_baudrate_mode_type
  * @param  restart   UART_auto_baudrate_restart_type definitions
  * @return   none
  */
void UART_StartAutoBaud(LPC_UART_TypeDef *UARTx, uint8_t mode, uint8_t restart)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));
    CHECK_PARAM(PARAM_UART_AB_MODE(mode));
    CHECK_PARAM(PARAM_UART_AB_RESTART(restart));       
    
    // Clear DLL and DLM value
    UARTx->LCR |= UART_LCR_DLAB_EN;
    UARTx->DLL = 0;
    UARTx->DLM = 0;
    UARTx->LCR &= ~UART_LCR_DLAB_EN;
    // FDR value must be reset to default value
    UARTx->FDR = 0x10;
    UARTx->ACR = (UART_ACR_START | (mode) | (restart));

}

/*
  * @brief     Clear Auto Baudrate int flag
  * @param  UARTx   UART peripheral selected, should be LPC_UART0, LPC_UART1
  *               Auto-baudrate mode enable bit will be cleared once this mode completed.
  * @return   none
  */

void UART_ClearAutoBaud(LPC_UART_TypeDef *UARTx)
{
    CHECK_PARAM(PARAM_UARTx(UARTx));

    UARTx->ACR = (UART_ACR_ABEOINT_CLR | UART_ACR_ABTOINT_CLR);
}

#endif //_UART_INT

#ifdef _MODEM
/*
  * @brief    Configure Full Modem mode for UART peripheral
  * @param UARTx  Only should be LPC_UART0 
  * @param config   A pointer to UART_MODEM_CFG_Type structure that
  *                         contains specified information about UART auto baudrate configuration
  * @return none
  */
void UART_SetConfigModem(LPC_UART_TypeDef *UARTx, UART_MODEM_CFG_Type *config)
{
    uint8_t tmp;

    CHECK_PARAM(PARAM_UART_MODEM(UARTx));
    CHECK_PARAM(PARAM_MODEM_PIN_DTR(config->DTRcontrol));
    CHECK_PARAM(PARAM_MODEM_PIN_RTS(config->RTScontrol));
    CHECK_PARAM(PARAM_MODEM_MODE_LOOPBACK(config->loopback));
    CHECK_PARAM(PARAM_MODEM_MODE_AUTO_RTS(config->RTSen));
    CHECK_PARAM(PARAM_MODEM_MODE_AUTO_CTS(config->CTSen));

    tmp = config->DTRcontrol;
    tmp |= config->RTScontrol;
    tmp |= config->loopback;
    tmp |= config->RTSen;
    tmp |= config->CTSen;   
    UARTx->MCR = (uint8_t)(tmp & UART_MCR_BITMASK);

    UART_ModemConfigration.DTRcontrol = (*config).DTRcontrol;
    UART_ModemConfigration.RTScontrol = (*config).RTScontrol;
    UART_ModemConfigration.loopback = (*config).loopback;
    UART_ModemConfigration.RTSen = (*config).RTSen;
    UART_ModemConfigration.CTSen = (*config).CTSen;

}

/*
  * @brief     Get configure Full Modem mode for UART peripheral
  * @param  UARTx   Only should be LPC_UART0
  * @param  config    A pointer to UART_MODEM_CFG_Type structure that
  *                           contains specified information about UART auto baudrate configuration
  * @return none
  */

void UART_GetConfigModem(LPC_UART_TypeDef *UARTx, UART_MODEM_CFG_Type *config)
{
    CHECK_PARAM(PARAM_UART_MODEM(UARTx));
    *config = UART_ModemConfigration;
}

/*
  * @brief    Get current status of modem status register
  * @param UARTx  Only should be LPC_UART0
  * @return  Current value of modem status register
  *               The return value of this function must be ANDed with each member
  *               UART_modem_status_type definitions to determine current flag status
  *               corresponding to each modem flag status. Because some flags in
  *               modem status register will be cleared after reading, the next reading
  *               modem register could not be correct. So this function used to
  *               read modem status register in one time only, then the return value
  *               used to check all flags.
  */
uint8_t UART_GetModemStatus(LPC_UART_TypeDef *UARTx)
{
    CHECK_PARAM(PARAM_UART_MODEM(UARTx));
    return ((UARTx->MSR) & UART_MSR_BITMASK);
}
#endif
#ifdef _RS485
/*
  * @brief   Configure UART in RS485 mode according to the specified parameters
  * @param UARTx  Only should be LPC_UART0
  * @param config   RS485ConfigStruct Pointer to a UART_RS485_CFG_Type structure
  *                         that contains the configuration information for specified UART in RS485 mode.
  * @return  None
  */
void UART_SetConfigRS485(LPC_UART_TypeDef *UARTx, UART_RS485_CFG_Type *config)
{
    uint32_t tmp;

    CHECK_PARAM(PARAM_UART_485(UARTx));
    CHECK_PARAM(PARAM_UART_RS485_NORMAL_MULTIDROP(config->nmmen));
    CHECK_PARAM(PARAM_UART_RS485_RECEIVER(config->rxdis));
    CHECK_PARAM(PARAM_UART_RS485_AUTO_ADDRRESS(config->adden));
    CHECK_PARAM(PARAM_UART_RS485_DIRPIN_SELECT(config->dirpinsel));
    CHECK_PARAM(PARAM_UART_RS485_AUTO_DIRECTION(config->autodiren));
    CHECK_PARAM(PARAM_UART_RS485_DIRPIN_LEVEL(config->pinlevel));
    CHECK_PARAM((config->delay) < 0xFF);
    CHECK_PARAM((config->addrress) < 0xFF);

    // If Auto Direction Control is enabled -  This function is used in Master mode
    tmp = config->nmmen;
    tmp |= config->rxdis;
    tmp |= config->adden;
    tmp |= config->dirpinsel;
    tmp |= config->autodiren;
    tmp |= config->pinlevel;

    UARTx->RS485CTRL = tmp & UART_RS485CTRL_BITMASK;    
    UARTx->ADRMATCH = config->addrress & UART_RS485ADRMATCH_BITMASK;   
    UARTx->RS485DLY = config->delay & UART_RS485DLY_BITMASK;  

    UART_RS485Configration.nmmen = (*config).nmmen;
    UART_RS485Configration.rxdis = (*config).rxdis;
    UART_RS485Configration.adden = (*config).adden;
    UART_RS485Configration.dirpinsel = (*config).dirpinsel;
    UART_RS485Configration.autodiren = (*config).autodiren;
    UART_RS485Configration.pinlevel = (*config).pinlevel;
    UART_RS485Configration.delay = (*config).delay;
    UART_RS485Configration.addrress = (*config).addrress;

}


/**
  * @brief    Get configuration UART in RS485 mode according to the specified parameters
  * @param UARTx  Only should be LPC_UART0.
  * @param config   A pointer to UART_RS485_CFG_Type structure that
  *                         that contains the configuration information for specified UART in RS485 mode.
  * @return  None
  */
void UART_GetConfigRS485(LPC_UART_TypeDef *UARTx, UART_RS485_CFG_Type *config)
{
    CHECK_PARAM(PARAM_UART_485(UARTx));
    *config = UART_RS485Configration;
}
#endif /* _RS485 */

#ifdef _IRDA
/*
  * @brief   Configure UART in IRDA mode according to the specified parameters
  * @param UARTx  Only should be LPC_UART1
  * @param config   RS485ConfigStruct Pointer to a UART_IRDA_CFG_Type structure
  *                         that contains the configuration information for specified UART in IRDA mode.
  * @return  None
  */
void UART_SetConfigIRDA(LPC_UART_TypeDef *UARTx, UART_IRDA_CFG_Type *config)
{
    volatile uint32_t tmp;

    CHECK_PARAM(PARAM_UART_IRDA(UARTx));
    CHECK_PARAM(PARAM_UART_IRDA_MODE(config->mode));
    CHECK_PARAM(PARAM_UART_IRDA_INPUT(config->invert));
    CHECK_PARAM(PARAM_UART_IRDA_PULSEDIV(config->pulse));

    tmp = config->mode;
    tmp |= config->invert;
    tmp |= config->pulse;
    UARTx->ICR = tmp & UART_ICR_BITMASK;

    UART_IRDAConfigration.mode = (*config).mode;
    UART_IRDAConfigration.invert = (*config).invert;
    UART_IRDAConfigration.pulse = (*config).pulse;

}

/**
  * @brief    Get configuration UART in IRDA mode according to the specified parameters
  * @param UARTx  Only should be LPC_UART1.
  * @param config   A pointer to UART_IRDA_CFG_Type structure that
  *                         that contains the configuration information for specified UART in IRDA mode.
  * @return  None
  */
void UART_GetConfigIRDA(LPC_UART_TypeDef *UARTx, UART_IRDA_CFG_Type *config)
{
    CHECK_PARAM(PARAM_UART_IRDA(UARTx));
    *config = UART_IRDAConfigration;
}
#endif /* _IRDA */

/**
 * @}
 */

#endif
 
/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

