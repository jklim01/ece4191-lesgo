/*******************************************************************************
* File Name: motor_r_pwm_PM.c
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

#include "motor_r_pwm.h"

static motor_r_pwm_backupStruct motor_r_pwm_backup;


/*******************************************************************************
* Function Name: motor_r_pwm_SaveConfig
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
*  motor_r_pwm_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void motor_r_pwm_SaveConfig(void) 
{

    #if(!motor_r_pwm_UsingFixedFunction)
        #if(!motor_r_pwm_PWMModeIsCenterAligned)
            motor_r_pwm_backup.PWMPeriod = motor_r_pwm_ReadPeriod();
        #endif /* (!motor_r_pwm_PWMModeIsCenterAligned) */
        motor_r_pwm_backup.PWMUdb = motor_r_pwm_ReadCounter();
        #if (motor_r_pwm_UseStatus)
            motor_r_pwm_backup.InterruptMaskValue = motor_r_pwm_STATUS_MASK;
        #endif /* (motor_r_pwm_UseStatus) */

        #if(motor_r_pwm_DeadBandMode == motor_r_pwm__B_PWM__DBM_256_CLOCKS || \
            motor_r_pwm_DeadBandMode == motor_r_pwm__B_PWM__DBM_2_4_CLOCKS)
            motor_r_pwm_backup.PWMdeadBandValue = motor_r_pwm_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(motor_r_pwm_KillModeMinTime)
             motor_r_pwm_backup.PWMKillCounterPeriod = motor_r_pwm_ReadKillTime();
        #endif /* (motor_r_pwm_KillModeMinTime) */

        #if(motor_r_pwm_UseControl)
            motor_r_pwm_backup.PWMControlRegister = motor_r_pwm_ReadControlRegister();
        #endif /* (motor_r_pwm_UseControl) */
    #endif  /* (!motor_r_pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor_r_pwm_RestoreConfig
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
*  motor_r_pwm_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motor_r_pwm_RestoreConfig(void) 
{
        #if(!motor_r_pwm_UsingFixedFunction)
            #if(!motor_r_pwm_PWMModeIsCenterAligned)
                motor_r_pwm_WritePeriod(motor_r_pwm_backup.PWMPeriod);
            #endif /* (!motor_r_pwm_PWMModeIsCenterAligned) */

            motor_r_pwm_WriteCounter(motor_r_pwm_backup.PWMUdb);

            #if (motor_r_pwm_UseStatus)
                motor_r_pwm_STATUS_MASK = motor_r_pwm_backup.InterruptMaskValue;
            #endif /* (motor_r_pwm_UseStatus) */

            #if(motor_r_pwm_DeadBandMode == motor_r_pwm__B_PWM__DBM_256_CLOCKS || \
                motor_r_pwm_DeadBandMode == motor_r_pwm__B_PWM__DBM_2_4_CLOCKS)
                motor_r_pwm_WriteDeadTime(motor_r_pwm_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(motor_r_pwm_KillModeMinTime)
                motor_r_pwm_WriteKillTime(motor_r_pwm_backup.PWMKillCounterPeriod);
            #endif /* (motor_r_pwm_KillModeMinTime) */

            #if(motor_r_pwm_UseControl)
                motor_r_pwm_WriteControlRegister(motor_r_pwm_backup.PWMControlRegister);
            #endif /* (motor_r_pwm_UseControl) */
        #endif  /* (!motor_r_pwm_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: motor_r_pwm_Sleep
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
*  motor_r_pwm_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motor_r_pwm_Sleep(void) 
{
    #if(motor_r_pwm_UseControl)
        if(motor_r_pwm_CTRL_ENABLE == (motor_r_pwm_CONTROL & motor_r_pwm_CTRL_ENABLE))
        {
            /*Component is enabled */
            motor_r_pwm_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            motor_r_pwm_backup.PWMEnableState = 0u;
        }
    #endif /* (motor_r_pwm_UseControl) */

    /* Stop component */
    motor_r_pwm_Stop();

    /* Save registers configuration */
    motor_r_pwm_SaveConfig();
}


/*******************************************************************************
* Function Name: motor_r_pwm_Wakeup
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
*  motor_r_pwm_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motor_r_pwm_Wakeup(void) 
{
     /* Restore registers values */
    motor_r_pwm_RestoreConfig();

    if(motor_r_pwm_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        motor_r_pwm_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
