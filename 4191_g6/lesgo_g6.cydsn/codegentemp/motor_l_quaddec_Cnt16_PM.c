/*******************************************************************************
* File Name: motor_l_quaddec_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "motor_l_quaddec_Cnt16.h"

static motor_l_quaddec_Cnt16_backupStruct motor_l_quaddec_Cnt16_backup;


/*******************************************************************************
* Function Name: motor_l_quaddec_Cnt16_SaveConfig
********************************************************************************
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
*  motor_l_quaddec_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void motor_l_quaddec_Cnt16_SaveConfig(void) 
{
    #if (!motor_l_quaddec_Cnt16_UsingFixedFunction)

        motor_l_quaddec_Cnt16_backup.CounterUdb = motor_l_quaddec_Cnt16_ReadCounter();

        #if(!motor_l_quaddec_Cnt16_ControlRegRemoved)
            motor_l_quaddec_Cnt16_backup.CounterControlRegister = motor_l_quaddec_Cnt16_ReadControlRegister();
        #endif /* (!motor_l_quaddec_Cnt16_ControlRegRemoved) */

    #endif /* (!motor_l_quaddec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Cnt16_RestoreConfig
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
*  motor_l_quaddec_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void motor_l_quaddec_Cnt16_RestoreConfig(void) 
{      
    #if (!motor_l_quaddec_Cnt16_UsingFixedFunction)

       motor_l_quaddec_Cnt16_WriteCounter(motor_l_quaddec_Cnt16_backup.CounterUdb);

        #if(!motor_l_quaddec_Cnt16_ControlRegRemoved)
            motor_l_quaddec_Cnt16_WriteControlRegister(motor_l_quaddec_Cnt16_backup.CounterControlRegister);
        #endif /* (!motor_l_quaddec_Cnt16_ControlRegRemoved) */

    #endif /* (!motor_l_quaddec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Cnt16_Sleep
********************************************************************************
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
*  motor_l_quaddec_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void motor_l_quaddec_Cnt16_Sleep(void) 
{
    #if(!motor_l_quaddec_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(motor_l_quaddec_Cnt16_CTRL_ENABLE == (motor_l_quaddec_Cnt16_CONTROL & motor_l_quaddec_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            motor_l_quaddec_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            motor_l_quaddec_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        motor_l_quaddec_Cnt16_backup.CounterEnableState = 1u;
        if(motor_l_quaddec_Cnt16_backup.CounterEnableState != 0u)
        {
            motor_l_quaddec_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!motor_l_quaddec_Cnt16_ControlRegRemoved) */
    
    motor_l_quaddec_Cnt16_Stop();
    motor_l_quaddec_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Cnt16_Wakeup
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
*  motor_l_quaddec_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void motor_l_quaddec_Cnt16_Wakeup(void) 
{
    motor_l_quaddec_Cnt16_RestoreConfig();
    #if(!motor_l_quaddec_Cnt16_ControlRegRemoved)
        if(motor_l_quaddec_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            motor_l_quaddec_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!motor_l_quaddec_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
