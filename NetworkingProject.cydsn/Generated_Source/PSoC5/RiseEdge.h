/*******************************************************************************
* File Name: RiseEdge.h  
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

#if !defined(CY_PINS_RiseEdge_H) /* Pins RiseEdge_H */
#define CY_PINS_RiseEdge_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RiseEdge_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RiseEdge__PORT == 15 && ((RiseEdge__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RiseEdge_Write(uint8 value);
void    RiseEdge_SetDriveMode(uint8 mode);
uint8   RiseEdge_ReadDataReg(void);
uint8   RiseEdge_Read(void);
void    RiseEdge_SetInterruptMode(uint16 position, uint16 mode);
uint8   RiseEdge_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RiseEdge_SetDriveMode() function.
     *  @{
     */
        #define RiseEdge_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RiseEdge_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RiseEdge_DM_RES_UP          PIN_DM_RES_UP
        #define RiseEdge_DM_RES_DWN         PIN_DM_RES_DWN
        #define RiseEdge_DM_OD_LO           PIN_DM_OD_LO
        #define RiseEdge_DM_OD_HI           PIN_DM_OD_HI
        #define RiseEdge_DM_STRONG          PIN_DM_STRONG
        #define RiseEdge_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RiseEdge_MASK               RiseEdge__MASK
#define RiseEdge_SHIFT              RiseEdge__SHIFT
#define RiseEdge_WIDTH              1u

/* Interrupt constants */
#if defined(RiseEdge__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RiseEdge_SetInterruptMode() function.
     *  @{
     */
        #define RiseEdge_INTR_NONE      (uint16)(0x0000u)
        #define RiseEdge_INTR_RISING    (uint16)(0x0001u)
        #define RiseEdge_INTR_FALLING   (uint16)(0x0002u)
        #define RiseEdge_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RiseEdge_INTR_MASK      (0x01u) 
#endif /* (RiseEdge__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RiseEdge_PS                     (* (reg8 *) RiseEdge__PS)
/* Data Register */
#define RiseEdge_DR                     (* (reg8 *) RiseEdge__DR)
/* Port Number */
#define RiseEdge_PRT_NUM                (* (reg8 *) RiseEdge__PRT) 
/* Connect to Analog Globals */                                                  
#define RiseEdge_AG                     (* (reg8 *) RiseEdge__AG)                       
/* Analog MUX bux enable */
#define RiseEdge_AMUX                   (* (reg8 *) RiseEdge__AMUX) 
/* Bidirectional Enable */                                                        
#define RiseEdge_BIE                    (* (reg8 *) RiseEdge__BIE)
/* Bit-mask for Aliased Register Access */
#define RiseEdge_BIT_MASK               (* (reg8 *) RiseEdge__BIT_MASK)
/* Bypass Enable */
#define RiseEdge_BYP                    (* (reg8 *) RiseEdge__BYP)
/* Port wide control signals */                                                   
#define RiseEdge_CTL                    (* (reg8 *) RiseEdge__CTL)
/* Drive Modes */
#define RiseEdge_DM0                    (* (reg8 *) RiseEdge__DM0) 
#define RiseEdge_DM1                    (* (reg8 *) RiseEdge__DM1)
#define RiseEdge_DM2                    (* (reg8 *) RiseEdge__DM2) 
/* Input Buffer Disable Override */
#define RiseEdge_INP_DIS                (* (reg8 *) RiseEdge__INP_DIS)
/* LCD Common or Segment Drive */
#define RiseEdge_LCD_COM_SEG            (* (reg8 *) RiseEdge__LCD_COM_SEG)
/* Enable Segment LCD */
#define RiseEdge_LCD_EN                 (* (reg8 *) RiseEdge__LCD_EN)
/* Slew Rate Control */
#define RiseEdge_SLW                    (* (reg8 *) RiseEdge__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RiseEdge_PRTDSI__CAPS_SEL       (* (reg8 *) RiseEdge__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RiseEdge_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RiseEdge__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RiseEdge_PRTDSI__OE_SEL0        (* (reg8 *) RiseEdge__PRTDSI__OE_SEL0) 
#define RiseEdge_PRTDSI__OE_SEL1        (* (reg8 *) RiseEdge__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RiseEdge_PRTDSI__OUT_SEL0       (* (reg8 *) RiseEdge__PRTDSI__OUT_SEL0) 
#define RiseEdge_PRTDSI__OUT_SEL1       (* (reg8 *) RiseEdge__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RiseEdge_PRTDSI__SYNC_OUT       (* (reg8 *) RiseEdge__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RiseEdge__SIO_CFG)
    #define RiseEdge_SIO_HYST_EN        (* (reg8 *) RiseEdge__SIO_HYST_EN)
    #define RiseEdge_SIO_REG_HIFREQ     (* (reg8 *) RiseEdge__SIO_REG_HIFREQ)
    #define RiseEdge_SIO_CFG            (* (reg8 *) RiseEdge__SIO_CFG)
    #define RiseEdge_SIO_DIFF           (* (reg8 *) RiseEdge__SIO_DIFF)
#endif /* (RiseEdge__SIO_CFG) */

/* Interrupt Registers */
#if defined(RiseEdge__INTSTAT)
    #define RiseEdge_INTSTAT            (* (reg8 *) RiseEdge__INTSTAT)
    #define RiseEdge_SNAP               (* (reg8 *) RiseEdge__SNAP)
    
	#define RiseEdge_0_INTTYPE_REG 		(* (reg8 *) RiseEdge__0__INTTYPE)
#endif /* (RiseEdge__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RiseEdge_H */


/* [] END OF FILE */
