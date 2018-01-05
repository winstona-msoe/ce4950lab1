/*******************************************************************************
* File Name: CLOCK2.h
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

#if !defined(CY_CLOCK_CLOCK2_H)
#define CY_CLOCK_CLOCK2_H

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

void CLOCK2_Start(void) ;
void CLOCK2_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CLOCK2_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CLOCK2_StandbyPower(uint8 state) ;
void CLOCK2_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CLOCK2_GetDividerRegister(void) ;
void CLOCK2_SetModeRegister(uint8 modeBitMask) ;
void CLOCK2_ClearModeRegister(uint8 modeBitMask) ;
uint8 CLOCK2_GetModeRegister(void) ;
void CLOCK2_SetSourceRegister(uint8 clkSource) ;
uint8 CLOCK2_GetSourceRegister(void) ;
#if defined(CLOCK2__CFG3)
void CLOCK2_SetPhaseRegister(uint8 clkPhase) ;
uint8 CLOCK2_GetPhaseRegister(void) ;
#endif /* defined(CLOCK2__CFG3) */

#define CLOCK2_Enable()                       CLOCK2_Start()
#define CLOCK2_Disable()                      CLOCK2_Stop()
#define CLOCK2_SetDivider(clkDivider)         CLOCK2_SetDividerRegister(clkDivider, 1u)
#define CLOCK2_SetDividerValue(clkDivider)    CLOCK2_SetDividerRegister((clkDivider) - 1u, 1u)
#define CLOCK2_SetMode(clkMode)               CLOCK2_SetModeRegister(clkMode)
#define CLOCK2_SetSource(clkSource)           CLOCK2_SetSourceRegister(clkSource)
#if defined(CLOCK2__CFG3)
#define CLOCK2_SetPhase(clkPhase)             CLOCK2_SetPhaseRegister(clkPhase)
#define CLOCK2_SetPhaseValue(clkPhase)        CLOCK2_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CLOCK2__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CLOCK2_CLKEN              (* (reg8 *) CLOCK2__PM_ACT_CFG)
#define CLOCK2_CLKEN_PTR          ((reg8 *) CLOCK2__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CLOCK2_CLKSTBY            (* (reg8 *) CLOCK2__PM_STBY_CFG)
#define CLOCK2_CLKSTBY_PTR        ((reg8 *) CLOCK2__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CLOCK2_DIV_LSB            (* (reg8 *) CLOCK2__CFG0)
#define CLOCK2_DIV_LSB_PTR        ((reg8 *) CLOCK2__CFG0)
#define CLOCK2_DIV_PTR            ((reg16 *) CLOCK2__CFG0)

/* Clock MSB divider configuration register. */
#define CLOCK2_DIV_MSB            (* (reg8 *) CLOCK2__CFG1)
#define CLOCK2_DIV_MSB_PTR        ((reg8 *) CLOCK2__CFG1)

/* Mode and source configuration register */
#define CLOCK2_MOD_SRC            (* (reg8 *) CLOCK2__CFG2)
#define CLOCK2_MOD_SRC_PTR        ((reg8 *) CLOCK2__CFG2)

#if defined(CLOCK2__CFG3)
/* Analog clock phase configuration register */
#define CLOCK2_PHASE              (* (reg8 *) CLOCK2__CFG3)
#define CLOCK2_PHASE_PTR          ((reg8 *) CLOCK2__CFG3)
#endif /* defined(CLOCK2__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CLOCK2_CLKEN_MASK         CLOCK2__PM_ACT_MSK
#define CLOCK2_CLKSTBY_MASK       CLOCK2__PM_STBY_MSK

/* CFG2 field masks */
#define CLOCK2_SRC_SEL_MSK        CLOCK2__CFG2_SRC_SEL_MASK
#define CLOCK2_MODE_MASK          (~(CLOCK2_SRC_SEL_MSK))

#if defined(CLOCK2__CFG3)
/* CFG3 phase mask */
#define CLOCK2_PHASE_MASK         CLOCK2__CFG3_PHASE_DLY_MASK
#endif /* defined(CLOCK2__CFG3) */

#endif /* CY_CLOCK_CLOCK2_H */


/* [] END OF FILE */
