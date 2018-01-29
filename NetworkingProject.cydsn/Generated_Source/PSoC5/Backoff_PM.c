/*******************************************************************************
* File Name: Backoff_PM.c
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

#include "Backoff.h"

static Backoff_backupStruct Backoff_backup;


/*******************************************************************************
* Function Name: Backoff_SaveConfig
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
*  Backoff_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Backoff_SaveConfig(void) 
{
    #if (!Backoff_UsingFixedFunction)
        Backoff_backup.TimerUdb = Backoff_ReadCounter();
        Backoff_backup.InterruptMaskValue = Backoff_STATUS_MASK;
        #if (Backoff_UsingHWCaptureCounter)
            Backoff_backup.TimerCaptureCounter = Backoff_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Backoff_UDB_CONTROL_REG_REMOVED)
            Backoff_backup.TimerControlRegister = Backoff_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Backoff_RestoreConfig
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
*  Backoff_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Backoff_RestoreConfig(void) 
{   
    #if (!Backoff_UsingFixedFunction)

        Backoff_WriteCounter(Backoff_backup.TimerUdb);
        Backoff_STATUS_MASK =Backoff_backup.InterruptMaskValue;
        #if (Backoff_UsingHWCaptureCounter)
            Backoff_SetCaptureCount(Backoff_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Backoff_UDB_CONTROL_REG_REMOVED)
            Backoff_WriteControlRegister(Backoff_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Backoff_Sleep
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
*  Backoff_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Backoff_Sleep(void) 
{
    #if(!Backoff_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Backoff_CTRL_ENABLE == (Backoff_CONTROL & Backoff_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Backoff_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Backoff_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Backoff_Stop();
    Backoff_SaveConfig();
}


/*******************************************************************************
* Function Name: Backoff_Wakeup
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
*  Backoff_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Backoff_Wakeup(void) 
{
    Backoff_RestoreConfig();
    #if(!Backoff_UDB_CONTROL_REG_REMOVED)
        if(Backoff_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Backoff_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
