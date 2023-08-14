/*******************************************************************************
* File Name: motor_l_quaddec.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
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

#if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
    #include "motor_l_quaddec_PVT.h"
#endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */

uint8 motor_l_quaddec_initVar = 0u;


/*******************************************************************************
* Function Name: motor_l_quaddec_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_l_quaddec_Init(void) 
{
    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(motor_l_quaddec_ISR_NUMBER);
        /* Set the ISR to point to the motor_l_quaddec_isr Interrupt. */
        (void) CyIntSetVector(motor_l_quaddec_ISR_NUMBER, & motor_l_quaddec_ISR);
        /* Set the priority. */
        CyIntSetPriority(motor_l_quaddec_ISR_NUMBER, motor_l_quaddec_ISR_PRIORITY);
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_l_quaddec_Enable(void) 
{
    uint8 enableInterrupts;

    motor_l_quaddec_SetInterruptMask(motor_l_quaddec_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) motor_l_quaddec_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    motor_l_quaddec_SR_AUX_CONTROL |= motor_l_quaddec_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(motor_l_quaddec_ISR_NUMBER);
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  motor_l_quaddec_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void motor_l_quaddec_Start(void) 
{
    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT)
        motor_l_quaddec_Cnt8_Start();
        motor_l_quaddec_Cnt8_WriteCounter(motor_l_quaddec_COUNTER_INIT_VALUE);
    #else
        /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT) || 
        *  (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT) 
        */
        motor_l_quaddec_Cnt16_Start();
        motor_l_quaddec_Cnt16_WriteCounter(motor_l_quaddec_COUNTER_INIT_VALUE);
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT */
    
    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)        
       motor_l_quaddec_count32SoftPart = 0;
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */

    if (motor_l_quaddec_initVar == 0u)
    {
        motor_l_quaddec_Init();
        motor_l_quaddec_initVar = 1u;
    }

    motor_l_quaddec_Enable();
}


/*******************************************************************************
* Function Name: motor_l_quaddec_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_l_quaddec_Stop(void) 
{
    uint8 enableInterrupts;

    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT)
        motor_l_quaddec_Cnt8_Stop();
    #else 
        /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT) ||
        *  (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        */
        motor_l_quaddec_Cnt16_Stop();    /* counter disable */
    #endif /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    motor_l_quaddec_SR_AUX_CONTROL &= (uint8) (~motor_l_quaddec_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        CyIntDisable(motor_l_quaddec_ISR_NUMBER);    /* interrupt disable */
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  motor_l_quaddec_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 motor_l_quaddec_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(motor_l_quaddec_ISR_NUMBER);

        tmpCnt = motor_l_quaddec_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) motor_l_quaddec_COUNTER_INIT_VALUE);
        count = motor_l_quaddec_count32SoftPart + hwCount;

        CyIntEnable(motor_l_quaddec_ISR_NUMBER);
    #else 
        /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT) || 
        *  (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT)
        */
        #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT)
            tmpCnt = motor_l_quaddec_Cnt8_ReadCounter();
        #else /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT) */
            tmpCnt = motor_l_quaddec_Cnt16_ReadCounter();
        #endif  /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) motor_l_quaddec_COUNTER_INIT_VALUE);

    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: motor_l_quaddec_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  motor_l_quaddec_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void motor_l_quaddec_SetCounter(int32 value) 
{
    #if ((motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT) || \
         (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + motor_l_quaddec_COUNTER_INIT_VALUE;
        }
        else
        {
            count = motor_l_quaddec_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT)
            motor_l_quaddec_Cnt8_WriteCounter(count);
        #else /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT) */
            motor_l_quaddec_Cnt16_WriteCounter(count);
        #endif  /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT */
    #else /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT) */
        CyIntDisable(motor_l_quaddec_ISR_NUMBER);

        motor_l_quaddec_Cnt16_WriteCounter(motor_l_quaddec_COUNTER_INIT_VALUE);
        motor_l_quaddec_count32SoftPart = value;

        CyIntEnable(motor_l_quaddec_ISR_NUMBER);
    #endif  /* (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_8_BIT) ||
             * (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: motor_l_quaddec_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 motor_l_quaddec_GetEvents(void) 
{
    return (motor_l_quaddec_STATUS_REG & motor_l_quaddec_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: motor_l_quaddec_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void motor_l_quaddec_SetInterruptMask(uint8 mask) 
{
    #if (motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (motor_l_quaddec_COUNTER_OVERFLOW | motor_l_quaddec_COUNTER_UNDERFLOW |
                 motor_l_quaddec_COUNTER_RESET);
    #endif /* motor_l_quaddec_COUNTER_SIZE == motor_l_quaddec_COUNTER_SIZE_32_BIT */

    motor_l_quaddec_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: motor_l_quaddec_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 motor_l_quaddec_GetInterruptMask(void) 
{
    return (motor_l_quaddec_STATUS_MASK & motor_l_quaddec_INIT_INT_MASK);
}


/* [] END OF FILE */
