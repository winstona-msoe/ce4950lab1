/*******************************************************************************
* File Name: RisingISR.h
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
#if !defined(CY_ISR_RisingISR_H)
#define CY_ISR_RisingISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void RisingISR_Start(void);
void RisingISR_StartEx(cyisraddress address);
void RisingISR_Stop(void);

CY_ISR_PROTO(RisingISR_Interrupt);

void RisingISR_SetVector(cyisraddress address);
cyisraddress RisingISR_GetVector(void);

void RisingISR_SetPriority(uint8 priority);
uint8 RisingISR_GetPriority(void);

void RisingISR_Enable(void);
uint8 RisingISR_GetState(void);
void RisingISR_Disable(void);

void RisingISR_SetPending(void);
void RisingISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the RisingISR ISR. */
#define RisingISR_INTC_VECTOR            ((reg32 *) RisingISR__INTC_VECT)

/* Address of the RisingISR ISR priority. */
#define RisingISR_INTC_PRIOR             ((reg8 *) RisingISR__INTC_PRIOR_REG)

/* Priority of the RisingISR interrupt. */
#define RisingISR_INTC_PRIOR_NUMBER      RisingISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable RisingISR interrupt. */
#define RisingISR_INTC_SET_EN            ((reg32 *) RisingISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the RisingISR interrupt. */
#define RisingISR_INTC_CLR_EN            ((reg32 *) RisingISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the RisingISR interrupt state to pending. */
#define RisingISR_INTC_SET_PD            ((reg32 *) RisingISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the RisingISR interrupt. */
#define RisingISR_INTC_CLR_PD            ((reg32 *) RisingISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_RisingISR_H */


/* [] END OF FILE */
