/*******************************************************************************
* File Name: TimerRX.h
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

#if !defined(CY_Timer_v2_60_TimerRX_H)
#define CY_Timer_v2_60_TimerRX_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TimerRX_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TimerRX_Resolution                 16u
#define TimerRX_UsingFixedFunction         0u
#define TimerRX_UsingHWCaptureCounter      1u
#define TimerRX_SoftwareCaptureMode        0u
#define TimerRX_SoftwareTriggerMode        0u
#define TimerRX_UsingHWEnable              0u
#define TimerRX_EnableTriggerMode          0u
#define TimerRX_InterruptOnCaptureCount    0u
#define TimerRX_RunModeUsed                0u
#define TimerRX_ControlRegRemoved          0u

#if defined(TimerRX_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define TimerRX_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (TimerRX_UsingFixedFunction)
    #define TimerRX_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define TimerRX_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End TimerRX_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!TimerRX_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (TimerRX_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!TimerRX_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}TimerRX_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TimerRX_Start(void) ;
void    TimerRX_Stop(void) ;

void    TimerRX_SetInterruptMode(uint8 interruptMode) ;
uint8   TimerRX_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TimerRX_GetInterruptSource() TimerRX_ReadStatusRegister()

#if(!TimerRX_UDB_CONTROL_REG_REMOVED)
    uint8   TimerRX_ReadControlRegister(void) ;
    void    TimerRX_WriteControlRegister(uint8 control) ;
#endif /* (!TimerRX_UDB_CONTROL_REG_REMOVED) */

uint16  TimerRX_ReadPeriod(void) ;
void    TimerRX_WritePeriod(uint16 period) ;
uint16  TimerRX_ReadCounter(void) ;
void    TimerRX_WriteCounter(uint16 counter) ;
uint16  TimerRX_ReadCapture(void) ;
void    TimerRX_SoftwareCapture(void) ;

#if(!TimerRX_UsingFixedFunction) /* UDB Prototypes */
    #if (TimerRX_SoftwareCaptureMode)
        void    TimerRX_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TimerRX_UsingFixedFunction) */

    #if (TimerRX_SoftwareTriggerMode)
        void    TimerRX_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TimerRX_SoftwareTriggerMode) */

    #if (TimerRX_EnableTriggerMode)
        void    TimerRX_EnableTrigger(void) ;
        void    TimerRX_DisableTrigger(void) ;
    #endif /* (TimerRX_EnableTriggerMode) */


    #if(TimerRX_InterruptOnCaptureCount)
        void    TimerRX_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (TimerRX_InterruptOnCaptureCount) */

    #if (TimerRX_UsingHWCaptureCounter)
        void    TimerRX_SetCaptureCount(uint8 captureCount) ;
        uint8   TimerRX_ReadCaptureCount(void) ;
    #endif /* (TimerRX_UsingHWCaptureCounter) */

    void TimerRX_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TimerRX_Init(void)          ;
void TimerRX_Enable(void)        ;
void TimerRX_SaveConfig(void)    ;
void TimerRX_RestoreConfig(void) ;
void TimerRX_Sleep(void)         ;
void TimerRX_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TimerRX__B_TIMER__CM_NONE 0
#define TimerRX__B_TIMER__CM_RISINGEDGE 1
#define TimerRX__B_TIMER__CM_FALLINGEDGE 2
#define TimerRX__B_TIMER__CM_EITHEREDGE 3
#define TimerRX__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TimerRX__B_TIMER__TM_NONE 0x00u
#define TimerRX__B_TIMER__TM_RISINGEDGE 0x04u
#define TimerRX__B_TIMER__TM_FALLINGEDGE 0x08u
#define TimerRX__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TimerRX__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TimerRX_INIT_PERIOD             9u
#define TimerRX_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << TimerRX_CTRL_CAP_MODE_SHIFT))
#define TimerRX_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << TimerRX_CTRL_TRIG_MODE_SHIFT))
#if (TimerRX_UsingFixedFunction)
    #define TimerRX_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << TimerRX_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << TimerRX_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define TimerRX_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << TimerRX_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimerRX_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimerRX_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (TimerRX_UsingFixedFunction) */
#define TimerRX_INIT_CAPTURE_COUNT      (2u)
#define TimerRX_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << TimerRX_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (TimerRX_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TimerRX_STATUS         (*(reg8 *) TimerRX_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TimerRX_STATUS_MASK    (*(reg8 *) TimerRX_TimerHW__SR0 )
    #define TimerRX_CONTROL        (*(reg8 *) TimerRX_TimerHW__CFG0)
    #define TimerRX_CONTROL2       (*(reg8 *) TimerRX_TimerHW__CFG1)
    #define TimerRX_CONTROL2_PTR   ( (reg8 *) TimerRX_TimerHW__CFG1)
    #define TimerRX_RT1            (*(reg8 *) TimerRX_TimerHW__RT1)
    #define TimerRX_RT1_PTR        ( (reg8 *) TimerRX_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TimerRX_CONTROL3       (*(reg8 *) TimerRX_TimerHW__CFG2)
        #define TimerRX_CONTROL3_PTR   ( (reg8 *) TimerRX_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TimerRX_GLOBAL_ENABLE  (*(reg8 *) TimerRX_TimerHW__PM_ACT_CFG)
    #define TimerRX_GLOBAL_STBY_ENABLE  (*(reg8 *) TimerRX_TimerHW__PM_STBY_CFG)

    #define TimerRX_CAPTURE_LSB         (* (reg16 *) TimerRX_TimerHW__CAP0 )
    #define TimerRX_CAPTURE_LSB_PTR       ((reg16 *) TimerRX_TimerHW__CAP0 )
    #define TimerRX_PERIOD_LSB          (* (reg16 *) TimerRX_TimerHW__PER0 )
    #define TimerRX_PERIOD_LSB_PTR        ((reg16 *) TimerRX_TimerHW__PER0 )
    #define TimerRX_COUNTER_LSB         (* (reg16 *) TimerRX_TimerHW__CNT_CMP0 )
    #define TimerRX_COUNTER_LSB_PTR       ((reg16 *) TimerRX_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TimerRX_BLOCK_EN_MASK                     TimerRX_TimerHW__PM_ACT_MSK
    #define TimerRX_BLOCK_STBY_EN_MASK                TimerRX_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TimerRX_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TimerRX_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TimerRX_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TimerRX_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TimerRX_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TimerRX_CTRL_ENABLE                        ((uint8)((uint8)0x01u << TimerRX_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TimerRX_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TimerRX_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << TimerRX_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TimerRX_CTRL_MODE_SHIFT                 0x01u
        #define TimerRX_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << TimerRX_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TimerRX_CTRL_RCOD_SHIFT        0x02u
        #define TimerRX_CTRL_ENBL_SHIFT        0x00u
        #define TimerRX_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TimerRX_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << TimerRX_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define TimerRX_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << TimerRX_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define TimerRX_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << TimerRX_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define TimerRX_CTRL_RCOD       ((uint8)((uint8)0x03u << TimerRX_CTRL_RCOD_SHIFT))
        #define TimerRX_CTRL_ENBL       ((uint8)((uint8)0x80u << TimerRX_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TimerRX_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TimerRX_RT1_MASK                        ((uint8)((uint8)0x03u << TimerRX_RT1_SHIFT))
    #define TimerRX_SYNC                            ((uint8)((uint8)0x03u << TimerRX_RT1_SHIFT))
    #define TimerRX_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TimerRX_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << TimerRX_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define TimerRX_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << TimerRX_SYNCDSI_SHIFT))

    #define TimerRX_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << TimerRX_CTRL_MODE_SHIFT))
    #define TimerRX_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << TimerRX_CTRL_MODE_SHIFT))
    #define TimerRX_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << TimerRX_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimerRX_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimerRX_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimerRX_STATUS_TC_INT_MASK_SHIFT        (TimerRX_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimerRX_STATUS_CAPTURE_INT_MASK_SHIFT   (TimerRX_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define TimerRX_STATUS_TC                       ((uint8)((uint8)0x01u << TimerRX_STATUS_TC_SHIFT))
    #define TimerRX_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << TimerRX_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TimerRX_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << TimerRX_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TimerRX_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << TimerRX_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TimerRX_STATUS              (* (reg8 *) TimerRX_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TimerRX_STATUS_MASK         (* (reg8 *) TimerRX_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TimerRX_STATUS_AUX_CTRL     (* (reg8 *) TimerRX_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TimerRX_CONTROL             (* (reg8 *) TimerRX_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(TimerRX_Resolution <= 8u) /* 8-bit Timer */
        #define TimerRX_CAPTURE_LSB         (* (reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TimerRX_CAPTURE_LSB_PTR       ((reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TimerRX_PERIOD_LSB          (* (reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TimerRX_PERIOD_LSB_PTR        ((reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TimerRX_COUNTER_LSB         (* (reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TimerRX_COUNTER_LSB_PTR       ((reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(TimerRX_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define TimerRX_CAPTURE_LSB         (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TimerRX_CAPTURE_LSB_PTR       ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TimerRX_PERIOD_LSB          (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TimerRX_PERIOD_LSB_PTR        ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TimerRX_COUNTER_LSB         (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TimerRX_COUNTER_LSB_PTR       ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define TimerRX_CAPTURE_LSB         (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TimerRX_CAPTURE_LSB_PTR       ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TimerRX_PERIOD_LSB          (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TimerRX_PERIOD_LSB_PTR        ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TimerRX_COUNTER_LSB         (* (reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define TimerRX_COUNTER_LSB_PTR       ((reg16 *) TimerRX_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(TimerRX_Resolution <= 24u)/* 24-bit Timer */
        #define TimerRX_CAPTURE_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TimerRX_CAPTURE_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TimerRX_PERIOD_LSB          (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TimerRX_PERIOD_LSB_PTR        ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TimerRX_COUNTER_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TimerRX_COUNTER_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define TimerRX_CAPTURE_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TimerRX_CAPTURE_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TimerRX_PERIOD_LSB          (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TimerRX_PERIOD_LSB_PTR        ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TimerRX_COUNTER_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TimerRX_COUNTER_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define TimerRX_CAPTURE_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TimerRX_CAPTURE_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TimerRX_PERIOD_LSB          (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TimerRX_PERIOD_LSB_PTR        ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TimerRX_COUNTER_LSB         (* (reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define TimerRX_COUNTER_LSB_PTR       ((reg32 *) TimerRX_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define TimerRX_COUNTER_LSB_PTR_8BIT       ((reg8 *) TimerRX_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (TimerRX_UsingHWCaptureCounter)
        #define TimerRX_CAP_COUNT              (*(reg8 *) TimerRX_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimerRX_CAP_COUNT_PTR          ( (reg8 *) TimerRX_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimerRX_CAPTURE_COUNT_CTRL     (*(reg8 *) TimerRX_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TimerRX_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TimerRX_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TimerRX_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TimerRX_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TimerRX_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TimerRX_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TimerRX_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TimerRX_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TimerRX_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << TimerRX_CTRL_INTCNT_SHIFT))
    #define TimerRX_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << TimerRX_CTRL_TRIG_MODE_SHIFT))
    #define TimerRX_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << TimerRX_CTRL_TRIG_EN_SHIFT))
    #define TimerRX_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << TimerRX_CTRL_CAP_MODE_SHIFT))
    #define TimerRX_CTRL_ENABLE                    ((uint8)((uint8)0x01u << TimerRX_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TimerRX_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TimerRX_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TimerRX_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TimerRX_STATUS_TC_INT_MASK_SHIFT       TimerRX_STATUS_TC_SHIFT
    #define TimerRX_STATUS_CAPTURE_INT_MASK_SHIFT  TimerRX_STATUS_CAPTURE_SHIFT
    #define TimerRX_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TimerRX_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TimerRX_STATUS_FIFOFULL_INT_MASK_SHIFT TimerRX_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TimerRX_STATUS_TC                      ((uint8)((uint8)0x01u << TimerRX_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define TimerRX_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << TimerRX_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerRX_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << TimerRX_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerRX_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << TimerRX_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TimerRX_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << TimerRX_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TimerRX_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << TimerRX_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimerRX_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << TimerRX_STATUS_FIFOFULL_SHIFT))

    #define TimerRX_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TimerRX_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TimerRX_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TimerRX_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TimerRX_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TimerRX_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_TimerRX_H */


/* [] END OF FILE */
