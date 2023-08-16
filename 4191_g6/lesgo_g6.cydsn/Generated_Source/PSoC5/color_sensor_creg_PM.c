/*******************************************************************************
* File Name: color_sensor_creg_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "color_sensor_creg.h"

/* Check for removal by optimization */
#if !defined(color_sensor_creg_Sync_ctrl_reg__REMOVED)

static color_sensor_creg_BACKUP_STRUCT  color_sensor_creg_backup = {0u};

    
/*******************************************************************************
* Function Name: color_sensor_creg_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void color_sensor_creg_SaveConfig(void) 
{
    color_sensor_creg_backup.controlState = color_sensor_creg_Control;
}


/*******************************************************************************
* Function Name: color_sensor_creg_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void color_sensor_creg_RestoreConfig(void) 
{
     color_sensor_creg_Control = color_sensor_creg_backup.controlState;
}


/*******************************************************************************
* Function Name: color_sensor_creg_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void color_sensor_creg_Sleep(void) 
{
    color_sensor_creg_SaveConfig();
}


/*******************************************************************************
* Function Name: color_sensor_creg_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void color_sensor_creg_Wakeup(void)  
{
    color_sensor_creg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
