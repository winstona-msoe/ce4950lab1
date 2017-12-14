/*******************************************************************************
* File Name: IDLE.h  
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

#if !defined(CY_PINS_IDLE_H) /* Pins IDLE_H */
#define CY_PINS_IDLE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "IDLE_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 IDLE__PORT == 15 && ((IDLE__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    IDLE_Write(uint8 value);
void    IDLE_SetDriveMode(uint8 mode);
uint8   IDLE_ReadDataReg(void);
uint8   IDLE_Read(void);
void    IDLE_SetInterruptMode(uint16 position, uint16 mode);
uint8   IDLE_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the IDLE_SetDriveMode() function.
     *  @{
     */
        #define IDLE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define IDLE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define IDLE_DM_RES_UP          PIN_DM_RES_UP
        #define IDLE_DM_RES_DWN         PIN_DM_RES_DWN
        #define IDLE_DM_OD_LO           PIN_DM_OD_LO
        #define IDLE_DM_OD_HI           PIN_DM_OD_HI
        #define IDLE_DM_STRONG          PIN_DM_STRONG
        #define IDLE_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define IDLE_MASK               IDLE__MASK
#define IDLE_SHIFT              IDLE__SHIFT
#define IDLE_WIDTH              1u

/* Interrupt constants */
#if defined(IDLE__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IDLE_SetInterruptMode() function.
     *  @{
     */
        #define IDLE_INTR_NONE      (uint16)(0x0000u)
        #define IDLE_INTR_RISING    (uint16)(0x0001u)
        #define IDLE_INTR_FALLING   (uint16)(0x0002u)
        #define IDLE_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define IDLE_INTR_MASK      (0x01u) 
#endif /* (IDLE__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define IDLE_PS                     (* (reg8 *) IDLE__PS)
/* Data Register */
#define IDLE_DR                     (* (reg8 *) IDLE__DR)
/* Port Number */
#define IDLE_PRT_NUM                (* (reg8 *) IDLE__PRT) 
/* Connect to Analog Globals */                                                  
#define IDLE_AG                     (* (reg8 *) IDLE__AG)                       
/* Analog MUX bux enable */
#define IDLE_AMUX                   (* (reg8 *) IDLE__AMUX) 
/* Bidirectional Enable */                                                        
#define IDLE_BIE                    (* (reg8 *) IDLE__BIE)
/* Bit-mask for Aliased Register Access */
#define IDLE_BIT_MASK               (* (reg8 *) IDLE__BIT_MASK)
/* Bypass Enable */
#define IDLE_BYP                    (* (reg8 *) IDLE__BYP)
/* Port wide control signals */                                                   
#define IDLE_CTL                    (* (reg8 *) IDLE__CTL)
/* Drive Modes */
#define IDLE_DM0                    (* (reg8 *) IDLE__DM0) 
#define IDLE_DM1                    (* (reg8 *) IDLE__DM1)
#define IDLE_DM2                    (* (reg8 *) IDLE__DM2) 
/* Input Buffer Disable Override */
#define IDLE_INP_DIS                (* (reg8 *) IDLE__INP_DIS)
/* LCD Common or Segment Drive */
#define IDLE_LCD_COM_SEG            (* (reg8 *) IDLE__LCD_COM_SEG)
/* Enable Segment LCD */
#define IDLE_LCD_EN                 (* (reg8 *) IDLE__LCD_EN)
/* Slew Rate Control */
#define IDLE_SLW                    (* (reg8 *) IDLE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define IDLE_PRTDSI__CAPS_SEL       (* (reg8 *) IDLE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define IDLE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) IDLE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define IDLE_PRTDSI__OE_SEL0        (* (reg8 *) IDLE__PRTDSI__OE_SEL0) 
#define IDLE_PRTDSI__OE_SEL1        (* (reg8 *) IDLE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define IDLE_PRTDSI__OUT_SEL0       (* (reg8 *) IDLE__PRTDSI__OUT_SEL0) 
#define IDLE_PRTDSI__OUT_SEL1       (* (reg8 *) IDLE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define IDLE_PRTDSI__SYNC_OUT       (* (reg8 *) IDLE__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(IDLE__SIO_CFG)
    #define IDLE_SIO_HYST_EN        (* (reg8 *) IDLE__SIO_HYST_EN)
    #define IDLE_SIO_REG_HIFREQ     (* (reg8 *) IDLE__SIO_REG_HIFREQ)
    #define IDLE_SIO_CFG            (* (reg8 *) IDLE__SIO_CFG)
    #define IDLE_SIO_DIFF           (* (reg8 *) IDLE__SIO_DIFF)
#endif /* (IDLE__SIO_CFG) */

/* Interrupt Registers */
#if defined(IDLE__INTSTAT)
    #define IDLE_INTSTAT            (* (reg8 *) IDLE__INTSTAT)
    #define IDLE_SNAP               (* (reg8 *) IDLE__SNAP)
    
	#define IDLE_0_INTTYPE_REG 		(* (reg8 *) IDLE__0__INTTYPE)
#endif /* (IDLE__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_IDLE_H */


/* [] END OF FILE */
