/*******************************************************************************
* File Name: motor_r_quaddec_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "motor_r_quaddec.h"

static motor_r_quaddec_BACKUP_STRUCT motor_r_quaddec_backup = {0u};


/*******************************************************************************
* Function Name: motor_r_quaddec_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_r_quaddec_SaveConfig(void) 
{
    #if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT)
        motor_r_quaddec_Cnt8_SaveConfig();
    #else 
        /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_16_BIT) || 
         * (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT)
         */
        motor_r_quaddec_Cnt16_SaveConfig();
    #endif /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: motor_r_quaddec_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_r_quaddec_RestoreConfig(void) 
{
    #if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT)
        motor_r_quaddec_Cnt8_RestoreConfig();
    #else 
        /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_16_BIT) || 
         * (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT) 
         */
        motor_r_quaddec_Cnt16_RestoreConfig();
    #endif /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: motor_r_quaddec_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  motor_r_quaddec_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void motor_r_quaddec_Sleep(void) 
{
    if (0u != (motor_r_quaddec_SR_AUX_CONTROL & motor_r_quaddec_INTERRUPTS_ENABLE))
    {
        motor_r_quaddec_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        motor_r_quaddec_backup.enableState = 0u;
    }

    motor_r_quaddec_Stop();
    motor_r_quaddec_SaveConfig();
}


/*******************************************************************************
* Function Name: motor_r_quaddec_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  motor_r_quaddec_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void motor_r_quaddec_Wakeup(void) 
{
    motor_r_quaddec_RestoreConfig();

    if (motor_r_quaddec_backup.enableState != 0u)
    {
        #if (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT)
            motor_r_quaddec_Cnt8_Enable();
        #else 
            /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_16_BIT) || 
            *  (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_32_BIT) 
            */
            motor_r_quaddec_Cnt16_Enable();
        #endif /* (motor_r_quaddec_COUNTER_SIZE == motor_r_quaddec_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        motor_r_quaddec_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

