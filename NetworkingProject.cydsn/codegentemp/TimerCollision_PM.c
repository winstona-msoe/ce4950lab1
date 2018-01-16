/*******************************************************************************
* File Name: TimerCollision_PM.c
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

#include "TimerCollision.h"

static TimerCollision_backupStruct TimerCollision_backup;


/*******************************************************************************
* Function Name: TimerCollision_SaveConfig
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
*  TimerCollision_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TimerCollision_SaveConfig(void) 
{
    #if (!TimerCollision_UsingFixedFunction)
        TimerCollision_backup.TimerUdb = TimerCollision_ReadCounter();
        TimerCollision_backup.InterruptMaskValue = TimerCollision_STATUS_MASK;
        #if (TimerCollision_UsingHWCaptureCounter)
            TimerCollision_backup.TimerCaptureCounter = TimerCollision_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TimerCollision_UDB_CONTROL_REG_REMOVED)
            TimerCollision_backup.TimerControlRegister = TimerCollision_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TimerCollision_RestoreConfig
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
*  TimerCollision_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TimerCollision_RestoreConfig(void) 
{   
    #if (!TimerCollision_UsingFixedFunction)

        TimerCollision_WriteCounter(TimerCollision_backup.TimerUdb);
        TimerCollision_STATUS_MASK =TimerCollision_backup.InterruptMaskValue;
        #if (TimerCollision_UsingHWCaptureCounter)
            TimerCollision_SetCaptureCount(TimerCollision_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TimerCollision_UDB_CONTROL_REG_REMOVED)
            TimerCollision_WriteControlRegister(TimerCollision_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TimerCollision_Sleep
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
*  TimerCollision_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TimerCollision_Sleep(void) 
{
    #if(!TimerCollision_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TimerCollision_CTRL_ENABLE == (TimerCollision_CONTROL & TimerCollision_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TimerCollision_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TimerCollision_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TimerCollision_Stop();
    TimerCollision_SaveConfig();
}


/*******************************************************************************
* Function Name: TimerCollision_Wakeup
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
*  TimerCollision_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TimerCollision_Wakeup(void) 
{
    TimerCollision_RestoreConfig();
    #if(!TimerCollision_UDB_CONTROL_REG_REMOVED)
        if(TimerCollision_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TimerCollision_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
