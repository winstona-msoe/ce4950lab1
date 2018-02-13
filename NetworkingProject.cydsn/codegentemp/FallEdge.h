/*******************************************************************************
* File Name: FallEdge.h  
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

#if !defined(CY_PINS_FallEdge_H) /* Pins FallEdge_H */
#define CY_PINS_FallEdge_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "FallEdge_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 FallEdge__PORT == 15 && ((FallEdge__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    FallEdge_Write(uint8 value);
void    FallEdge_SetDriveMode(uint8 mode);
uint8   FallEdge_ReadDataReg(void);
uint8   FallEdge_Read(void);
void    FallEdge_SetInterruptMode(uint16 position, uint16 mode);
uint8   FallEdge_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the FallEdge_SetDriveMode() function.
     *  @{
     */
        #define FallEdge_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define FallEdge_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define FallEdge_DM_RES_UP          PIN_DM_RES_UP
        #define FallEdge_DM_RES_DWN         PIN_DM_RES_DWN
        #define FallEdge_DM_OD_LO           PIN_DM_OD_LO
        #define FallEdge_DM_OD_HI           PIN_DM_OD_HI
        #define FallEdge_DM_STRONG          PIN_DM_STRONG
        #define FallEdge_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define FallEdge_MASK               FallEdge__MASK
#define FallEdge_SHIFT              FallEdge__SHIFT
#define FallEdge_WIDTH              1u

/* Interrupt constants */
#if defined(FallEdge__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FallEdge_SetInterruptMode() function.
     *  @{
     */
        #define FallEdge_INTR_NONE      (uint16)(0x0000u)
        #define FallEdge_INTR_RISING    (uint16)(0x0001u)
        #define FallEdge_INTR_FALLING   (uint16)(0x0002u)
        #define FallEdge_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define FallEdge_INTR_MASK      (0x01u) 
#endif /* (FallEdge__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define FallEdge_PS                     (* (reg8 *) FallEdge__PS)
/* Data Register */
#define FallEdge_DR                     (* (reg8 *) FallEdge__DR)
/* Port Number */
#define FallEdge_PRT_NUM                (* (reg8 *) FallEdge__PRT) 
/* Connect to Analog Globals */                                                  
#define FallEdge_AG                     (* (reg8 *) FallEdge__AG)                       
/* Analog MUX bux enable */
#define FallEdge_AMUX                   (* (reg8 *) FallEdge__AMUX) 
/* Bidirectional Enable */                                                        
#define FallEdge_BIE                    (* (reg8 *) FallEdge__BIE)
/* Bit-mask for Aliased Register Access */
#define FallEdge_BIT_MASK               (* (reg8 *) FallEdge__BIT_MASK)
/* Bypass Enable */
#define FallEdge_BYP                    (* (reg8 *) FallEdge__BYP)
/* Port wide control signals */                                                   
#define FallEdge_CTL                    (* (reg8 *) FallEdge__CTL)
/* Drive Modes */
#define FallEdge_DM0                    (* (reg8 *) FallEdge__DM0) 
#define FallEdge_DM1                    (* (reg8 *) FallEdge__DM1)
#define FallEdge_DM2                    (* (reg8 *) FallEdge__DM2) 
/* Input Buffer Disable Override */
#define FallEdge_INP_DIS                (* (reg8 *) FallEdge__INP_DIS)
/* LCD Common or Segment Drive */
#define FallEdge_LCD_COM_SEG            (* (reg8 *) FallEdge__LCD_COM_SEG)
/* Enable Segment LCD */
#define FallEdge_LCD_EN                 (* (reg8 *) FallEdge__LCD_EN)
/* Slew Rate Control */
#define FallEdge_SLW                    (* (reg8 *) FallEdge__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define FallEdge_PRTDSI__CAPS_SEL       (* (reg8 *) FallEdge__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define FallEdge_PRTDSI__DBL_SYNC_IN    (* (reg8 *) FallEdge__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define FallEdge_PRTDSI__OE_SEL0        (* (reg8 *) FallEdge__PRTDSI__OE_SEL0) 
#define FallEdge_PRTDSI__OE_SEL1        (* (reg8 *) FallEdge__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define FallEdge_PRTDSI__OUT_SEL0       (* (reg8 *) FallEdge__PRTDSI__OUT_SEL0) 
#define FallEdge_PRTDSI__OUT_SEL1       (* (reg8 *) FallEdge__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define FallEdge_PRTDSI__SYNC_OUT       (* (reg8 *) FallEdge__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(FallEdge__SIO_CFG)
    #define FallEdge_SIO_HYST_EN        (* (reg8 *) FallEdge__SIO_HYST_EN)
    #define FallEdge_SIO_REG_HIFREQ     (* (reg8 *) FallEdge__SIO_REG_HIFREQ)
    #define FallEdge_SIO_CFG            (* (reg8 *) FallEdge__SIO_CFG)
    #define FallEdge_SIO_DIFF           (* (reg8 *) FallEdge__SIO_DIFF)
#endif /* (FallEdge__SIO_CFG) */

/* Interrupt Registers */
#if defined(FallEdge__INTSTAT)
    #define FallEdge_INTSTAT            (* (reg8 *) FallEdge__INTSTAT)
    #define FallEdge_SNAP               (* (reg8 *) FallEdge__SNAP)
    
	#define FallEdge_0_INTTYPE_REG 		(* (reg8 *) FallEdge__0__INTTYPE)
#endif /* (FallEdge__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_FallEdge_H */


/* [] END OF FILE */
