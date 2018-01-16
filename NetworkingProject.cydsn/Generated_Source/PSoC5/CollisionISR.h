/*******************************************************************************
* File Name: CollisionISR.h
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
#if !defined(CY_ISR_CollisionISR_H)
#define CY_ISR_CollisionISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void CollisionISR_Start(void);
void CollisionISR_StartEx(cyisraddress address);
void CollisionISR_Stop(void);

CY_ISR_PROTO(CollisionISR_Interrupt);

void CollisionISR_SetVector(cyisraddress address);
cyisraddress CollisionISR_GetVector(void);

void CollisionISR_SetPriority(uint8 priority);
uint8 CollisionISR_GetPriority(void);

void CollisionISR_Enable(void);
uint8 CollisionISR_GetState(void);
void CollisionISR_Disable(void);

void CollisionISR_SetPending(void);
void CollisionISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the CollisionISR ISR. */
#define CollisionISR_INTC_VECTOR            ((reg32 *) CollisionISR__INTC_VECT)

/* Address of the CollisionISR ISR priority. */
#define CollisionISR_INTC_PRIOR             ((reg8 *) CollisionISR__INTC_PRIOR_REG)

/* Priority of the CollisionISR interrupt. */
#define CollisionISR_INTC_PRIOR_NUMBER      CollisionISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable CollisionISR interrupt. */
#define CollisionISR_INTC_SET_EN            ((reg32 *) CollisionISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the CollisionISR interrupt. */
#define CollisionISR_INTC_CLR_EN            ((reg32 *) CollisionISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the CollisionISR interrupt state to pending. */
#define CollisionISR_INTC_SET_PD            ((reg32 *) CollisionISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the CollisionISR interrupt. */
#define CollisionISR_INTC_CLR_PD            ((reg32 *) CollisionISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_CollisionISR_H */


/* [] END OF FILE */
