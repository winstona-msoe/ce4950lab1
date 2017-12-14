/*******************************************************************************
* File Name: FallingEdgeISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_FallingEdgeISR_H)
#define CY_ISR_FallingEdgeISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void FallingEdgeISR_Start(void);
void FallingEdgeISR_StartEx(cyisraddress address);
void FallingEdgeISR_Stop(void);

CY_ISR_PROTO(FallingEdgeISR_Interrupt);

void FallingEdgeISR_SetVector(cyisraddress address);
cyisraddress FallingEdgeISR_GetVector(void);

void FallingEdgeISR_SetPriority(uint8 priority);
uint8 FallingEdgeISR_GetPriority(void);

void FallingEdgeISR_Enable(void);
uint8 FallingEdgeISR_GetState(void);
void FallingEdgeISR_Disable(void);

void FallingEdgeISR_SetPending(void);
void FallingEdgeISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the FallingEdgeISR ISR. */
#define FallingEdgeISR_INTC_VECTOR            ((reg32 *) FallingEdgeISR__INTC_VECT)

/* Address of the FallingEdgeISR ISR priority. */
#define FallingEdgeISR_INTC_PRIOR             ((reg8 *) FallingEdgeISR__INTC_PRIOR_REG)

/* Priority of the FallingEdgeISR interrupt. */
#define FallingEdgeISR_INTC_PRIOR_NUMBER      FallingEdgeISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable FallingEdgeISR interrupt. */
#define FallingEdgeISR_INTC_SET_EN            ((reg32 *) FallingEdgeISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the FallingEdgeISR interrupt. */
#define FallingEdgeISR_INTC_CLR_EN            ((reg32 *) FallingEdgeISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the FallingEdgeISR interrupt state to pending. */
#define FallingEdgeISR_INTC_SET_PD            ((reg32 *) FallingEdgeISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the FallingEdgeISR interrupt. */
#define FallingEdgeISR_INTC_CLR_PD            ((reg32 *) FallingEdgeISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_FallingEdgeISR_H */


/* [] END OF FILE */
