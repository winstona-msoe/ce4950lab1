/*******************************************************************************
* File Name: TimerRX_PM.c
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

#include "TimerRX.h"

static TimerRX_backupStruct TimerRX_backup;


/*******************************************************************************
* Function Name: TimerRX_SaveConfig
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
*  TimerRX_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TimerRX_SaveConfig(void) 
{
    #if (!TimerRX_UsingFixedFunction)
        TimerRX_backup.TimerUdb = TimerRX_ReadCounter();
        TimerRX_backup.InterruptMaskValue = TimerRX_STATUS_MASK;
        #if (TimerRX_UsingHWCaptureCounter)
            TimerRX_backup.TimerCaptureCounter = TimerRX_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TimerRX_UDB_CONTROL_REG_REMOVED)
            TimerRX_backup.TimerControlRegister = TimerRX_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TimerRX_RestoreConfig
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
*  TimerRX_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TimerRX_RestoreConfig(void) 
{   
    #if (!TimerRX_UsingFixedFunction)

        TimerRX_WriteCounter(TimerRX_backup.TimerUdb);
        TimerRX_STATUS_MASK =TimerRX_backup.InterruptMaskValue;
        #if (TimerRX_UsingHWCaptureCounter)
            TimerRX_SetCaptureCount(TimerRX_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TimerRX_UDB_CONTROL_REG_REMOVED)
            TimerRX_WriteControlRegister(TimerRX_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TimerRX_Sleep
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
*  TimerRX_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TimerRX_Sleep(void) 
{
    #if(!TimerRX_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TimerRX_CTRL_ENABLE == (TimerRX_CONTROL & TimerRX_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TimerRX_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TimerRX_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TimerRX_Stop();
    TimerRX_SaveConfig();
}


/*******************************************************************************
* Function Name: TimerRX_Wakeup
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
*  TimerRX_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TimerRX_Wakeup(void) 
{
    TimerRX_RestoreConfig();
    #if(!TimerRX_UDB_CONTROL_REG_REMOVED)
        if(TimerRX_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TimerRX_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
