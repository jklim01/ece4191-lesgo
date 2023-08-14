/*******************************************************************************
* File Name: p_controller_timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "p_controller_timer.h"

static p_controller_timer_backupStruct p_controller_timer_backup;


/*******************************************************************************
* Function Name: p_controller_timer_SaveConfig
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
*  p_controller_timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void p_controller_timer_SaveConfig(void) 
{
    #if (!p_controller_timer_UsingFixedFunction)
        p_controller_timer_backup.TimerUdb = p_controller_timer_ReadCounter();
        p_controller_timer_backup.InterruptMaskValue = p_controller_timer_STATUS_MASK;
        #if (p_controller_timer_UsingHWCaptureCounter)
            p_controller_timer_backup.TimerCaptureCounter = p_controller_timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!p_controller_timer_UDB_CONTROL_REG_REMOVED)
            p_controller_timer_backup.TimerControlRegister = p_controller_timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: p_controller_timer_RestoreConfig
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
*  p_controller_timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void p_controller_timer_RestoreConfig(void) 
{   
    #if (!p_controller_timer_UsingFixedFunction)

        p_controller_timer_WriteCounter(p_controller_timer_backup.TimerUdb);
        p_controller_timer_STATUS_MASK =p_controller_timer_backup.InterruptMaskValue;
        #if (p_controller_timer_UsingHWCaptureCounter)
            p_controller_timer_SetCaptureCount(p_controller_timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!p_controller_timer_UDB_CONTROL_REG_REMOVED)
            p_controller_timer_WriteControlRegister(p_controller_timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: p_controller_timer_Sleep
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
*  p_controller_timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void p_controller_timer_Sleep(void) 
{
    #if(!p_controller_timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(p_controller_timer_CTRL_ENABLE == (p_controller_timer_CONTROL & p_controller_timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            p_controller_timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            p_controller_timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    p_controller_timer_Stop();
    p_controller_timer_SaveConfig();
}


/*******************************************************************************
* Function Name: p_controller_timer_Wakeup
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
*  p_controller_timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void p_controller_timer_Wakeup(void) 
{
    p_controller_timer_RestoreConfig();
    #if(!p_controller_timer_UDB_CONTROL_REG_REMOVED)
        if(p_controller_timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                p_controller_timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
