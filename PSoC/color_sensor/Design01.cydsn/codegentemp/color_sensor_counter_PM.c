/*******************************************************************************
* File Name: color_sensor_counter_PM.c  
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

#include "color_sensor_counter.h"

static color_sensor_counter_backupStruct color_sensor_counter_backup;


/*******************************************************************************
* Function Name: color_sensor_counter_SaveConfig
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
*  color_sensor_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void color_sensor_counter_SaveConfig(void) 
{
    #if (!color_sensor_counter_UsingFixedFunction)

        color_sensor_counter_backup.CounterUdb = color_sensor_counter_ReadCounter();

        #if(!color_sensor_counter_ControlRegRemoved)
            color_sensor_counter_backup.CounterControlRegister = color_sensor_counter_ReadControlRegister();
        #endif /* (!color_sensor_counter_ControlRegRemoved) */

    #endif /* (!color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_RestoreConfig
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
*  color_sensor_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void color_sensor_counter_RestoreConfig(void) 
{      
    #if (!color_sensor_counter_UsingFixedFunction)

       color_sensor_counter_WriteCounter(color_sensor_counter_backup.CounterUdb);

        #if(!color_sensor_counter_ControlRegRemoved)
            color_sensor_counter_WriteControlRegister(color_sensor_counter_backup.CounterControlRegister);
        #endif /* (!color_sensor_counter_ControlRegRemoved) */

    #endif /* (!color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_Sleep
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
*  color_sensor_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void color_sensor_counter_Sleep(void) 
{
    #if(!color_sensor_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(color_sensor_counter_CTRL_ENABLE == (color_sensor_counter_CONTROL & color_sensor_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            color_sensor_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            color_sensor_counter_backup.CounterEnableState = 0u;
        }
    #else
        color_sensor_counter_backup.CounterEnableState = 1u;
        if(color_sensor_counter_backup.CounterEnableState != 0u)
        {
            color_sensor_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!color_sensor_counter_ControlRegRemoved) */
    
    color_sensor_counter_Stop();
    color_sensor_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: color_sensor_counter_Wakeup
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
*  color_sensor_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void color_sensor_counter_Wakeup(void) 
{
    color_sensor_counter_RestoreConfig();
    #if(!color_sensor_counter_ControlRegRemoved)
        if(color_sensor_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            color_sensor_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!color_sensor_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
