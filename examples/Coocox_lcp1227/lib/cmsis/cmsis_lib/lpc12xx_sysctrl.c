/**************************************************************************//**
 * $Id: lpc12xx_sysctrl.c 544 2010-09-16 01:33:45Z cnh82208 $
 *
 * @file     lpc12xx_sysctrl.c
 * @brief    Contains all functions support for SYSCTRL firmware library on LPC12xx.
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
/** @addtogroup SYSCTRL
 * @{
 */

/* Includes ----------------------------------------------------------------- */
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_libcfig.h"

#ifdef _SYSCTRL
/** @defgroup Sysctrl_Private_Functions
 * @{
 */

/**
  * @brief  Get the latest reset event.
  *
  * @param  None.  
  * @retval Latest reset source.
  *	    Each bit in the return value has the corresponding meaning:
  *             @arg bit 0: POR reset status 
  *             @arg bit 1: External reset status
  *             @arg bit 2: Watchdog reset status
  *             @arg bit 3: BOD reset status
  *             @arg bit 4: software system reset status
  */ 
uint32_t SYS_GetResetStatus(void)
{
	return (LPC_SYSCON->SYSRESSTAT & 0x1F);     
}

/**
  * @brief  Select where the ARM interrupt vectors are read from.
  *
  * @param  MapValue: System memory remap mode.  
  *         This parameter can be one of the following values:
  *             @arg SYS_BOOTLOADERMODE: Boot Loader Mode 
  *             @arg SYS_USERRAMMODE: User RAM Mode
  *             @arg SYS_USERFLASHMODE: User Flash Mode 
  * @retval None.
  */ 
void SYS_SetMemMap(uint32_t MapValue)
{
	MapValue &= 0x03;
	LPC_SYSCON->SYSMEMREMAP = MapValue;     
}

/**
  * @brief  Select and update system PLL clock source.
  *
  * @param  ClkSrc: PLL clock source.  
  *         This parameter can be one of the following values:
  *             @arg SYS_IRCOSC: IRC ocsillator 
  *             @arg SYS_SYSOSC: System ocsillator
  *             @arg SYS_WDTOSC: WDT ocsillator 
  * @retval None.
  */
void SYS_SelectSystemPLLCLKSource(uint32_t ClkSrc)
{
	ClkSrc &= 0x03;
	LPC_SYSCON->SYSPLLCLKSEL = ClkSrc;				         
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;			    
	LPC_SYSCON->SYSPLLCLKUEN = 0x00;			
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;
	while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));
}

/**
  * @brief  Config and lock system PLL.
  *
  * @param  M: Feedback divider value.  
  *         This parameter can be 1~32.
  * @param  P: Post divider ratio.  
  *         This parameter can be 2, 4, 8 or 16. 
  * @param  DirectState: FCCO output directly.  
  *         This parameter can be ENABLE or DISABLE.
  * @param  BypassEn: FSysPLLCLKIn output to divider directly.  
  *         This parameter can be ENABLE or DISABLE.
  * @retval None.
  *		1. DirectEn: DISABLE, BypassEn: DISABLE
  *		FSysPLLCLKOut = M * FSysPLLCLKIn
  *		FCCO = P * FSysPLLCLKOut
  *		Make sure FCCO is in the range 156 to 320MHz
  *		2. DirectEn: ENABLE, BypassEn: DISABLE
  *		FSysPLLCLKOut = M * FSysPLLCLKIn
  *		FCCO = FSysPLLCLKOut
  *		Make sure FCCO is in the range 156 to 320MHz
  *		3. DirectEn: DISABLE, BypassEn: ENABLE
  *		FSysPLLCLKOut = FSysPLLCLKIn / P
  *		4. DirectEn: ENABLE, BypassEn: ENABLE
  *		FSysPLLCLKOut = FSysPLLCLKIn 
  */
void SYS_ConfigSystemPLL(uint32_t M, uint32_t P, 
                         FunctionalState DirectState, FunctionalState BypassEn)
{
	uint32_t tmp;
	if((M > 32) || (M == 0)) return;
	if((P != 2) || (P != 4) || (P != 8) || (P != 16)) return;
	tmp = P >> 2;
	if(tmp == 4) tmp--;
	tmp = (BypassEn << 8) | (DirectState << 7) | (tmp << 5) | (M - 1);
	LPC_SYSCON->SYSPLLCTRL = tmp;
	LPC_SYSCON->PDRUNCFG &= ~SYS_PDRUNCFG_SYSPLL;				
	while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));		
}

/**
  * @brief  Select and update the main clock source.
  *
  * @param  ClkSrc: Main clock source.  
  *         This parameter can be one of the following values:
  *             @arg SYS_IRCOSC: IRC ocsillator
  *             @arg SYS_SYSPLLCLKIN: Input clock to system PLL 
  *             @arg SYS_SYSPLLCLKOUT: System PLL clock out
  *             @arg SYS_WDTOSC: WDT ocsillator 
  * @retval None.
  */
void SYS_SelectMainCLKSource(uint32_t ClkSrc)
{
	ClkSrc &= 0x03;
	LPC_SYSCON->MAINCLKSEL = ClkSrc;     		 
	LPC_SYSCON->MAINCLKUEN = 0x01;				 
	LPC_SYSCON->MAINCLKUEN = 0x00;			  
	LPC_SYSCON->MAINCLKUEN = 0x01;
  while (!(LPC_SYSCON->MAINCLKUEN & 0x01));	     
}

/**
  * @brief  Set clock divider for AHB.
  *
  * @param  DivNum: The divider for AHB clock/system clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: Diable the AHB clock/system clock
  *             @arg 1~255: Divided by this number  
  * @retval None.
  *		FAHBClk = FMainClk / DivNum
  */
void SYS_SetAHBClockDiv(uint32_t DivNum)
{
	DivNum &= 0xFF;
	LPC_SYSCON->SYSAHBCLKDIV = DivNum;
}

/**
  * @brief  Disable or enable system and peripheral clock.
  *
  * @param  AHBClk_Type: AHB clock type.
  * @param  CmdState: Enable or disable the clock for System or 
  *					  peripheral blocks.  
  *         This parameter can be ENABLE or DISABLE.  
  * @retval None.
  */                
void SYS_ConfigAHBCLK(uint32_t AHBClk_Type, FunctionalState CmdState)
{
	if(CmdState) LPC_SYSCON->SYSAHBCLKCTRL |= AHBClk_Type;
	else LPC_SYSCON->SYSAHBCLKCTRL &= ~(AHBClk_Type);
}

/**
  * @brief  Config WDT ocsillator clock.
  *
  * @param  DivNum: Divider for Fclkana to creat WDT OSC clock.
  *         This parameter can be one of the following values:
  *             2 * n: n = 1~32
  * @param  FreqSel: Select Fclkana.  
  *         This parameter can be one of the following values:
  *		@arg 0x01: 0.5MHz
  *		@arg 0x02: 0.8MHz
  *		@arg 0x03: 1.1MHz
  *		@arg 0x04: 1.4MHz
  *		@arg 0x05: 1.6MHz  
  *		@arg 0x06: 1.8MHz
  *		@arg 0x07: 2.0MHz
  *		@arg 0x08: 2.2MHz
  *		@arg 0x09: 2.4MHz
  *		@arg 0x0A: 2.6MHz
  *		@arg 0x0B: 2.7MHz
  *		@arg 0x0C: 2.9MHz
  *		@arg 0x0D: 3.1MHz
  *		@arg 0x0E: 3.2MHz
  *		@arg 0x0F: 3.4MHz    
  * @retval None.
  *		FWDTOSCClk = Fclkana / DivNum
  */
void SYS_ConfigWDTOSC(uint32_t DivNum, uint32_t FreqSel)
{	
	if((DivNum >> 1 ) > 32) return;
	if(FreqSel > 15) return;
	LPC_SYSCON->WDTOSCCTRL = (FreqSel << 5) | ((DivNum >> 1) - 1);
	LPC_SYSCON->PDRUNCFG &= ~SYS_PDRUNCFG_WDTOSC;           
}


/**
  * @brief  Config the CLKOUT clock.
  *
  * @param  ClkSrc: CLKOUT clock source.  
  *         This parameter can be one of the following values:
  *             @arg SYS_IRCOSC: IRC ocsillator 
  *             @arg SYS_SYSOSC: System ocsillator
  *             @arg SYS_WDTOSC: WDT ocsillator
  *             @arg SYS_MAINCLKOUT: Main clock
  * @param  DivNum: The divider for CLKOUT clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: Gate
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FCLKOUT = FClkSrc / DivNum
  */
void SYS_ConfigCLKOUTCLK(uint32_t ClkSrc, uint32_t DivNum)
{
	ClkSrc &= 0x03;
	DivNum &= 0xFF;
	LPC_SYSCON->CLKOUTCLKSEL = ClkSrc;
	LPC_SYSCON->CLKOUTUEN = 0x01;          
	LPC_SYSCON->CLKOUTUEN = 0x00;          
	LPC_SYSCON->CLKOUTUEN = 0x01;
	while (!(LPC_SYSCON->CLKOUTUEN & 0x01));
	LPC_SYSCON->CLKOUTDIV = DivNum;
}

/**
  * @brief  Config the RTC clock.
  *
  * @param  ClkSrc: RTC clock source.  
  *         This parameter can be one of the following values:
  *             @arg SYS_RTC_1HZ: 1Hz clock 
  *             @arg SYS_RTC_DELAY1HZ: delayed 1Hz clock
  *             @arg SYS_RTC_1KHZ: 1kHz clock
  *             @arg SYS_RTC_PCLK: RTC PCLK
  * @param  DivNum: The divider for RTC clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: Disable RTC PCLK
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FRTC_PCLK = FClkSrc / DivNum
  */
void SYS_ConfigRTCCLK(uint32_t ClkSrc, uint32_t DivNum)
{
	ClkSrc &= 0x03;
	DivNum &= 0xFF;
	LPC_SYSCON->RTCCLKDIV = DivNum;
	LPC_SYSCON->RTCOSCCTRL = ClkSrc;
}

/**
  * @brief  Reset peripheral.
  *
  * @param  RSTBlock: Peripheral type.  
  * @param  CmdState: Command State.  
  *         This parameter can be DISABLE or ENABLE. 
  * @retval None.
  */
void SYS_ResetPeripheral(uint32_t RSTBlock, FunctionalState CmdState)
{
	if (CmdState == ENABLE){
		LPC_SYSCON->PRESETCTRL &= ~(RSTBlock);
	} else {
		LPC_SYSCON->PRESETCTRL |= (RSTBlock);
	}
}

/**
  * @brief  Set clock divider value for SSP.
  *
  * @param  DivNum: The divider for SSP clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable SSP
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FSSP = FMainClk / DivNum
  */
void SYS_SetSSPClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->SSPCLKDIV = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for UART0.
  *
  * @param  DivNum: The divider for UART0 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable UART0
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FUART0 = FMainClk / DivNum
  */
void SYS_SetUART0ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->UART0CLKDIV = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for UART1.
  *
  * @param  DivNum: The divider for UART1 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable UART1
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FUART1 = FMainClk / DivNum
  */
void SYS_SetUART1ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->UART1CLKDIV = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter0.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter0 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter0 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter0ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG0 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter1.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter1 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter1 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter1ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG1 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter2.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter2 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter2 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter2ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG2 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter3.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter3 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter3 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter3ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG3 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter4.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter4 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter4 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter4ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG4 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter5.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter5 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter5 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter5ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG5 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for IOCONFIG glitch filter6.
  *
  * @param  DivNum: The divider for IOCONFIG glitch filter6 clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable glitch filter clock
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FGlitchFilter6 = FMainClk / DivNum
  */
void SYS_SetGlitchFilter6ClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->FILTERCLKCFG6 = (DivNum & 0xFF);
}

/**
  * @brief  Set clock divider value for SysTick.
  *
  * @param  DivNum: The divider for SysTick clock.  
  *         This parameter can be one of the following values:
  *             @arg 0: disable SysTick
  *             @arg 1~255: Divided by this number 
  * @retval None.
  *		FSysTick = FMainClk / DivNum
  */
void SYS_SetSysTickClockDiv(uint32_t DivNum)
{
	LPC_SYSCON->SYSTICKCLKDIV = (DivNum & 0xFF);
}

/**
  * @brief  Configures Deep sleep for specified peripheral to indicate                                                                         
  * 				the state the chip must enter when Deep-sleep  
  * 				mode is asserted by the ARM.
  *
  * @param  DStype: Specified peripheral for Deep sleep configuration.
  * @param  CmdState: Command State.  
  *         This parameter can be one of the following values:
  *             @arg ENABLE: Enable power down for this peripheral
  * 				when the Deep sleep mode is entered
  *             @arg DISABLE: Disable power down for this peripheral
  * 			        when the Deep sleep mode is entered 
  * @retval None.
  */
void SYS_ConfigDeepSleep(uint32_t DStype, FunctionalState CmdState)
{
	if (CmdState == ENABLE){
		LPC_SYSCON->PDSLEEPCFG |= (DStype);
	} else {
		LPC_SYSCON->PDSLEEPCFG &= ~(DStype);
	}
}

/**
  * @brief  Enter Power Down for a specified peripheral immediately.
  *
  * @param  PDtype: Specified peripheral for power down configuration.
  * @param  CmdState: Command State.  
  *         This parameter can be one of the following values:
  *             @arg ENABLE: Enable power down for this peripheral.
  *             @arg DISABLE: Disable power down for this peripheral. 
  * @retval None.
  */
void SYS_ConfigPowerDown(uint32_t PDtype, FunctionalState CmdState)
{
	if (CmdState == ENABLE){
		LPC_SYSCON->PDRUNCFG |= (PDtype);
	} else {
		LPC_SYSCON->PDRUNCFG &= ~(PDtype);
	}
}

/**
  * @brief  Configures wake up for specified peripheral to indicate
  * 				the state the chip must enter when it is waking up from
  * 				Deep-sleep mode.
  *
  * @param  WUtype: Specified peripheral for wake up configuration.
  * @param  CmdState: Command State.  
  *         This parameter can be one of the following values:
  *             @arg ENABLE: Enable power down for this peripheral
  * 							after waking up.
  *             @arg DISABLE: Disable power down for this peripheral
  * 							after waking up. 
  * @retval None.
  */
void SYS_ConfigWakeUp(uint32_t WUtype, FunctionalState CmdState)
{
	if (CmdState == ENABLE){
		LPC_SYSCON->PDAWAKECFG |= (WUtype);
	} else {
		LPC_SYSCON->PDAWAKECFG &= ~(WUtype);
	}
}

/**
  * @brief  Set the Peripheral interrupt as NMI.
  *
  * @param  Peripheral_int: Peripheral interrupt.  
  * @retval None.
  */
void SYS_NMIInterrupt(uint32_t Peripheral_int)
{
	LPC_SYSCON->INTNMI = Peripheral_int;
}

/**
  * @brief  Enable or disable the start signal bits in start logic0.
  *
  * @param  Pin_x: Start signal input pin.
  * @param  CmdState: Enable or disable the start signal.  
  *         This parameter can be one of the following values:
  *             @arg ENABLE: 
  *             @arg DISABLE:  
  * @retval None.
  */
void SYS_EnableStartLogic0(uint32_t Pin_x, FunctionalState CmdState)
{
	if(CmdState) LPC_SYSCON->STARTERP0 |= Pin_x;
	else LPC_SYSCON->STARTERP0 &= ~(Pin_x); 
}

/**
  * @brief  Select the edge for start logic0.
  *
  * @param  Pin_x: Start signal input pin.
  * @param  Edge: Edge for start signal.  
  *         This parameter can be one of the following values:
  *             @arg SYS_FALLINGEDGE: 
  *             @arg SYS_RISINGEDGE:  
  * @retval None.
  */
void SYS_SetStartLogic0Edge(uint32_t Pin_x, uint32_t Edge)
{
	if(Edge) LPC_SYSCON->STARTAPRP0 |= Pin_x;
	else LPC_SYSCON->STARTAPRP0 &= ~(Pin_x); 
}

/**
  * @brief  Reset start logic0 signal.
  *
  * @param  Pin_x: Start signal input pin.  
  * @retval None.
  */
void SYS_ResetStartLogic0(uint32_t Pin_x)
{
	LPC_SYSCON->STARTRSRP0CLR |= Pin_x; 
}

/**
  * @brief  Enable or disable the start signal bits in start logic1.
  *
  * @param  Peripheral_int: Start signal input.
  * @param  CmdState: Enable or disable the start signal.  
  *         This parameter can be one of the following values:
  *             @arg ENABLE: 
  *             @arg DISABLE:  
  * @retval None.
  */
void SYS_EnableStartLogic1(uint32_t Peripheral_int, FunctionalState CmdState)
{
	if(CmdState) LPC_SYSCON->STARTERP1 |= Peripheral_int;
	else LPC_SYSCON->STARTERP1 &= ~(Peripheral_int); 
}

/**
  * @brief  Select the edge for start logic1.
  *
  * @param  Peripheral_int: Start signal input.
  * @param  Edge: Edge for start signal.  
  *         This parameter can be one of the following values:
  *             @arg SYS_FALLINGEDGE: 
  *             @arg SYS_RISINGEDGE:  
  * @retval None.
  */
void SYS_SetStartLogic1Edge(uint32_t Peripheral_int, uint32_t Edge)
{
	if(Edge) LPC_SYSCON->STARTAPRP1 |= Peripheral_int;
	else LPC_SYSCON->STARTAPRP1 &= ~(Peripheral_int); 
}

/**
  * @brief  Reset start logic0 signal.
  *
  * @param  Peripheral_int: Start signal input.  
  * @retval None.
  */
void SYS_ResetStartLogic1(uint32_t Peripheral_int)
{
	LPC_SYSCON->STARTRSRP1CLR |= Peripheral_int; 
}

/**
  * @brief  Get the part ID of the chip.
  *
  * @param  None.  
  * @retval Device ID, should be:
  * 					- XXX: xxx
  */
uint32_t SYS_GetDeviceID(void)
{
	return(LPC_SYSCON->DEVICE_ID);
}

/**
 * @}
 */

#endif
 
/**
 * @}
 */
  
/* --------------------------------- End Of File ------------------------------ */
