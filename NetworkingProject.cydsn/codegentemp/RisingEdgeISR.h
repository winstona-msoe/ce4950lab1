/*******************************************************************************
* File Name: RisingEdgeISR.h
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
#if !defined(CY_ISR_RisingEdgeISR_H)
#define CY_ISR_RisingEdgeISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void RisingEdgeISR_Start(void);
void RisingEdgeISR_StartEx(cyisraddress address);
void RisingEdgeISR_Stop(void);

CY_ISR_PROTO(RisingEdgeISR_Interrupt);

void RisingEdgeISR_SetVector(cyisraddress address);
cyisraddress RisingEdgeISR_GetVector(void);

void RisingEdgeISR_SetPriority(uint8 priority);
uint8 RisingEdgeISR_GetPriority(void);

void RisingEdgeISR_Enable(void);
uint8 RisingEdgeISR_GetState(void);
void RisingEdgeISR_Disable(void);

void RisingEdgeISR_SetPending(void);
void RisingEdgeISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the RisingEdgeISR ISR. */
#define RisingEdgeISR_INTC_VECTOR            ((reg32 *) RisingEdgeISR__INTC_VECT)

/* Address of the RisingEdgeISR ISR priority. */
#define RisingEdgeISR_INTC_PRIOR             ((reg8 *) RisingEdgeISR__INTC_PRIOR_REG)

/* Priority of the RisingEdgeISR interrupt. */
#define RisingEdgeISR_INTC_PRIOR_NUMBER      RisingEdgeISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable RisingEdgeISR interrupt. */
#define RisingEdgeISR_INTC_SET_EN            ((reg32 *) RisingEdgeISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the RisingEdgeISR interrupt. */
#define RisingEdgeISR_INTC_CLR_EN            ((reg32 *) RisingEdgeISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the RisingEdgeISR interrupt state to pending. */
#define RisingEdgeISR_INTC_SET_PD            ((reg32 *) RisingEdgeISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the RisingEdgeISR interrupt. */
#define RisingEdgeISR_INTC_CLR_PD            ((reg32 *) RisingEdgeISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_RisingEdgeISR_H */


/* [] END OF FILE */
