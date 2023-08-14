/*******************************************************************************
* File Name: motor_l_quaddec_INT.c
* Version 3.0
*
* Description:
*  This file contains the Interrupt Service Routine (ISR) for the Quadrature
*  Decoder component.
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

#include "motor_l_quaddec.h"
#include "motor_l_quaddec_PVT.h"
#include "cyapicallbacks.h"

volatile int32 motor_l_quaddec_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void motor_l_quaddec_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for Quadrature Decoder Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  motor_l_quaddec_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  motor_l_quaddec_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( motor_l_quaddec_ISR )
{
   uint8 motor_l_quaddec_swStatus;

   motor_l_quaddec_swStatus = motor_l_quaddec_STATUS_REG;

    #ifdef motor_l_quaddec_ISR_ENTRY_CALLBACK
        motor_l_quaddec_ISR_EntryCallback();
    #endif /* motor_l_quaddec_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START motor_l_quaddec_ISR_START` */

    /* `#END` */

    if (0u != (motor_l_quaddec_swStatus & motor_l_quaddec_COUNTER_OVERFLOW))
    {
        motor_l_quaddec_count32SoftPart += (int32) motor_l_quaddec_COUNTER_MAX_VALUE;
    }
    else if (0u != (motor_l_quaddec_swStatus & motor_l_quaddec_COUNTER_UNDERFLOW))
    {
        motor_l_quaddec_count32SoftPart -= (int32) motor_l_quaddec_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (motor_l_quaddec_swStatus & motor_l_quaddec_COUNTER_RESET))
    {
        motor_l_quaddec_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START motor_l_quaddec_ISR_END` */

    /* `#END` */

    #ifdef motor_l_quaddec_ISR_EXIT_CALLBACK
        motor_l_quaddec_ISR_ExitCallback();
    #endif /* motor_l_quaddec_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */
