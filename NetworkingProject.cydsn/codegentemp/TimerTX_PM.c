/*******************************************************************************
* File Name: TimerTX_PM.c
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

#include "TimerTX.h"

static TimerTX_backupStruct TimerTX_backup;


/*******************************************************************************
* Function Name: TimerTX_SaveConfig
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
*  TimerTX_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TimerTX_SaveConfig(void) 
{
    #if (!TimerTX_UsingFixedFunction)
        TimerTX_backup.TimerUdb = TimerTX_ReadCounter();
        TimerTX_backup.InterruptMaskValue = TimerTX_STATUS_MASK;
        #if (TimerTX_UsingHWCaptureCounter)
            TimerTX_backup.TimerCaptureCounter = TimerTX_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TimerTX_UDB_CONTROL_REG_REMOVED)
            TimerTX_backup.TimerControlRegister = TimerTX_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TimerTX_RestoreConfig
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
*  TimerTX_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TimerTX_RestoreConfig(void) 
{   
    #if (!TimerTX_UsingFixedFunction)

        TimerTX_WriteCounter(TimerTX_backup.TimerUdb);
        TimerTX_STATUS_MASK =TimerTX_backup.InterruptMaskValue;
        #if (TimerTX_UsingHWCaptureCounter)
            TimerTX_SetCaptureCount(TimerTX_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TimerTX_UDB_CONTROL_REG_REMOVED)
            TimerTX_WriteControlRegister(TimerTX_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TimerTX_Sleep
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
*  TimerTX_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TimerTX_Sleep(void) 
{
    #if(!TimerTX_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TimerTX_CTRL_ENABLE == (TimerTX_CONTROL & TimerTX_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TimerTX_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TimerTX_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TimerTX_Stop();
    TimerTX_SaveConfig();
}


/*******************************************************************************
* Function Name: TimerTX_Wakeup
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
*  TimerTX_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TimerTX_Wakeup(void) 
{
    TimerTX_RestoreConfig();
    #if(!TimerTX_UDB_CONTROL_REG_REMOVED)
        if(TimerTX_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TimerTX_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
