/*******************************************************************************
* File Name: color_sensor_creg.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_color_sensor_creg_H) /* CY_CONTROL_REG_color_sensor_creg_H */
#define CY_CONTROL_REG_color_sensor_creg_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} color_sensor_creg_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    color_sensor_creg_Write(uint8 control) ;
uint8   color_sensor_creg_Read(void) ;

void color_sensor_creg_SaveConfig(void) ;
void color_sensor_creg_RestoreConfig(void) ;
void color_sensor_creg_Sleep(void) ; 
void color_sensor_creg_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define color_sensor_creg_Control        (* (reg8 *) color_sensor_creg_Sync_ctrl_reg__CONTROL_REG )
#define color_sensor_creg_Control_PTR    (  (reg8 *) color_sensor_creg_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_color_sensor_creg_H */


/* [] END OF FILE */
