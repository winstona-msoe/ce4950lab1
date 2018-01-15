/*******************************************************************************
* File Name: CLOCK3.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_CLOCK3_H)
#define CY_CLOCK_CLOCK3_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void CLOCK3_Start(void) ;
void CLOCK3_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CLOCK3_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CLOCK3_StandbyPower(uint8 state) ;
void CLOCK3_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CLOCK3_GetDividerRegister(void) ;
void CLOCK3_SetModeRegister(uint8 modeBitMask) ;
void CLOCK3_ClearModeRegister(uint8 modeBitMask) ;
uint8 CLOCK3_GetModeRegister(void) ;
void CLOCK3_SetSourceRegister(uint8 clkSource) ;
uint8 CLOCK3_GetSourceRegister(void) ;
#if defined(CLOCK3__CFG3)
void CLOCK3_SetPhaseRegister(uint8 clkPhase) ;
uint8 CLOCK3_GetPhaseRegister(void) ;
#endif /* defined(CLOCK3__CFG3) */

#define CLOCK3_Enable()                       CLOCK3_Start()
#define CLOCK3_Disable()                      CLOCK3_Stop()
#define CLOCK3_SetDivider(clkDivider)         CLOCK3_SetDividerRegister(clkDivider, 1u)
#define CLOCK3_SetDividerValue(clkDivider)    CLOCK3_SetDividerRegister((clkDivider) - 1u, 1u)
#define CLOCK3_SetMode(clkMode)               CLOCK3_SetModeRegister(clkMode)
#define CLOCK3_SetSource(clkSource)           CLOCK3_SetSourceRegister(clkSource)
#if defined(CLOCK3__CFG3)
#define CLOCK3_SetPhase(clkPhase)             CLOCK3_SetPhaseRegister(clkPhase)
#define CLOCK3_SetPhaseValue(clkPhase)        CLOCK3_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CLOCK3__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CLOCK3_CLKEN              (* (reg8 *) CLOCK3__PM_ACT_CFG)
#define CLOCK3_CLKEN_PTR          ((reg8 *) CLOCK3__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CLOCK3_CLKSTBY            (* (reg8 *) CLOCK3__PM_STBY_CFG)
#define CLOCK3_CLKSTBY_PTR        ((reg8 *) CLOCK3__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CLOCK3_DIV_LSB            (* (reg8 *) CLOCK3__CFG0)
#define CLOCK3_DIV_LSB_PTR        ((reg8 *) CLOCK3__CFG0)
#define CLOCK3_DIV_PTR            ((reg16 *) CLOCK3__CFG0)

/* Clock MSB divider configuration register. */
#define CLOCK3_DIV_MSB            (* (reg8 *) CLOCK3__CFG1)
#define CLOCK3_DIV_MSB_PTR        ((reg8 *) CLOCK3__CFG1)

/* Mode and source configuration register */
#define CLOCK3_MOD_SRC            (* (reg8 *) CLOCK3__CFG2)
#define CLOCK3_MOD_SRC_PTR        ((reg8 *) CLOCK3__CFG2)

#if defined(CLOCK3__CFG3)
/* Analog clock phase configuration register */
#define CLOCK3_PHASE              (* (reg8 *) CLOCK3__CFG3)
#define CLOCK3_PHASE_PTR          ((reg8 *) CLOCK3__CFG3)
#endif /* defined(CLOCK3__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CLOCK3_CLKEN_MASK         CLOCK3__PM_ACT_MSK
#define CLOCK3_CLKSTBY_MASK       CLOCK3__PM_STBY_MSK

/* CFG2 field masks */
#define CLOCK3_SRC_SEL_MSK        CLOCK3__CFG2_SRC_SEL_MASK
#define CLOCK3_MODE_MASK          (~(CLOCK3_SRC_SEL_MSK))

#if defined(CLOCK3__CFG3)
/* CFG3 phase mask */
#define CLOCK3_PHASE_MASK         CLOCK3__CFG3_PHASE_DLY_MASK
#endif /* defined(CLOCK3__CFG3) */

#endif /* CY_CLOCK_CLOCK3_H */


/* [] END OF FILE */
