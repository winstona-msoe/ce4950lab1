/*******************************************************************************
* File Name: BUSY.h  
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

#if !defined(CY_PINS_BUSY_H) /* Pins BUSY_H */
#define CY_PINS_BUSY_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BUSY_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BUSY__PORT == 15 && ((BUSY__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BUSY_Write(uint8 value);
void    BUSY_SetDriveMode(uint8 mode);
uint8   BUSY_ReadDataReg(void);
uint8   BUSY_Read(void);
void    BUSY_SetInterruptMode(uint16 position, uint16 mode);
uint8   BUSY_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BUSY_SetDriveMode() function.
     *  @{
     */
        #define BUSY_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BUSY_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BUSY_DM_RES_UP          PIN_DM_RES_UP
        #define BUSY_DM_RES_DWN         PIN_DM_RES_DWN
        #define BUSY_DM_OD_LO           PIN_DM_OD_LO
        #define BUSY_DM_OD_HI           PIN_DM_OD_HI
        #define BUSY_DM_STRONG          PIN_DM_STRONG
        #define BUSY_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BUSY_MASK               BUSY__MASK
#define BUSY_SHIFT              BUSY__SHIFT
#define BUSY_WIDTH              1u

/* Interrupt constants */
#if defined(BUSY__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BUSY_SetInterruptMode() function.
     *  @{
     */
        #define BUSY_INTR_NONE      (uint16)(0x0000u)
        #define BUSY_INTR_RISING    (uint16)(0x0001u)
        #define BUSY_INTR_FALLING   (uint16)(0x0002u)
        #define BUSY_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BUSY_INTR_MASK      (0x01u) 
#endif /* (BUSY__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BUSY_PS                     (* (reg8 *) BUSY__PS)
/* Data Register */
#define BUSY_DR                     (* (reg8 *) BUSY__DR)
/* Port Number */
#define BUSY_PRT_NUM                (* (reg8 *) BUSY__PRT) 
/* Connect to Analog Globals */                                                  
#define BUSY_AG                     (* (reg8 *) BUSY__AG)                       
/* Analog MUX bux enable */
#define BUSY_AMUX                   (* (reg8 *) BUSY__AMUX) 
/* Bidirectional Enable */                                                        
#define BUSY_BIE                    (* (reg8 *) BUSY__BIE)
/* Bit-mask for Aliased Register Access */
#define BUSY_BIT_MASK               (* (reg8 *) BUSY__BIT_MASK)
/* Bypass Enable */
#define BUSY_BYP                    (* (reg8 *) BUSY__BYP)
/* Port wide control signals */                                                   
#define BUSY_CTL                    (* (reg8 *) BUSY__CTL)
/* Drive Modes */
#define BUSY_DM0                    (* (reg8 *) BUSY__DM0) 
#define BUSY_DM1                    (* (reg8 *) BUSY__DM1)
#define BUSY_DM2                    (* (reg8 *) BUSY__DM2) 
/* Input Buffer Disable Override */
#define BUSY_INP_DIS                (* (reg8 *) BUSY__INP_DIS)
/* LCD Common or Segment Drive */
#define BUSY_LCD_COM_SEG            (* (reg8 *) BUSY__LCD_COM_SEG)
/* Enable Segment LCD */
#define BUSY_LCD_EN                 (* (reg8 *) BUSY__LCD_EN)
/* Slew Rate Control */
#define BUSY_SLW                    (* (reg8 *) BUSY__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BUSY_PRTDSI__CAPS_SEL       (* (reg8 *) BUSY__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BUSY_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BUSY__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BUSY_PRTDSI__OE_SEL0        (* (reg8 *) BUSY__PRTDSI__OE_SEL0) 
#define BUSY_PRTDSI__OE_SEL1        (* (reg8 *) BUSY__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BUSY_PRTDSI__OUT_SEL0       (* (reg8 *) BUSY__PRTDSI__OUT_SEL0) 
#define BUSY_PRTDSI__OUT_SEL1       (* (reg8 *) BUSY__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BUSY_PRTDSI__SYNC_OUT       (* (reg8 *) BUSY__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BUSY__SIO_CFG)
    #define BUSY_SIO_HYST_EN        (* (reg8 *) BUSY__SIO_HYST_EN)
    #define BUSY_SIO_REG_HIFREQ     (* (reg8 *) BUSY__SIO_REG_HIFREQ)
    #define BUSY_SIO_CFG            (* (reg8 *) BUSY__SIO_CFG)
    #define BUSY_SIO_DIFF           (* (reg8 *) BUSY__SIO_DIFF)
#endif /* (BUSY__SIO_CFG) */

/* Interrupt Registers */
#if defined(BUSY__INTSTAT)
    #define BUSY_INTSTAT            (* (reg8 *) BUSY__INTSTAT)
    #define BUSY_SNAP               (* (reg8 *) BUSY__SNAP)
    
	#define BUSY_0_INTTYPE_REG 		(* (reg8 *) BUSY__0__INTTYPE)
#endif /* (BUSY__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BUSY_H */


/* [] END OF FILE */
