/*******************************************************************************
* File Name: TRANSMIT.h  
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

#if !defined(CY_PINS_TRANSMIT_H) /* Pins TRANSMIT_H */
#define CY_PINS_TRANSMIT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TRANSMIT_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TRANSMIT__PORT == 15 && ((TRANSMIT__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TRANSMIT_Write(uint8 value);
void    TRANSMIT_SetDriveMode(uint8 mode);
uint8   TRANSMIT_ReadDataReg(void);
uint8   TRANSMIT_Read(void);
void    TRANSMIT_SetInterruptMode(uint16 position, uint16 mode);
uint8   TRANSMIT_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TRANSMIT_SetDriveMode() function.
     *  @{
     */
        #define TRANSMIT_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TRANSMIT_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TRANSMIT_DM_RES_UP          PIN_DM_RES_UP
        #define TRANSMIT_DM_RES_DWN         PIN_DM_RES_DWN
        #define TRANSMIT_DM_OD_LO           PIN_DM_OD_LO
        #define TRANSMIT_DM_OD_HI           PIN_DM_OD_HI
        #define TRANSMIT_DM_STRONG          PIN_DM_STRONG
        #define TRANSMIT_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TRANSMIT_MASK               TRANSMIT__MASK
#define TRANSMIT_SHIFT              TRANSMIT__SHIFT
#define TRANSMIT_WIDTH              1u

/* Interrupt constants */
#if defined(TRANSMIT__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TRANSMIT_SetInterruptMode() function.
     *  @{
     */
        #define TRANSMIT_INTR_NONE      (uint16)(0x0000u)
        #define TRANSMIT_INTR_RISING    (uint16)(0x0001u)
        #define TRANSMIT_INTR_FALLING   (uint16)(0x0002u)
        #define TRANSMIT_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TRANSMIT_INTR_MASK      (0x01u) 
#endif /* (TRANSMIT__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TRANSMIT_PS                     (* (reg8 *) TRANSMIT__PS)
/* Data Register */
#define TRANSMIT_DR                     (* (reg8 *) TRANSMIT__DR)
/* Port Number */
#define TRANSMIT_PRT_NUM                (* (reg8 *) TRANSMIT__PRT) 
/* Connect to Analog Globals */                                                  
#define TRANSMIT_AG                     (* (reg8 *) TRANSMIT__AG)                       
/* Analog MUX bux enable */
#define TRANSMIT_AMUX                   (* (reg8 *) TRANSMIT__AMUX) 
/* Bidirectional Enable */                                                        
#define TRANSMIT_BIE                    (* (reg8 *) TRANSMIT__BIE)
/* Bit-mask for Aliased Register Access */
#define TRANSMIT_BIT_MASK               (* (reg8 *) TRANSMIT__BIT_MASK)
/* Bypass Enable */
#define TRANSMIT_BYP                    (* (reg8 *) TRANSMIT__BYP)
/* Port wide control signals */                                                   
#define TRANSMIT_CTL                    (* (reg8 *) TRANSMIT__CTL)
/* Drive Modes */
#define TRANSMIT_DM0                    (* (reg8 *) TRANSMIT__DM0) 
#define TRANSMIT_DM1                    (* (reg8 *) TRANSMIT__DM1)
#define TRANSMIT_DM2                    (* (reg8 *) TRANSMIT__DM2) 
/* Input Buffer Disable Override */
#define TRANSMIT_INP_DIS                (* (reg8 *) TRANSMIT__INP_DIS)
/* LCD Common or Segment Drive */
#define TRANSMIT_LCD_COM_SEG            (* (reg8 *) TRANSMIT__LCD_COM_SEG)
/* Enable Segment LCD */
#define TRANSMIT_LCD_EN                 (* (reg8 *) TRANSMIT__LCD_EN)
/* Slew Rate Control */
#define TRANSMIT_SLW                    (* (reg8 *) TRANSMIT__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TRANSMIT_PRTDSI__CAPS_SEL       (* (reg8 *) TRANSMIT__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TRANSMIT_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TRANSMIT__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TRANSMIT_PRTDSI__OE_SEL0        (* (reg8 *) TRANSMIT__PRTDSI__OE_SEL0) 
#define TRANSMIT_PRTDSI__OE_SEL1        (* (reg8 *) TRANSMIT__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TRANSMIT_PRTDSI__OUT_SEL0       (* (reg8 *) TRANSMIT__PRTDSI__OUT_SEL0) 
#define TRANSMIT_PRTDSI__OUT_SEL1       (* (reg8 *) TRANSMIT__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TRANSMIT_PRTDSI__SYNC_OUT       (* (reg8 *) TRANSMIT__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TRANSMIT__SIO_CFG)
    #define TRANSMIT_SIO_HYST_EN        (* (reg8 *) TRANSMIT__SIO_HYST_EN)
    #define TRANSMIT_SIO_REG_HIFREQ     (* (reg8 *) TRANSMIT__SIO_REG_HIFREQ)
    #define TRANSMIT_SIO_CFG            (* (reg8 *) TRANSMIT__SIO_CFG)
    #define TRANSMIT_SIO_DIFF           (* (reg8 *) TRANSMIT__SIO_DIFF)
#endif /* (TRANSMIT__SIO_CFG) */

/* Interrupt Registers */
#if defined(TRANSMIT__INTSTAT)
    #define TRANSMIT_INTSTAT            (* (reg8 *) TRANSMIT__INTSTAT)
    #define TRANSMIT_SNAP               (* (reg8 *) TRANSMIT__SNAP)
    
	#define TRANSMIT_0_INTTYPE_REG 		(* (reg8 *) TRANSMIT__0__INTTYPE)
#endif /* (TRANSMIT__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TRANSMIT_H */


/* [] END OF FILE */
