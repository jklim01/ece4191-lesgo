/*******************************************************************************
* File Name: ir_r_sreg.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ir_r_sreg.h"

#if !defined(ir_r_sreg_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: ir_r_sreg_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 ir_r_sreg_Read(void) 
{ 
    return ir_r_sreg_Status;
}


/*******************************************************************************
* Function Name: ir_r_sreg_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ir_r_sreg_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    ir_r_sreg_Status_Aux_Ctrl |= ir_r_sreg_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ir_r_sreg_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ir_r_sreg_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    ir_r_sreg_Status_Aux_Ctrl &= (uint8)(~ir_r_sreg_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ir_r_sreg_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void ir_r_sreg_WriteMask(uint8 mask) 
{
    #if(ir_r_sreg_INPUTS < 8u)
    	mask &= ((uint8)(1u << ir_r_sreg_INPUTS) - 1u);
	#endif /* End ir_r_sreg_INPUTS < 8u */
    ir_r_sreg_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: ir_r_sreg_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 ir_r_sreg_ReadMask(void) 
{
    return ir_r_sreg_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
