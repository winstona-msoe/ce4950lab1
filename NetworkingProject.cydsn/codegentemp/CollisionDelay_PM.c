/*******************************************************************************
* File Name: CollisionDelay_PM.c
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

#include "CollisionDelay.h"

static CollisionDelay_backupStruct CollisionDelay_backup;


/*******************************************************************************
* Function Name: CollisionDelay_SaveConfig
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
*  CollisionDelay_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void CollisionDelay_SaveConfig(void) 
{
    #if (!CollisionDelay_UsingFixedFunction)
        CollisionDelay_backup.TimerUdb = CollisionDelay_ReadCounter();
        CollisionDelay_backup.InterruptMaskValue = CollisionDelay_STATUS_MASK;
        #if (CollisionDelay_UsingHWCaptureCounter)
            CollisionDelay_backup.TimerCaptureCounter = CollisionDelay_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!CollisionDelay_UDB_CONTROL_REG_REMOVED)
            CollisionDelay_backup.TimerControlRegister = CollisionDelay_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: CollisionDelay_RestoreConfig
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
*  CollisionDelay_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void CollisionDelay_RestoreConfig(void) 
{   
    #if (!CollisionDelay_UsingFixedFunction)

        CollisionDelay_WriteCounter(CollisionDelay_backup.TimerUdb);
        CollisionDelay_STATUS_MASK =CollisionDelay_backup.InterruptMaskValue;
        #if (CollisionDelay_UsingHWCaptureCounter)
            CollisionDelay_SetCaptureCount(CollisionDelay_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!CollisionDelay_UDB_CONTROL_REG_REMOVED)
            CollisionDelay_WriteControlRegister(CollisionDelay_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: CollisionDelay_Sleep
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
*  CollisionDelay_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void CollisionDelay_Sleep(void) 
{
    #if(!CollisionDelay_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(CollisionDelay_CTRL_ENABLE == (CollisionDelay_CONTROL & CollisionDelay_CTRL_ENABLE))
        {
            /* Timer is enabled */
            CollisionDelay_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            CollisionDelay_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    CollisionDelay_Stop();
    CollisionDelay_SaveConfig();
}


/*******************************************************************************
* Function Name: CollisionDelay_Wakeup
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
*  CollisionDelay_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void CollisionDelay_Wakeup(void) 
{
    CollisionDelay_RestoreConfig();
    #if(!CollisionDelay_UDB_CONTROL_REG_REMOVED)
        if(CollisionDelay_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                CollisionDelay_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
