/*******************************************************************************
* File Name: CollisionDelayClk.h
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

#if !defined(CY_CLOCK_CollisionDelayClk_H)
#define CY_CLOCK_CollisionDelayClk_H

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

void CollisionDelayClk_Start(void) ;
void CollisionDelayClk_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CollisionDelayClk_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CollisionDelayClk_StandbyPower(uint8 state) ;
void CollisionDelayClk_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CollisionDelayClk_GetDividerRegister(void) ;
void CollisionDelayClk_SetModeRegister(uint8 modeBitMask) ;
void CollisionDelayClk_ClearModeRegister(uint8 modeBitMask) ;
uint8 CollisionDelayClk_GetModeRegister(void) ;
void CollisionDelayClk_SetSourceRegister(uint8 clkSource) ;
uint8 CollisionDelayClk_GetSourceRegister(void) ;
#if defined(CollisionDelayClk__CFG3)
void CollisionDelayClk_SetPhaseRegister(uint8 clkPhase) ;
uint8 CollisionDelayClk_GetPhaseRegister(void) ;
#endif /* defined(CollisionDelayClk__CFG3) */

#define CollisionDelayClk_Enable()                       CollisionDelayClk_Start()
#define CollisionDelayClk_Disable()                      CollisionDelayClk_Stop()
#define CollisionDelayClk_SetDivider(clkDivider)         CollisionDelayClk_SetDividerRegister(clkDivider, 1u)
#define CollisionDelayClk_SetDividerValue(clkDivider)    CollisionDelayClk_SetDividerRegister((clkDivider) - 1u, 1u)
#define CollisionDelayClk_SetMode(clkMode)               CollisionDelayClk_SetModeRegister(clkMode)
#define CollisionDelayClk_SetSource(clkSource)           CollisionDelayClk_SetSourceRegister(clkSource)
#if defined(CollisionDelayClk__CFG3)
#define CollisionDelayClk_SetPhase(clkPhase)             CollisionDelayClk_SetPhaseRegister(clkPhase)
#define CollisionDelayClk_SetPhaseValue(clkPhase)        CollisionDelayClk_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CollisionDelayClk__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CollisionDelayClk_CLKEN              (* (reg8 *) CollisionDelayClk__PM_ACT_CFG)
#define CollisionDelayClk_CLKEN_PTR          ((reg8 *) CollisionDelayClk__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CollisionDelayClk_CLKSTBY            (* (reg8 *) CollisionDelayClk__PM_STBY_CFG)
#define CollisionDelayClk_CLKSTBY_PTR        ((reg8 *) CollisionDelayClk__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CollisionDelayClk_DIV_LSB            (* (reg8 *) CollisionDelayClk__CFG0)
#define CollisionDelayClk_DIV_LSB_PTR        ((reg8 *) CollisionDelayClk__CFG0)
#define CollisionDelayClk_DIV_PTR            ((reg16 *) CollisionDelayClk__CFG0)

/* Clock MSB divider configuration register. */
#define CollisionDelayClk_DIV_MSB            (* (reg8 *) CollisionDelayClk__CFG1)
#define CollisionDelayClk_DIV_MSB_PTR        ((reg8 *) CollisionDelayClk__CFG1)

/* Mode and source configuration register */
#define CollisionDelayClk_MOD_SRC            (* (reg8 *) CollisionDelayClk__CFG2)
#define CollisionDelayClk_MOD_SRC_PTR        ((reg8 *) CollisionDelayClk__CFG2)

#if defined(CollisionDelayClk__CFG3)
/* Analog clock phase configuration register */
#define CollisionDelayClk_PHASE              (* (reg8 *) CollisionDelayClk__CFG3)
#define CollisionDelayClk_PHASE_PTR          ((reg8 *) CollisionDelayClk__CFG3)
#endif /* defined(CollisionDelayClk__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CollisionDelayClk_CLKEN_MASK         CollisionDelayClk__PM_ACT_MSK
#define CollisionDelayClk_CLKSTBY_MASK       CollisionDelayClk__PM_STBY_MSK

/* CFG2 field masks */
#define CollisionDelayClk_SRC_SEL_MSK        CollisionDelayClk__CFG2_SRC_SEL_MASK
#define CollisionDelayClk_MODE_MASK          (~(CollisionDelayClk_SRC_SEL_MSK))

#if defined(CollisionDelayClk__CFG3)
/* CFG3 phase mask */
#define CollisionDelayClk_PHASE_MASK         CollisionDelayClk__CFG3_PHASE_DLY_MASK
#endif /* defined(CollisionDelayClk__CFG3) */

#endif /* CY_CLOCK_CollisionDelayClk_H */


/* [] END OF FILE */
