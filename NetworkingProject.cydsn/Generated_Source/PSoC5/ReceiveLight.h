/*******************************************************************************
* File Name: ReceiveLight.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ReceiveLight_H) /* Pins ReceiveLight_H */
#define CY_PINS_ReceiveLight_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ReceiveLight_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ReceiveLight__PORT == 15 && ((ReceiveLight__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ReceiveLight_Write(uint8 value);
void    ReceiveLight_SetDriveMode(uint8 mode);
uint8   ReceiveLight_ReadDataReg(void);
uint8   ReceiveLight_Read(void);
void    ReceiveLight_SetInterruptMode(uint16 position, uint16 mode);
uint8   ReceiveLight_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ReceiveLight_SetDriveMode() function.
     *  @{
     */
        #define ReceiveLight_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ReceiveLight_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ReceiveLight_DM_RES_UP          PIN_DM_RES_UP
        #define ReceiveLight_DM_RES_DWN         PIN_DM_RES_DWN
        #define ReceiveLight_DM_OD_LO           PIN_DM_OD_LO
        #define ReceiveLight_DM_OD_HI           PIN_DM_OD_HI
        #define ReceiveLight_DM_STRONG          PIN_DM_STRONG
        #define ReceiveLight_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ReceiveLight_MASK               ReceiveLight__MASK
#define ReceiveLight_SHIFT              ReceiveLight__SHIFT
#define ReceiveLight_WIDTH              1u

/* Interrupt constants */
#if defined(ReceiveLight__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ReceiveLight_SetInterruptMode() function.
     *  @{
     */
        #define ReceiveLight_INTR_NONE      (uint16)(0x0000u)
        #define ReceiveLight_INTR_RISING    (uint16)(0x0001u)
        #define ReceiveLight_INTR_FALLING   (uint16)(0x0002u)
        #define ReceiveLight_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ReceiveLight_INTR_MASK      (0x01u) 
#endif /* (ReceiveLight__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ReceiveLight_PS                     (* (reg8 *) ReceiveLight__PS)
/* Data Register */
#define ReceiveLight_DR                     (* (reg8 *) ReceiveLight__DR)
/* Port Number */
#define ReceiveLight_PRT_NUM                (* (reg8 *) ReceiveLight__PRT) 
/* Connect to Analog Globals */                                                  
#define ReceiveLight_AG                     (* (reg8 *) ReceiveLight__AG)                       
/* Analog MUX bux enable */
#define ReceiveLight_AMUX                   (* (reg8 *) ReceiveLight__AMUX) 
/* Bidirectional Enable */                                                        
#define ReceiveLight_BIE                    (* (reg8 *) ReceiveLight__BIE)
/* Bit-mask for Aliased Register Access */
#define ReceiveLight_BIT_MASK               (* (reg8 *) ReceiveLight__BIT_MASK)
/* Bypass Enable */
#define ReceiveLight_BYP                    (* (reg8 *) ReceiveLight__BYP)
/* Port wide control signals */                                                   
#define ReceiveLight_CTL                    (* (reg8 *) ReceiveLight__CTL)
/* Drive Modes */
#define ReceiveLight_DM0                    (* (reg8 *) ReceiveLight__DM0) 
#define ReceiveLight_DM1                    (* (reg8 *) ReceiveLight__DM1)
#define ReceiveLight_DM2                    (* (reg8 *) ReceiveLight__DM2) 
/* Input Buffer Disable Override */
#define ReceiveLight_INP_DIS                (* (reg8 *) ReceiveLight__INP_DIS)
/* LCD Common or Segment Drive */
#define ReceiveLight_LCD_COM_SEG            (* (reg8 *) ReceiveLight__LCD_COM_SEG)
/* Enable Segment LCD */
#define ReceiveLight_LCD_EN                 (* (reg8 *) ReceiveLight__LCD_EN)
/* Slew Rate Control */
#define ReceiveLight_SLW                    (* (reg8 *) ReceiveLight__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ReceiveLight_PRTDSI__CAPS_SEL       (* (reg8 *) ReceiveLight__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ReceiveLight_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ReceiveLight__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ReceiveLight_PRTDSI__OE_SEL0        (* (reg8 *) ReceiveLight__PRTDSI__OE_SEL0) 
#define ReceiveLight_PRTDSI__OE_SEL1        (* (reg8 *) ReceiveLight__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ReceiveLight_PRTDSI__OUT_SEL0       (* (reg8 *) ReceiveLight__PRTDSI__OUT_SEL0) 
#define ReceiveLight_PRTDSI__OUT_SEL1       (* (reg8 *) ReceiveLight__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ReceiveLight_PRTDSI__SYNC_OUT       (* (reg8 *) ReceiveLight__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ReceiveLight__SIO_CFG)
    #define ReceiveLight_SIO_HYST_EN        (* (reg8 *) ReceiveLight__SIO_HYST_EN)
    #define ReceiveLight_SIO_REG_HIFREQ     (* (reg8 *) ReceiveLight__SIO_REG_HIFREQ)
    #define ReceiveLight_SIO_CFG            (* (reg8 *) ReceiveLight__SIO_CFG)
    #define ReceiveLight_SIO_DIFF           (* (reg8 *) ReceiveLight__SIO_DIFF)
#endif /* (ReceiveLight__SIO_CFG) */

/* Interrupt Registers */
#if defined(ReceiveLight__INTSTAT)
    #define ReceiveLight_INTSTAT            (* (reg8 *) ReceiveLight__INTSTAT)
    #define ReceiveLight_SNAP               (* (reg8 *) ReceiveLight__SNAP)
    
	#define ReceiveLight_0_INTTYPE_REG 		(* (reg8 *) ReceiveLight__0__INTTYPE)
#endif /* (ReceiveLight__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ReceiveLight_H */


/* [] END OF FILE */
