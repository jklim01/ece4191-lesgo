/*******************************************************************************
* File Name: us_trigger_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "us_trigger.h"

static us_trigger_backupStruct us_trigger_backup;


/*******************************************************************************
* Function Name: us_trigger_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  us_trigger_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void us_trigger_SaveConfig(void) 
{

    #if(!us_trigger_UsingFixedFunction)
        #if(!us_trigger_PWMModeIsCenterAligned)
            us_trigger_backup.PWMPeriod = us_trigger_ReadPeriod();
        #endif /* (!us_trigger_PWMModeIsCenterAligned) */
        us_trigger_backup.PWMUdb = us_trigger_ReadCounter();
        #if (us_trigger_UseStatus)
            us_trigger_backup.InterruptMaskValue = us_trigger_STATUS_MASK;
        #endif /* (us_trigger_UseStatus) */

        #if(us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_256_CLOCKS || \
            us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_2_4_CLOCKS)
            us_trigger_backup.PWMdeadBandValue = us_trigger_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(us_trigger_KillModeMinTime)
             us_trigger_backup.PWMKillCounterPeriod = us_trigger_ReadKillTime();
        #endif /* (us_trigger_KillModeMinTime) */

        #if(us_trigger_UseControl)
            us_trigger_backup.PWMControlRegister = us_trigger_ReadControlRegister();
        #endif /* (us_trigger_UseControl) */
    #endif  /* (!us_trigger_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: us_trigger_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  us_trigger_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void us_trigger_RestoreConfig(void) 
{
        #if(!us_trigger_UsingFixedFunction)
            #if(!us_trigger_PWMModeIsCenterAligned)
                us_trigger_WritePeriod(us_trigger_backup.PWMPeriod);
            #endif /* (!us_trigger_PWMModeIsCenterAligned) */

            us_trigger_WriteCounter(us_trigger_backup.PWMUdb);

            #if (us_trigger_UseStatus)
                us_trigger_STATUS_MASK = us_trigger_backup.InterruptMaskValue;
            #endif /* (us_trigger_UseStatus) */

            #if(us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_256_CLOCKS || \
                us_trigger_DeadBandMode == us_trigger__B_PWM__DBM_2_4_CLOCKS)
                us_trigger_WriteDeadTime(us_trigger_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(us_trigger_KillModeMinTime)
                us_trigger_WriteKillTime(us_trigger_backup.PWMKillCounterPeriod);
            #endif /* (us_trigger_KillModeMinTime) */

            #if(us_trigger_UseControl)
                us_trigger_WriteControlRegister(us_trigger_backup.PWMControlRegister);
            #endif /* (us_trigger_UseControl) */
        #endif  /* (!us_trigger_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: us_trigger_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  us_trigger_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void us_trigger_Sleep(void) 
{
    #if(us_trigger_UseControl)
        if(us_trigger_CTRL_ENABLE == (us_trigger_CONTROL & us_trigger_CTRL_ENABLE))
        {
            /*Component is enabled */
            us_trigger_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            us_trigger_backup.PWMEnableState = 0u;
        }
    #endif /* (us_trigger_UseControl) */

    /* Stop component */
    us_trigger_Stop();

    /* Save registers configuration */
    us_trigger_SaveConfig();
}


/*******************************************************************************
* Function Name: us_trigger_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  us_trigger_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void us_trigger_Wakeup(void) 
{
     /* Restore registers values */
    us_trigger_RestoreConfig();

    if(us_trigger_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        us_trigger_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
