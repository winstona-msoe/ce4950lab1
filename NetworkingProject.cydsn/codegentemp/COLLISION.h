/*******************************************************************************
* File Name: COLLISION.h  
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

#if !defined(CY_PINS_COLLISION_H) /* Pins COLLISION_H */
#define CY_PINS_COLLISION_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "COLLISION_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 COLLISION__PORT == 15 && ((COLLISION__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    COLLISION_Write(uint8 value);
void    COLLISION_SetDriveMode(uint8 mode);
uint8   COLLISION_ReadDataReg(void);
uint8   COLLISION_Read(void);
void    COLLISION_SetInterruptMode(uint16 position, uint16 mode);
uint8   COLLISION_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the COLLISION_SetDriveMode() function.
     *  @{
     */
        #define COLLISION_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define COLLISION_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define COLLISION_DM_RES_UP          PIN_DM_RES_UP
        #define COLLISION_DM_RES_DWN         PIN_DM_RES_DWN
        #define COLLISION_DM_OD_LO           PIN_DM_OD_LO
        #define COLLISION_DM_OD_HI           PIN_DM_OD_HI
        #define COLLISION_DM_STRONG          PIN_DM_STRONG
        #define COLLISION_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define COLLISION_MASK               COLLISION__MASK
#define COLLISION_SHIFT              COLLISION__SHIFT
#define COLLISION_WIDTH              1u

/* Interrupt constants */
#if defined(COLLISION__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in COLLISION_SetInterruptMode() function.
     *  @{
     */
        #define COLLISION_INTR_NONE      (uint16)(0x0000u)
        #define COLLISION_INTR_RISING    (uint16)(0x0001u)
        #define COLLISION_INTR_FALLING   (uint16)(0x0002u)
        #define COLLISION_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define COLLISION_INTR_MASK      (0x01u) 
#endif /* (COLLISION__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define COLLISION_PS                     (* (reg8 *) COLLISION__PS)
/* Data Register */
#define COLLISION_DR                     (* (reg8 *) COLLISION__DR)
/* Port Number */
#define COLLISION_PRT_NUM                (* (reg8 *) COLLISION__PRT) 
/* Connect to Analog Globals */                                                  
#define COLLISION_AG                     (* (reg8 *) COLLISION__AG)                       
/* Analog MUX bux enable */
#define COLLISION_AMUX                   (* (reg8 *) COLLISION__AMUX) 
/* Bidirectional Enable */                                                        
#define COLLISION_BIE                    (* (reg8 *) COLLISION__BIE)
/* Bit-mask for Aliased Register Access */
#define COLLISION_BIT_MASK               (* (reg8 *) COLLISION__BIT_MASK)
/* Bypass Enable */
#define COLLISION_BYP                    (* (reg8 *) COLLISION__BYP)
/* Port wide control signals */                                                   
#define COLLISION_CTL                    (* (reg8 *) COLLISION__CTL)
/* Drive Modes */
#define COLLISION_DM0                    (* (reg8 *) COLLISION__DM0) 
#define COLLISION_DM1                    (* (reg8 *) COLLISION__DM1)
#define COLLISION_DM2                    (* (reg8 *) COLLISION__DM2) 
/* Input Buffer Disable Override */
#define COLLISION_INP_DIS                (* (reg8 *) COLLISION__INP_DIS)
/* LCD Common or Segment Drive */
#define COLLISION_LCD_COM_SEG            (* (reg8 *) COLLISION__LCD_COM_SEG)
/* Enable Segment LCD */
#define COLLISION_LCD_EN                 (* (reg8 *) COLLISION__LCD_EN)
/* Slew Rate Control */
#define COLLISION_SLW                    (* (reg8 *) COLLISION__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define COLLISION_PRTDSI__CAPS_SEL       (* (reg8 *) COLLISION__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define COLLISION_PRTDSI__DBL_SYNC_IN    (* (reg8 *) COLLISION__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define COLLISION_PRTDSI__OE_SEL0        (* (reg8 *) COLLISION__PRTDSI__OE_SEL0) 
#define COLLISION_PRTDSI__OE_SEL1        (* (reg8 *) COLLISION__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define COLLISION_PRTDSI__OUT_SEL0       (* (reg8 *) COLLISION__PRTDSI__OUT_SEL0) 
#define COLLISION_PRTDSI__OUT_SEL1       (* (reg8 *) COLLISION__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define COLLISION_PRTDSI__SYNC_OUT       (* (reg8 *) COLLISION__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(COLLISION__SIO_CFG)
    #define COLLISION_SIO_HYST_EN        (* (reg8 *) COLLISION__SIO_HYST_EN)
    #define COLLISION_SIO_REG_HIFREQ     (* (reg8 *) COLLISION__SIO_REG_HIFREQ)
    #define COLLISION_SIO_CFG            (* (reg8 *) COLLISION__SIO_CFG)
    #define COLLISION_SIO_DIFF           (* (reg8 *) COLLISION__SIO_DIFF)
#endif /* (COLLISION__SIO_CFG) */

/* Interrupt Registers */
#if defined(COLLISION__INTSTAT)
    #define COLLISION_INTSTAT            (* (reg8 *) COLLISION__INTSTAT)
    #define COLLISION_SNAP               (* (reg8 *) COLLISION__SNAP)
    
	#define COLLISION_0_INTTYPE_REG 		(* (reg8 *) COLLISION__0__INTTYPE)
#endif /* (COLLISION__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_COLLISION_H */


/* [] END OF FILE */
