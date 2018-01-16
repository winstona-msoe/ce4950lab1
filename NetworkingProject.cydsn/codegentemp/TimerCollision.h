/*******************************************************************************
* File Name: TimerCollision.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_TimerCollision_H)
#define CY_Timer_v2_60_TimerCollision_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TimerCollision_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TimerCollision_Resolution                 8u
#define TimerCollision_UsingFixedFunction         1u
#define TimerCollision_UsingHWCaptureCounter      0u
#define TimerCollision_SoftwareCaptureMode        0u
#define TimerCollision_SoftwareTriggerMode        0u
#define TimerCollision_UsingHWEnable              0u
#define TimerCollision_EnableTriggerMode          0u
#define TimerCollision_InterruptOnCaptureCount    0u
#define TimerCollision_RunModeUsed                0u
#define TimerCollision_ControlRegRemoved          0u

#if defined(TimerCollision_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define TimerCollision_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (TimerCollision_UsingFixedFunction)
    #define TimerCollision_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define TimerCollision_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End TimerCollision_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!TimerCollision_UsingFixedFunction)

        uint8 TimerUdb;
        uint8 InterruptMaskValue;
        #if (TimerCollision_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!TimerCollision_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}TimerCollision_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TimerCollision_Start(void) ;
void    TimerCollision_Stop(void) ;

void    TimerCollision_SetInterruptMode(uint8 interruptMode) ;
uint8   TimerCollision_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TimerCollision_GetInterruptSource() TimerCollision_ReadStatusRegister()

#if(!TimerCollision_UDB_CONTROL_REG_REMOVED)
    uint8   TimerCollision_ReadControlRegister(void) ;
    void    TimerCollision_WriteControlRegister(uint8 control) ;
#endif /* (!TimerCollision_UDB_CONTROL_REG_REMOVED) */

uint8  TimerCollision_ReadPeriod(void) ;
void    TimerCollision_WritePeriod(uint8 period) ;
uint8  TimerCollision_ReadCounter(void) ;
void    TimerCollision_WriteCounter(uint8 counter) ;
uint8  TimerCollision_ReadCapture(void) ;
void    TimerCollision_SoftwareCapture(void) ;

#if(!TimerCollision_UsingFixedFunction) /* UDB Prototypes */
    #if (TimerCollision_SoftwareCaptureMode)
        void    TimerCollision_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TimerCollision_UsingFixedFunction) */

    #if (TimerCollision_SoftwareTriggerMode)
        void    TimerCollision_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TimerCollision_SoftwareTriggerMode) */

    #if (TimerCollision_EnableTriggerMode)
        void    TimerCollision_EnableTrigger(void) ;
        void    TimerCollision_DisableTrigger(void) ;
    #endif /* (TimerCollision_EnableTriggerMode) */


    #if(TimerCollision_InterruptOnCaptureCount)
        void    TimerCollision_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (TimerCollision_InterruptOnCaptureCount) */

    #if (TimerCollision_UsingHWCaptureCounter)
        void    TimerCollision_SetCaptureCount(uint8 captureCount) ;
        uint8   TimerCollision_ReadCaptureCount(void) ;
    #endif /* (TimerCollision_UsingHWCaptureCounter) */

    void TimerCollision_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TimerCollision_Init(void)          ;
void TimerCollision_Enable(void)        ;
void TimerCollision_SaveConfig(void)    ;
void TimerCollision_RestoreConfig(void) ;
void TimerCollision_Sleep(void)         ;
void TimerCollision_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TimerCollision__B_TIMER__CM_NONE 0
#define TimerCollision__B_TIMER__CM_RISINGEDGE 1
#define TimerCollision__B_TIMER__CM_FALLINGEDGE 2
#define TimerCollision__B_TIMER__CM_EITHEREDGE 3
#define TimerCollision__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TimerCollision__B_TIMER__TM_NONE 0x00u
#define TimerCollision__B_TIMER__TM_RISINGEDGE 0x04u
#define TimerCollision__B_TIMER__TM_FALLINGEDGE 0x08u
#define TimerCollision__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TimerCollision__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TimerCollision_INIT_PERIOD             255u
#define TimerCollision_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << TimerCollision_CTRL_CAP_MODE_SHIFT))
#define TimerCollision_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << TimerCollision_CTRL_TRIG_MODE_SHIFT))
#if (TimerCollision_UsingFixedFunction)
    #define TimerCollision_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TimerCollision_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << TimerCollision_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define TimerCollision_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TimerCollision_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimerCollision_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimerCollision_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (TimerCollision_UsingFixedFunction) */
#define TimerCollision_INIT_CAPTURE_COUNT      (2u)
#define TimerCollision_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << TimerCollision_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (TimerCollision_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TimerCollision_STATUS         (*(reg8 *) TimerCollision_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TimerCollision_STATUS_MASK    (*(reg8 *) TimerCollision_TimerHW__SR0 )
    #define TimerCollision_CONTROL        (*(reg8 *) TimerCollision_TimerHW__CFG0)
    #define TimerCollision_CONTROL2       (*(reg8 *) TimerCollision_TimerHW__CFG1)
    #define TimerCollision_CONTROL2_PTR   ( (reg8 *) TimerCollision_TimerHW__CFG1)
    #define TimerCollision_RT1            (*(reg8 *) TimerCollision_TimerHW__RT1)
    #define TimerCollision_RT1_PTR        ( (reg8 *) TimerCollision_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TimerCollision_CONTROL3       (*(reg8 *) TimerCollision_TimerHW__CFG2)
        #define TimerCollision_CONTROL3_PTR   ( (reg8 *) TimerCollision_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TimerCollision_GLOBAL_ENABLE  (*(reg8 *) TimerCollision_TimerHW__PM_ACT_CFG)
    #define TimerCollision_GLOBAL_STBY_ENABLE  (*(reg8 *) TimerCollision_TimerHW__PM_STBY_CFG)

    #define TimerCollision_CAPTURE_LSB         (* (reg16 *) TimerCollision_TimerHW__CAP0 )
    #define TimerCollision_CAPTURE_LSB_PTR       ((reg16 *) TimerCollision_TimerHW__CAP0 )
    #define TimerCollision_PERIOD_LSB          (* (reg16 *) TimerCollision_TimerHW__PER0 )
    #define TimerCollision_PERIOD_LSB_PTR        ((reg16 *) TimerCollision_TimerHW__PER0 )
    #define TimerCollision_COUNTER_LSB         (* (reg16 *) TimerCollision_TimerHW__CNT_CMP0 )
    #define TimerCollision_COUNTER_LSB_PTR       ((reg16 *) TimerCollision_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TimerCollision_BLOCK_EN_MASK                     TimerCollision_TimerHW__PM_ACT_MSK
    #define TimerCollision_BLOCK_STBY_EN_MASK                TimerCollision_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TimerCollision_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TimerCollision_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TimerCollision_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TimerCollision_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TimerCollision_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TimerCollision_CTRL_ENABLE                        ((uint8)((uint8)0x01u << TimerCollision_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TimerCollision_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TimerCollision_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << TimerCollision_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TimerCollision_CTRL_MODE_SHIFT                 0x01u
        #define TimerCollision_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << TimerCollision_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TimerCollision_CTRL_RCOD_SHIFT        0x02u
        #define TimerCollision_CTRL_ENBL_SHIFT        0x00u
        #define TimerCollision_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TimerCollision_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << TimerCollision_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define TimerCollision_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << TimerCollision_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define TimerCollision_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << TimerCollision_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define TimerCollision_CTRL_RCOD       ((uint8)((uint8)0x03u << TimerCollision_CTRL_RCOD_SHIFT))
        #define TimerCollision_CTRL_ENBL       ((uint8)((uint8)0x80u << TimerCollision_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TimerCollision_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TimerCollision_RT1_MASK                        ((uint8)((uint8)0x03u << TimerCollision_RT1_SHIFT))
    #define TimerCollision_SYNC                            ((uint8)((uint8)0x03u << TimerCollision_RT1_SHIFT))
    #define TimerCollision_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TimerCollision_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << TimerCollision_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define TimerCollision_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << TimerCollision_SYNCDSI_SHIFT))

    #define TimerCollision_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << TimerCollision_CTRL_MODE_SHIFT))
    #define TimerCollision_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << TimerCollision_CTRL_MODE_SHIFT))
    #define TimerCollision_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << TimerCollision_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimerCollision_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimerCollision_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimerCollision_STATUS_TC_INT_MASK_SHIFT        (TimerCollision_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimerCollision_STATUS_CAPTURE_INT_MASK_SHIFT   (TimerCollision_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define TimerCollision_STATUS_TC                       ((uint8)((uint8)0x01u << TimerCollision_STATUS_TC_SHIFT))
    #define TimerCollision_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << TimerCollision_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TimerCollision_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << TimerCollision_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TimerCollision_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << TimerCollision_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TimerCollision_STATUS              (* (reg8 *) TimerCollision_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TimerCollision_STATUS_MASK         (* (reg8 *) TimerCollision_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TimerCollision_STATUS_AUX_CTRL     (* (reg8 *) TimerCollision_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TimerCollision_CONTROL             (* (reg8 *) TimerCollision_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(TimerCollision_Resolution <= 8u) /* 8-bit Timer */
        #define TimerCollision_CAPTURE_LSB         (* (reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define TimerCollision_CAPTURE_LSB_PTR       ((reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define TimerCollision_PERIOD_LSB          (* (reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define TimerCollision_PERIOD_LSB_PTR        ((reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define TimerCollision_COUNTER_LSB         (* (reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define TimerCollision_COUNTER_LSB_PTR       ((reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(TimerCollision_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define TimerCollision_CAPTURE_LSB         (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define TimerCollision_CAPTURE_LSB_PTR       ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define TimerCollision_PERIOD_LSB          (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define TimerCollision_PERIOD_LSB_PTR        ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define TimerCollision_COUNTER_LSB         (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define TimerCollision_COUNTER_LSB_PTR       ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define TimerCollision_CAPTURE_LSB         (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define TimerCollision_CAPTURE_LSB_PTR       ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define TimerCollision_PERIOD_LSB          (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define TimerCollision_PERIOD_LSB_PTR        ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define TimerCollision_COUNTER_LSB         (* (reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define TimerCollision_COUNTER_LSB_PTR       ((reg16 *) TimerCollision_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(TimerCollision_Resolution <= 24u)/* 24-bit Timer */
        #define TimerCollision_CAPTURE_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define TimerCollision_CAPTURE_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define TimerCollision_PERIOD_LSB          (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define TimerCollision_PERIOD_LSB_PTR        ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define TimerCollision_COUNTER_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define TimerCollision_COUNTER_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define TimerCollision_CAPTURE_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define TimerCollision_CAPTURE_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define TimerCollision_PERIOD_LSB          (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define TimerCollision_PERIOD_LSB_PTR        ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define TimerCollision_COUNTER_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define TimerCollision_COUNTER_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define TimerCollision_CAPTURE_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define TimerCollision_CAPTURE_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define TimerCollision_PERIOD_LSB          (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define TimerCollision_PERIOD_LSB_PTR        ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define TimerCollision_COUNTER_LSB         (* (reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define TimerCollision_COUNTER_LSB_PTR       ((reg32 *) TimerCollision_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define TimerCollision_COUNTER_LSB_PTR_8BIT       ((reg8 *) TimerCollision_TimerUDB_sT8_timerdp_u0__A0_REG )
    
    #if (TimerCollision_UsingHWCaptureCounter)
        #define TimerCollision_CAP_COUNT              (*(reg8 *) TimerCollision_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimerCollision_CAP_COUNT_PTR          ( (reg8 *) TimerCollision_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimerCollision_CAPTURE_COUNT_CTRL     (*(reg8 *) TimerCollision_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TimerCollision_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TimerCollision_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TimerCollision_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TimerCollision_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TimerCollision_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TimerCollision_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TimerCollision_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TimerCollision_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TimerCollision_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << TimerCollision_CTRL_INTCNT_SHIFT))
    #define TimerCollision_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << TimerCollision_CTRL_TRIG_MODE_SHIFT))
    #define TimerCollision_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << TimerCollision_CTRL_TRIG_EN_SHIFT))
    #define TimerCollision_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << TimerCollision_CTRL_CAP_MODE_SHIFT))
    #define TimerCollision_CTRL_ENABLE                    ((uint8)((uint8)0x01u << TimerCollision_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TimerCollision_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TimerCollision_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TimerCollision_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TimerCollision_STATUS_TC_INT_MASK_SHIFT       TimerCollision_STATUS_TC_SHIFT
    #define TimerCollision_STATUS_CAPTURE_INT_MASK_SHIFT  TimerCollision_STATUS_CAPTURE_SHIFT
    #define TimerCollision_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TimerCollision_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TimerCollision_STATUS_FIFOFULL_INT_MASK_SHIFT TimerCollision_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TimerCollision_STATUS_TC                      ((uint8)((uint8)0x01u << TimerCollision_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define TimerCollision_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << TimerCollision_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerCollision_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << TimerCollision_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerCollision_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << TimerCollision_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TimerCollision_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << TimerCollision_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TimerCollision_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << TimerCollision_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerCollision_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << TimerCollision_STATUS_FIFOFULL_SHIFT))

    #define TimerCollision_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TimerCollision_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TimerCollision_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TimerCollision_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TimerCollision_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TimerCollision_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_TimerCollision_H */


/* [] END OF FILE */
