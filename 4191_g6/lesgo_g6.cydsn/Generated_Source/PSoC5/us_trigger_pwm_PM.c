/*******************************************************************************
* File Name: us_trigger_pwm_PM.c
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

#include "us_trigger_pwm.h"

static us_trigger_pwm_backupStruct us_trigger_pwm_backup;


/*******************************************************************************
* Function Name: us_trigger_pwm_SaveConfig
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
*  us_trigger_pwm_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void us_trigger_pwm_SaveConfig(void) 
{

    #if(!us_trigger_pwm_UsingFixedFunction)
        #if(!us_trigger_pwm_PWMModeIsCenterAligned)
            us_trigger_pwm_backup.PWMPeriod = us_trigger_pwm_ReadPeriod();
        #endif /* (!us_trigger_pwm_PWMModeIsCenterAligned) */
        us_trigger_pwm_backup.PWMUdb = us_trigger_pwm_ReadCounter();
        #if (us_trigger_pwm_UseStatus)
            us_trigger_pwm_backup.InterruptMaskValue = us_trigger_pwm_STATUS_MASK;
        #endif /* (us_trigger_pwm_UseStatus) */

        #if(us_trigger_pwm_DeadBandMode == us_trigger_pwm__B_PWM__DBM_256_CLOCKS || \
            us_trigger_pwm_DeadBandMode == us_trigger_pwm__B_PWM__DBM_2_4_CLOCKS)
            us_trigger_pwm_backup.PWMdeadBandValue = us_trigger_pwm_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(us_trigger_pwm_KillModeMinTime)
             us_trigger_pwm_backup.PWMKillCounterPeriod = us_trigger_pwm_ReadKillTime();
        #endif /* (us_trigger_pwm_KillModeMinTime) */

        #if(us_trigger_pwm_UseControl)
            us_trigger_pwm_backup.PWMControlRegister = us_trigger_pwm_ReadControlRegister();
        #endif /* (us_trigger_pwm_UseControl) */
    #endif  /* (!us_trigger_pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: us_trigger_pwm_RestoreConfig
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
*  us_trigger_pwm_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void us_trigger_pwm_RestoreConfig(void) 
{
        #if(!us_trigger_pwm_UsingFixedFunction)
            #if(!us_trigger_pwm_PWMModeIsCenterAligned)
                us_trigger_pwm_WritePeriod(us_trigger_pwm_backup.PWMPeriod);
            #endif /* (!us_trigger_pwm_PWMModeIsCenterAligned) */

            us_trigger_pwm_WriteCounter(us_trigger_pwm_backup.PWMUdb);

            #if (us_trigger_pwm_UseStatus)
                us_trigger_pwm_STATUS_MASK = us_trigger_pwm_backup.InterruptMaskValue;
            #endif /* (us_trigger_pwm_UseStatus) */

            #if(us_trigger_pwm_DeadBandMode == us_trigger_pwm__B_PWM__DBM_256_CLOCKS || \
                us_trigger_pwm_DeadBandMode == us_trigger_pwm__B_PWM__DBM_2_4_CLOCKS)
                us_trigger_pwm_WriteDeadTime(us_trigger_pwm_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(us_trigger_pwm_KillModeMinTime)
                us_trigger_pwm_WriteKillTime(us_trigger_pwm_backup.PWMKillCounterPeriod);
            #endif /* (us_trigger_pwm_KillModeMinTime) */

            #if(us_trigger_pwm_UseControl)
                us_trigger_pwm_WriteControlRegister(us_trigger_pwm_backup.PWMControlRegister);
            #endif /* (us_trigger_pwm_UseControl) */
        #endif  /* (!us_trigger_pwm_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: us_trigger_pwm_Sleep
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
*  us_trigger_pwm_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void us_trigger_pwm_Sleep(void) 
{
    #if(us_trigger_pwm_UseControl)
        if(us_trigger_pwm_CTRL_ENABLE == (us_trigger_pwm_CONTROL & us_trigger_pwm_CTRL_ENABLE))
        {
            /*Component is enabled */
            us_trigger_pwm_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            us_trigger_pwm_backup.PWMEnableState = 0u;
        }
    #endif /* (us_trigger_pwm_UseControl) */

    /* Stop component */
    us_trigger_pwm_Stop();

    /* Save registers configuration */
    us_trigger_pwm_SaveConfig();
}


/*******************************************************************************
* Function Name: us_trigger_pwm_Wakeup
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
*  us_trigger_pwm_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void us_trigger_pwm_Wakeup(void) 
{
     /* Restore registers values */
    us_trigger_pwm_RestoreConfig();

    if(us_trigger_pwm_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        us_trigger_pwm_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
