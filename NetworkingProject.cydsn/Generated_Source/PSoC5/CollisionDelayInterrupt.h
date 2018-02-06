/*******************************************************************************
* File Name: CollisionDelayInterrupt.h
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
#if !defined(CY_ISR_CollisionDelayInterrupt_H)
#define CY_ISR_CollisionDelayInterrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void CollisionDelayInterrupt_Start(void);
void CollisionDelayInterrupt_StartEx(cyisraddress address);
void CollisionDelayInterrupt_Stop(void);

CY_ISR_PROTO(CollisionDelayInterrupt_Interrupt);

void CollisionDelayInterrupt_SetVector(cyisraddress address);
cyisraddress CollisionDelayInterrupt_GetVector(void);

void CollisionDelayInterrupt_SetPriority(uint8 priority);
uint8 CollisionDelayInterrupt_GetPriority(void);

void CollisionDelayInterrupt_Enable(void);
uint8 CollisionDelayInterrupt_GetState(void);
void CollisionDelayInterrupt_Disable(void);

void CollisionDelayInterrupt_SetPending(void);
void CollisionDelayInterrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the CollisionDelayInterrupt ISR. */
#define CollisionDelayInterrupt_INTC_VECTOR            ((reg32 *) CollisionDelayInterrupt__INTC_VECT)

/* Address of the CollisionDelayInterrupt ISR priority. */
#define CollisionDelayInterrupt_INTC_PRIOR             ((reg8 *) CollisionDelayInterrupt__INTC_PRIOR_REG)

/* Priority of the CollisionDelayInterrupt interrupt. */
#define CollisionDelayInterrupt_INTC_PRIOR_NUMBER      CollisionDelayInterrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable CollisionDelayInterrupt interrupt. */
#define CollisionDelayInterrupt_INTC_SET_EN            ((reg32 *) CollisionDelayInterrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the CollisionDelayInterrupt interrupt. */
#define CollisionDelayInterrupt_INTC_CLR_EN            ((reg32 *) CollisionDelayInterrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the CollisionDelayInterrupt interrupt state to pending. */
#define CollisionDelayInterrupt_INTC_SET_PD            ((reg32 *) CollisionDelayInterrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the CollisionDelayInterrupt interrupt. */
#define CollisionDelayInterrupt_INTC_CLR_PD            ((reg32 *) CollisionDelayInterrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_CollisionDelayInterrupt_H */


/* [] END OF FILE */
