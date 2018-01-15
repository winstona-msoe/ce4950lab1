/*******************************************************************************
* File Name: ReceiveISR.h
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
#if !defined(CY_ISR_ReceiveISR_H)
#define CY_ISR_ReceiveISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ReceiveISR_Start(void);
void ReceiveISR_StartEx(cyisraddress address);
void ReceiveISR_Stop(void);

CY_ISR_PROTO(ReceiveISR_Interrupt);

void ReceiveISR_SetVector(cyisraddress address);
cyisraddress ReceiveISR_GetVector(void);

void ReceiveISR_SetPriority(uint8 priority);
uint8 ReceiveISR_GetPriority(void);

void ReceiveISR_Enable(void);
uint8 ReceiveISR_GetState(void);
void ReceiveISR_Disable(void);

void ReceiveISR_SetPending(void);
void ReceiveISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ReceiveISR ISR. */
#define ReceiveISR_INTC_VECTOR            ((reg32 *) ReceiveISR__INTC_VECT)

/* Address of the ReceiveISR ISR priority. */
#define ReceiveISR_INTC_PRIOR             ((reg8 *) ReceiveISR__INTC_PRIOR_REG)

/* Priority of the ReceiveISR interrupt. */
#define ReceiveISR_INTC_PRIOR_NUMBER      ReceiveISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ReceiveISR interrupt. */
#define ReceiveISR_INTC_SET_EN            ((reg32 *) ReceiveISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ReceiveISR interrupt. */
#define ReceiveISR_INTC_CLR_EN            ((reg32 *) ReceiveISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ReceiveISR interrupt state to pending. */
#define ReceiveISR_INTC_SET_PD            ((reg32 *) ReceiveISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ReceiveISR interrupt. */
#define ReceiveISR_INTC_CLR_PD            ((reg32 *) ReceiveISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_ReceiveISR_H */


/* [] END OF FILE */
