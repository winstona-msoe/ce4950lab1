/*******************************************************************************
* File Name: ReceiveLight.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ReceiveLight_ALIASES_H) /* Pins ReceiveLight_ALIASES_H */
#define CY_PINS_ReceiveLight_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define ReceiveLight_0			(ReceiveLight__0__PC)
#define ReceiveLight_0_INTR	((uint16)((uint16)0x0001u << ReceiveLight__0__SHIFT))

#define ReceiveLight_INTR_ALL	 ((uint16)(ReceiveLight_0_INTR))

#endif /* End Pins ReceiveLight_ALIASES_H */


/* [] END OF FILE */
