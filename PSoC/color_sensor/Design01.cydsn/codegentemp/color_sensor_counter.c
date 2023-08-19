/*******************************************************************************
* File Name: color_sensor_counter.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 color_sensor_counter_initVar = 0u;


/*******************************************************************************
* Function Name: color_sensor_counter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void color_sensor_counter_Init(void) 
{
        #if (!color_sensor_counter_UsingFixedFunction && !color_sensor_counter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!color_sensor_counter_UsingFixedFunction && !color_sensor_counter_ControlRegRemoved) */
        
        #if(!color_sensor_counter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 color_sensor_counter_interruptState;
        #endif /* (!color_sensor_counter_UsingFixedFunction) */
        
        #if (color_sensor_counter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            color_sensor_counter_CONTROL &= color_sensor_counter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                color_sensor_counter_CONTROL2 &= ((uint8)(~color_sensor_counter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                color_sensor_counter_CONTROL3 &= ((uint8)(~color_sensor_counter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (color_sensor_counter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                color_sensor_counter_CONTROL |= color_sensor_counter_ONESHOT;
            #endif /* (color_sensor_counter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            color_sensor_counter_CONTROL2 |= color_sensor_counter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            color_sensor_counter_RT1 &= ((uint8)(~color_sensor_counter_RT1_MASK));
            color_sensor_counter_RT1 |= color_sensor_counter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            color_sensor_counter_RT1 &= ((uint8)(~color_sensor_counter_SYNCDSI_MASK));
            color_sensor_counter_RT1 |= color_sensor_counter_SYNCDSI_EN;

        #else
            #if(!color_sensor_counter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = color_sensor_counter_CONTROL & ((uint8)(~color_sensor_counter_CTRL_CMPMODE_MASK));
            color_sensor_counter_CONTROL = ctrl | color_sensor_counter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = color_sensor_counter_CONTROL & ((uint8)(~color_sensor_counter_CTRL_CAPMODE_MASK));
            
            #if( 0 != color_sensor_counter_CAPTURE_MODE_CONF)
                color_sensor_counter_CONTROL = ctrl | color_sensor_counter_DEFAULT_CAPTURE_MODE;
            #else
                color_sensor_counter_CONTROL = ctrl;
            #endif /* 0 != color_sensor_counter_CAPTURE_MODE */ 
            
            #endif /* (!color_sensor_counter_ControlRegRemoved) */
        #endif /* (color_sensor_counter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!color_sensor_counter_UsingFixedFunction)
            color_sensor_counter_ClearFIFO();
        #endif /* (!color_sensor_counter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        color_sensor_counter_WritePeriod(color_sensor_counter_INIT_PERIOD_VALUE);
        #if (!(color_sensor_counter_UsingFixedFunction && (CY_PSOC5A)))
            color_sensor_counter_WriteCounter(color_sensor_counter_INIT_COUNTER_VALUE);
        #endif /* (!(color_sensor_counter_UsingFixedFunction && (CY_PSOC5A))) */
        color_sensor_counter_SetInterruptMode(color_sensor_counter_INIT_INTERRUPTS_MASK);
        
        #if (!color_sensor_counter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)color_sensor_counter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            color_sensor_counter_WriteCompare(color_sensor_counter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            color_sensor_counter_interruptState = CyEnterCriticalSection();
            
            color_sensor_counter_STATUS_AUX_CTRL |= color_sensor_counter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(color_sensor_counter_interruptState);
            
        #endif /* (!color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void color_sensor_counter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (color_sensor_counter_UsingFixedFunction)
        color_sensor_counter_GLOBAL_ENABLE |= color_sensor_counter_BLOCK_EN_MASK;
        color_sensor_counter_GLOBAL_STBY_ENABLE |= color_sensor_counter_BLOCK_STBY_EN_MASK;
    #endif /* (color_sensor_counter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!color_sensor_counter_ControlRegRemoved || color_sensor_counter_UsingFixedFunction)
        color_sensor_counter_CONTROL |= color_sensor_counter_CTRL_ENABLE;                
    #endif /* (!color_sensor_counter_ControlRegRemoved || color_sensor_counter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: color_sensor_counter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  color_sensor_counter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void color_sensor_counter_Start(void) 
{
    if(color_sensor_counter_initVar == 0u)
    {
        color_sensor_counter_Init();
        
        color_sensor_counter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    color_sensor_counter_Enable();        
}


/*******************************************************************************
* Function Name: color_sensor_counter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void color_sensor_counter_Stop(void) 
{
    /* Disable Counter */
    #if(!color_sensor_counter_ControlRegRemoved || color_sensor_counter_UsingFixedFunction)
        color_sensor_counter_CONTROL &= ((uint8)(~color_sensor_counter_CTRL_ENABLE));        
    #endif /* (!color_sensor_counter_ControlRegRemoved || color_sensor_counter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (color_sensor_counter_UsingFixedFunction)
        color_sensor_counter_GLOBAL_ENABLE &= ((uint8)(~color_sensor_counter_BLOCK_EN_MASK));
        color_sensor_counter_GLOBAL_STBY_ENABLE &= ((uint8)(~color_sensor_counter_BLOCK_STBY_EN_MASK));
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void color_sensor_counter_SetInterruptMode(uint8 interruptsMask) 
{
    color_sensor_counter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: color_sensor_counter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   color_sensor_counter_ReadStatusRegister(void) 
{
    return color_sensor_counter_STATUS;
}


#if(!color_sensor_counter_ControlRegRemoved)
/*******************************************************************************
* Function Name: color_sensor_counter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   color_sensor_counter_ReadControlRegister(void) 
{
    return color_sensor_counter_CONTROL;
}


/*******************************************************************************
* Function Name: color_sensor_counter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    color_sensor_counter_WriteControlRegister(uint8 control) 
{
    color_sensor_counter_CONTROL = control;
}

#endif  /* (!color_sensor_counter_ControlRegRemoved) */


#if (!(color_sensor_counter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: color_sensor_counter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void color_sensor_counter_WriteCounter(uint32 counter) \
                                   
{
    #if(color_sensor_counter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (color_sensor_counter_GLOBAL_ENABLE & color_sensor_counter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        color_sensor_counter_GLOBAL_ENABLE |= color_sensor_counter_BLOCK_EN_MASK;
        CY_SET_REG16(color_sensor_counter_COUNTER_LSB_PTR, (uint16)counter);
        color_sensor_counter_GLOBAL_ENABLE &= ((uint8)(~color_sensor_counter_BLOCK_EN_MASK));
    #else
        CY_SET_REG32(color_sensor_counter_COUNTER_LSB_PTR, counter);
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}
#endif /* (!(color_sensor_counter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: color_sensor_counter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) The present value of the counter.
*
*******************************************************************************/
uint32 color_sensor_counter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(color_sensor_counter_UsingFixedFunction)
		(void)CY_GET_REG16(color_sensor_counter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(color_sensor_counter_COUNTER_LSB_PTR_8BIT);
	#endif/* (color_sensor_counter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(color_sensor_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(color_sensor_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(color_sensor_counter_STATICCOUNT_LSB_PTR));
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint32) Present Capture value.
*
*******************************************************************************/
uint32 color_sensor_counter_ReadCapture(void) 
{
    #if(color_sensor_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(color_sensor_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG32(color_sensor_counter_STATICCOUNT_LSB_PTR));
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint32) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void color_sensor_counter_WritePeriod(uint32 period) 
{
    #if(color_sensor_counter_UsingFixedFunction)
        CY_SET_REG16(color_sensor_counter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG32(color_sensor_counter_PERIOD_LSB_PTR, period);
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) Present period value.
*
*******************************************************************************/
uint32 color_sensor_counter_ReadPeriod(void) 
{
    #if(color_sensor_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(color_sensor_counter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG32(color_sensor_counter_PERIOD_LSB_PTR));
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


#if (!color_sensor_counter_UsingFixedFunction)
/*******************************************************************************
* Function Name: color_sensor_counter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void color_sensor_counter_WriteCompare(uint32 compare) \
                                   
{
    #if(color_sensor_counter_UsingFixedFunction)
        CY_SET_REG16(color_sensor_counter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG32(color_sensor_counter_COMPARE_LSB_PTR, compare);
    #endif /* (color_sensor_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: color_sensor_counter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint32) Present compare value.
*
*******************************************************************************/
uint32 color_sensor_counter_ReadCompare(void) 
{
    return (CY_GET_REG32(color_sensor_counter_COMPARE_LSB_PTR));
}


#if (color_sensor_counter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: color_sensor_counter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void color_sensor_counter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    color_sensor_counter_CONTROL &= ((uint8)(~color_sensor_counter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    color_sensor_counter_CONTROL |= compareMode;
}
#endif  /* (color_sensor_counter_COMPARE_MODE_SOFTWARE) */


#if (color_sensor_counter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: color_sensor_counter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void color_sensor_counter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    color_sensor_counter_CONTROL &= ((uint8)(~color_sensor_counter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    color_sensor_counter_CONTROL |= ((uint8)((uint8)captureMode << color_sensor_counter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (color_sensor_counter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: color_sensor_counter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void color_sensor_counter_ClearFIFO(void) 
{

    while(0u != (color_sensor_counter_ReadStatusRegister() & color_sensor_counter_STATUS_FIFONEMP))
    {
        (void)color_sensor_counter_ReadCapture();
    }

}
#endif  /* (!color_sensor_counter_UsingFixedFunction) */


/* [] END OF FILE */

