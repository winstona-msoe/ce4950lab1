/*******************************************************************************
* File Name: BackoffISR.h
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
#if !defined(CY_ISR_BackoffISR_H)
#define CY_ISR_BackoffISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void BackoffISR_Start(void);
void BackoffISR_StartEx(cyisraddress address);
void BackoffISR_Stop(void);

CY_ISR_PROTO(BackoffISR_Interrupt);

void BackoffISR_SetVector(cyisraddress address);
cyisraddress BackoffISR_GetVector(void);

void BackoffISR_SetPriority(uint8 priority);
uint8 BackoffISR_GetPriority(void);

void BackoffISR_Enable(void);
uint8 BackoffISR_GetState(void);
void BackoffISR_Disable(void);

void BackoffISR_SetPending(void);
void BackoffISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the BackoffISR ISR. */
#define BackoffISR_INTC_VECTOR            ((reg32 *) BackoffISR__INTC_VECT)

/* Address of the BackoffISR ISR priority. */
#define BackoffISR_INTC_PRIOR             ((reg8 *) BackoffISR__INTC_PRIOR_REG)

/* Priority of the BackoffISR interrupt. */
#define BackoffISR_INTC_PRIOR_NUMBER      BackoffISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable BackoffISR interrupt. */
#define BackoffISR_INTC_SET_EN            ((reg32 *) BackoffISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the BackoffISR interrupt. */
#define BackoffISR_INTC_CLR_EN            ((reg32 *) BackoffISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the BackoffISR interrupt state to pending. */
#define BackoffISR_INTC_SET_PD            ((reg32 *) BackoffISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the BackoffISR interrupt. */
#define BackoffISR_INTC_CLR_PD            ((reg32 *) BackoffISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_BackoffISR_H */


/* [] END OF FILE */
