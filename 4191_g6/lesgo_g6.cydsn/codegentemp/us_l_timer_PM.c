/*******************************************************************************
* File Name: us_l_timer_PM.c
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

#include "us_l_timer.h"

static us_l_timer_backupStruct us_l_timer_backup;


/*******************************************************************************
* Function Name: us_l_timer_SaveConfig
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
*  us_l_timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void us_l_timer_SaveConfig(void) 
{
    #if (!us_l_timer_UsingFixedFunction)
        us_l_timer_backup.TimerUdb = us_l_timer_ReadCounter();
        us_l_timer_backup.InterruptMaskValue = us_l_timer_STATUS_MASK;
        #if (us_l_timer_UsingHWCaptureCounter)
            us_l_timer_backup.TimerCaptureCounter = us_l_timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!us_l_timer_UDB_CONTROL_REG_REMOVED)
            us_l_timer_backup.TimerControlRegister = us_l_timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: us_l_timer_RestoreConfig
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
*  us_l_timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void us_l_timer_RestoreConfig(void) 
{   
    #if (!us_l_timer_UsingFixedFunction)

        us_l_timer_WriteCounter(us_l_timer_backup.TimerUdb);
        us_l_timer_STATUS_MASK =us_l_timer_backup.InterruptMaskValue;
        #if (us_l_timer_UsingHWCaptureCounter)
            us_l_timer_SetCaptureCount(us_l_timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!us_l_timer_UDB_CONTROL_REG_REMOVED)
            us_l_timer_WriteControlRegister(us_l_timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: us_l_timer_Sleep
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
*  us_l_timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void us_l_timer_Sleep(void) 
{
    #if(!us_l_timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(us_l_timer_CTRL_ENABLE == (us_l_timer_CONTROL & us_l_timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            us_l_timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            us_l_timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    us_l_timer_Stop();
    us_l_timer_SaveConfig();
}


/*******************************************************************************
* Function Name: us_l_timer_Wakeup
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
*  us_l_timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void us_l_timer_Wakeup(void) 
{
    us_l_timer_RestoreConfig();
    #if(!us_l_timer_UDB_CONTROL_REG_REMOVED)
        if(us_l_timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                us_l_timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */