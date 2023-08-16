/*******************************************************************************
* File Name: color_sensor_pwm_PM.c
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

#include "color_sensor_pwm.h"

static color_sensor_pwm_backupStruct color_sensor_pwm_backup;


/*******************************************************************************
* Function Name: color_sensor_pwm_SaveConfig
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
*  color_sensor_pwm_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void color_sensor_pwm_SaveConfig(void) 
{

    #if(!color_sensor_pwm_UsingFixedFunction)
        #if(!color_sensor_pwm_PWMModeIsCenterAligned)
            color_sensor_pwm_backup.PWMPeriod = color_sensor_pwm_ReadPeriod();
        #endif /* (!color_sensor_pwm_PWMModeIsCenterAligned) */
        color_sensor_pwm_backup.PWMUdb = color_sensor_pwm_ReadCounter();
        #if (color_sensor_pwm_UseStatus)
            color_sensor_pwm_backup.InterruptMaskValue = color_sensor_pwm_STATUS_MASK;
        #endif /* (color_sensor_pwm_UseStatus) */

        #if(color_sensor_pwm_DeadBandMode == color_sensor_pwm__B_PWM__DBM_256_CLOCKS || \
            color_sensor_pwm_DeadBandMode == color_sensor_pwm__B_PWM__DBM_2_4_CLOCKS)
            color_sensor_pwm_backup.PWMdeadBandValue = color_sensor_pwm_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(color_sensor_pwm_KillModeMinTime)
             color_sensor_pwm_backup.PWMKillCounterPeriod = color_sensor_pwm_ReadKillTime();
        #endif /* (color_sensor_pwm_KillModeMinTime) */

        #if(color_sensor_pwm_UseControl)
            color_sensor_pwm_backup.PWMControlRegister = color_sensor_pwm_ReadControlRegister();
        #endif /* (color_sensor_pwm_UseControl) */
    #endif  /* (!color_sensor_pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_pwm_RestoreConfig
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
*  color_sensor_pwm_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void color_sensor_pwm_RestoreConfig(void) 
{
        #if(!color_sensor_pwm_UsingFixedFunction)
            #if(!color_sensor_pwm_PWMModeIsCenterAligned)
                color_sensor_pwm_WritePeriod(color_sensor_pwm_backup.PWMPeriod);
            #endif /* (!color_sensor_pwm_PWMModeIsCenterAligned) */

            color_sensor_pwm_WriteCounter(color_sensor_pwm_backup.PWMUdb);

            #if (color_sensor_pwm_UseStatus)
                color_sensor_pwm_STATUS_MASK = color_sensor_pwm_backup.InterruptMaskValue;
            #endif /* (color_sensor_pwm_UseStatus) */

            #if(color_sensor_pwm_DeadBandMode == color_sensor_pwm__B_PWM__DBM_256_CLOCKS || \
                color_sensor_pwm_DeadBandMode == color_sensor_pwm__B_PWM__DBM_2_4_CLOCKS)
                color_sensor_pwm_WriteDeadTime(color_sensor_pwm_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(color_sensor_pwm_KillModeMinTime)
                color_sensor_pwm_WriteKillTime(color_sensor_pwm_backup.PWMKillCounterPeriod);
            #endif /* (color_sensor_pwm_KillModeMinTime) */

            #if(color_sensor_pwm_UseControl)
                color_sensor_pwm_WriteControlRegister(color_sensor_pwm_backup.PWMControlRegister);
            #endif /* (color_sensor_pwm_UseControl) */
        #endif  /* (!color_sensor_pwm_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: color_sensor_pwm_Sleep
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
*  color_sensor_pwm_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void color_sensor_pwm_Sleep(void) 
{
    #if(color_sensor_pwm_UseControl)
        if(color_sensor_pwm_CTRL_ENABLE == (color_sensor_pwm_CONTROL & color_sensor_pwm_CTRL_ENABLE))
        {
            /*Component is enabled */
            color_sensor_pwm_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            color_sensor_pwm_backup.PWMEnableState = 0u;
        }
    #endif /* (color_sensor_pwm_UseControl) */

    /* Stop component */
    color_sensor_pwm_Stop();

    /* Save registers configuration */
    color_sensor_pwm_SaveConfig();
}


/*******************************************************************************
* Function Name: color_sensor_pwm_Wakeup
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
*  color_sensor_pwm_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void color_sensor_pwm_Wakeup(void) 
{
     /* Restore registers values */
    color_sensor_pwm_RestoreConfig();

    if(color_sensor_pwm_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        color_sensor_pwm_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
