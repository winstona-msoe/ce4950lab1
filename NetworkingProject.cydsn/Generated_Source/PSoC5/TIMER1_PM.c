/*******************************************************************************
* File Name: TIMER1_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "TIMER1.h"

static TIMER1_backupStruct TIMER1_backup;


/*******************************************************************************
* Function Name: TIMER1_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TIMER1_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TIMER1_SaveConfig(void) 
{
    #if (!TIMER1_UsingFixedFunction)
        TIMER1_backup.TimerUdb = TIMER1_ReadCounter();
        TIMER1_backup.InterruptMaskValue = TIMER1_STATUS_MASK;
        #if (TIMER1_UsingHWCaptureCounter)
            TIMER1_backup.TimerCaptureCounter = TIMER1_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TIMER1_UDB_CONTROL_REG_REMOVED)
            TIMER1_backup.TimerControlRegister = TIMER1_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TIMER1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TIMER1_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TIMER1_RestoreConfig(void) 
{   
    #if (!TIMER1_UsingFixedFunction)

        TIMER1_WriteCounter(TIMER1_backup.TimerUdb);
        TIMER1_STATUS_MASK =TIMER1_backup.InterruptMaskValue;
        #if (TIMER1_UsingHWCaptureCounter)
            TIMER1_SetCaptureCount(TIMER1_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TIMER1_UDB_CONTROL_REG_REMOVED)
            TIMER1_WriteControlRegister(TIMER1_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TIMER1_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TIMER1_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TIMER1_Sleep(void) 
{
    #if(!TIMER1_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TIMER1_CTRL_ENABLE == (TIMER1_CONTROL & TIMER1_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TIMER1_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TIMER1_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TIMER1_Stop();
    TIMER1_SaveConfig();
}


/*******************************************************************************
* Function Name: TIMER1_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TIMER1_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TIMER1_Wakeup(void) 
{
    TIMER1_RestoreConfig();
    #if(!TIMER1_UDB_CONTROL_REG_REMOVED)
        if(TIMER1_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TIMER1_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
