/*******************************************************************************
* File Name: CollisionDelayISR.h
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
#if !defined(CY_ISR_CollisionDelayISR_H)
#define CY_ISR_CollisionDelayISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void CollisionDelayISR_Start(void);
void CollisionDelayISR_StartEx(cyisraddress address);
void CollisionDelayISR_Stop(void);

CY_ISR_PROTO(CollisionDelayISR_Interrupt);

void CollisionDelayISR_SetVector(cyisraddress address);
cyisraddress CollisionDelayISR_GetVector(void);

void CollisionDelayISR_SetPriority(uint8 priority);
uint8 CollisionDelayISR_GetPriority(void);

void CollisionDelayISR_Enable(void);
uint8 CollisionDelayISR_GetState(void);
void CollisionDelayISR_Disable(void);

void CollisionDelayISR_SetPending(void);
void CollisionDelayISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the CollisionDelayISR ISR. */
#define CollisionDelayISR_INTC_VECTOR            ((reg32 *) CollisionDelayISR__INTC_VECT)

/* Address of the CollisionDelayISR ISR priority. */
#define CollisionDelayISR_INTC_PRIOR             ((reg8 *) CollisionDelayISR__INTC_PRIOR_REG)

/* Priority of the CollisionDelayISR interrupt. */
#define CollisionDelayISR_INTC_PRIOR_NUMBER      CollisionDelayISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable CollisionDelayISR interrupt. */
#define CollisionDelayISR_INTC_SET_EN            ((reg32 *) CollisionDelayISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the CollisionDelayISR interrupt. */
#define CollisionDelayISR_INTC_CLR_EN            ((reg32 *) CollisionDelayISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the CollisionDelayISR interrupt state to pending. */
#define CollisionDelayISR_INTC_SET_PD            ((reg32 *) CollisionDelayISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the CollisionDelayISR interrupt. */
#define CollisionDelayISR_INTC_CLR_PD            ((reg32 *) CollisionDelayISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_CollisionDelayISR_H */


/* [] END OF FILE */
