/*******************************************************************************
* File Name: motor_r_pwm.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "motor_r_pwm.h"

/* Error message for removed <resource> through optimization */
#ifdef motor_r_pwm_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* motor_r_pwm_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 motor_r_pwm_initVar = 0u;


/*******************************************************************************
* Function Name: motor_r_pwm_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  motor_r_pwm_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void motor_r_pwm_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(motor_r_pwm_initVar == 0u)
    {
        motor_r_pwm_Init();
        motor_r_pwm_initVar = 1u;
    }
    motor_r_pwm_Enable();

}


/*******************************************************************************
* Function Name: motor_r_pwm_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  motor_r_pwm_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void motor_r_pwm_Init(void) 
{
    #if (motor_r_pwm_UsingFixedFunction || motor_r_pwm_UseControl)
        uint8 ctrl;
    #endif /* (motor_r_pwm_UsingFixedFunction || motor_r_pwm_UseControl) */

    #if(!motor_r_pwm_UsingFixedFunction)
        #if(motor_r_pwm_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 motor_r_pwm_interruptState;
        #endif /* (motor_r_pwm_UseStatus) */
    #endif /* (!motor_r_pwm_UsingFixedFunction) */

    #if (motor_r_pwm_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        motor_r_pwm_CONTROL |= motor_r_pwm_CFG0_MODE;
        #if (motor_r_pwm_DeadBand2_4)
            motor_r_pwm_CONTROL |= motor_r_pwm_CFG0_DB;
        #endif /* (motor_r_pwm_DeadBand2_4) */

        ctrl = motor_r_pwm_CONTROL3 & ((uint8 )(~motor_r_pwm_CTRL_CMPMODE1_MASK));
        motor_r_pwm_CONTROL3 = ctrl | motor_r_pwm_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        motor_r_pwm_RT1 &= ((uint8)(~motor_r_pwm_RT1_MASK));
        motor_r_pwm_RT1 |= motor_r_pwm_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        motor_r_pwm_RT1 &= ((uint8)(~motor_r_pwm_SYNCDSI_MASK));
        motor_r_pwm_RT1 |= motor_r_pwm_SYNCDSI_EN;

    #elif (motor_r_pwm_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = motor_r_pwm_CONTROL & ((uint8)(~motor_r_pwm_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~motor_r_pwm_CTRL_CMPMODE1_MASK));
        motor_r_pwm_CONTROL = ctrl | motor_r_pwm_DEFAULT_COMPARE2_MODE |
                                   motor_r_pwm_DEFAULT_COMPARE1_MODE;
    #endif /* (motor_r_pwm_UsingFixedFunction) */

    #if (!motor_r_pwm_UsingFixedFunction)
        #if (motor_r_pwm_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            motor_r_pwm_AUX_CONTROLDP0 |= (motor_r_pwm_AUX_CTRL_FIFO0_CLR);
        #else /* (motor_r_pwm_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            motor_r_pwm_AUX_CONTROLDP0 |= (motor_r_pwm_AUX_CTRL_FIFO0_CLR);
            motor_r_pwm_AUX_CONTROLDP1 |= (motor_r_pwm_AUX_CTRL_FIFO0_CLR);
        #endif /* (motor_r_pwm_Resolution == 8) */

        motor_r_pwm_WriteCounter(motor_r_pwm_INIT_PERIOD_VALUE);
    #endif /* (!motor_r_pwm_UsingFixedFunction) */

    motor_r_pwm_WritePeriod(motor_r_pwm_INIT_PERIOD_VALUE);

        #if (motor_r_pwm_UseOneCompareMode)
            motor_r_pwm_WriteCompare(motor_r_pwm_INIT_COMPARE_VALUE1);
        #else
            motor_r_pwm_WriteCompare1(motor_r_pwm_INIT_COMPARE_VALUE1);
            motor_r_pwm_WriteCompare2(motor_r_pwm_INIT_COMPARE_VALUE2);
        #endif /* (motor_r_pwm_UseOneCompareMode) */

        #if (motor_r_pwm_KillModeMinTime)
            motor_r_pwm_WriteKillTime(motor_r_pwm_MinimumKillTime);
        #endif /* (motor_r_pwm_KillModeMinTime) */

        #if (motor_r_pwm_DeadBandUsed)
            motor_r_pwm_WriteDeadTime(motor_r_pwm_INIT_DEAD_TIME);
        #endif /* (motor_r_pwm_DeadBandUsed) */

    #if (motor_r_pwm_UseStatus || motor_r_pwm_UsingFixedFunction)
        motor_r_pwm_SetInterruptMode(motor_r_pwm_INIT_INTERRUPTS_MODE);
    #endif /* (motor_r_pwm_UseStatus || motor_r_pwm_UsingFixedFunction) */

    #if (motor_r_pwm_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        motor_r_pwm_GLOBAL_ENABLE |= motor_r_pwm_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        motor_r_pwm_CONTROL2 |= motor_r_pwm_CTRL2_IRQ_SEL;
    #else
        #if(motor_r_pwm_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            motor_r_pwm_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            motor_r_pwm_STATUS_AUX_CTRL |= motor_r_pwm_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(motor_r_pwm_interruptState);

            /* Clear the FIFO to enable the motor_r_pwm_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            motor_r_pwm_ClearFIFO();
        #endif /* (motor_r_pwm_UseStatus) */
    #endif /* (motor_r_pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor_r_pwm_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void motor_r_pwm_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (motor_r_pwm_UsingFixedFunction)
        motor_r_pwm_GLOBAL_ENABLE |= motor_r_pwm_BLOCK_EN_MASK;
        motor_r_pwm_GLOBAL_STBY_ENABLE |= motor_r_pwm_BLOCK_STBY_EN_MASK;
    #endif /* (motor_r_pwm_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (motor_r_pwm_UseControl || motor_r_pwm_UsingFixedFunction)
        motor_r_pwm_CONTROL |= motor_r_pwm_CTRL_ENABLE;
    #endif /* (motor_r_pwm_UseControl || motor_r_pwm_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: motor_r_pwm_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void motor_r_pwm_Stop(void) 
{
    #if (motor_r_pwm_UseControl || motor_r_pwm_UsingFixedFunction)
        motor_r_pwm_CONTROL &= ((uint8)(~motor_r_pwm_CTRL_ENABLE));
    #endif /* (motor_r_pwm_UseControl || motor_r_pwm_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (motor_r_pwm_UsingFixedFunction)
        motor_r_pwm_GLOBAL_ENABLE &= ((uint8)(~motor_r_pwm_BLOCK_EN_MASK));
        motor_r_pwm_GLOBAL_STBY_ENABLE &= ((uint8)(~motor_r_pwm_BLOCK_STBY_EN_MASK));
    #endif /* (motor_r_pwm_UsingFixedFunction) */
}

#if (motor_r_pwm_UseOneCompareMode)
    #if (motor_r_pwm_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motor_r_pwm_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void motor_r_pwm_SetCompareMode(uint8 comparemode) 
        {
            #if(motor_r_pwm_UsingFixedFunction)

                #if(0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor_r_pwm_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT) */

                motor_r_pwm_CONTROL3 &= ((uint8)(~motor_r_pwm_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                motor_r_pwm_CONTROL3 |= comparemodemasked;

            #elif (motor_r_pwm_UseControl)

                #if(0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << motor_r_pwm_CTRL_CMPMODE1_SHIFT)) &
                                                motor_r_pwm_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & motor_r_pwm_CTRL_CMPMODE1_MASK;
                #endif /* (0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT) */

                #if(0 != motor_r_pwm_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << motor_r_pwm_CTRL_CMPMODE2_SHIFT)) &
                                               motor_r_pwm_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & motor_r_pwm_CTRL_CMPMODE2_MASK;
                #endif /* (0 != motor_r_pwm_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                motor_r_pwm_CONTROL &= ((uint8)(~(motor_r_pwm_CTRL_CMPMODE1_MASK |
                                            motor_r_pwm_CTRL_CMPMODE2_MASK)));
                motor_r_pwm_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (motor_r_pwm_UsingFixedFunction) */
        }
    #endif /* motor_r_pwm_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (motor_r_pwm_CompareMode1SW)


        /*******************************************************************************
        * Function Name: motor_r_pwm_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void motor_r_pwm_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor_r_pwm_CTRL_CMPMODE1_SHIFT)) &
                                           motor_r_pwm_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & motor_r_pwm_CTRL_CMPMODE1_MASK;
            #endif /* (0 != motor_r_pwm_CTRL_CMPMODE1_SHIFT) */

            #if (motor_r_pwm_UseControl)
                motor_r_pwm_CONTROL &= ((uint8)(~motor_r_pwm_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                motor_r_pwm_CONTROL |= comparemodemasked;
            #endif /* (motor_r_pwm_UseControl) */
        }
    #endif /* motor_r_pwm_CompareMode1SW */

#if (motor_r_pwm_CompareMode2SW)


    /*******************************************************************************
    * Function Name: motor_r_pwm_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != motor_r_pwm_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << motor_r_pwm_CTRL_CMPMODE2_SHIFT)) &
                                                 motor_r_pwm_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & motor_r_pwm_CTRL_CMPMODE2_MASK;
        #endif /* (0 != motor_r_pwm_CTRL_CMPMODE2_SHIFT) */

        #if (motor_r_pwm_UseControl)
            motor_r_pwm_CONTROL &= ((uint8)(~motor_r_pwm_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            motor_r_pwm_CONTROL |= comparemodemasked;
        #endif /* (motor_r_pwm_UseControl) */
    }
    #endif /*motor_r_pwm_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!motor_r_pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void motor_r_pwm_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(motor_r_pwm_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 motor_r_pwm_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(motor_r_pwm_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(motor_r_pwm_CAPTURE_LSB_PTR));
    }

    #if (motor_r_pwm_UseStatus)


        /*******************************************************************************
        * Function Name: motor_r_pwm_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void motor_r_pwm_ClearFIFO(void) 
        {
            while(0u != (motor_r_pwm_ReadStatusRegister() & motor_r_pwm_STATUS_FIFONEMPTY))
            {
                (void)motor_r_pwm_ReadCapture();
            }
        }

    #endif /* motor_r_pwm_UseStatus */

#endif /* !motor_r_pwm_UsingFixedFunction */


/*******************************************************************************
* Function Name: motor_r_pwm_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void motor_r_pwm_WritePeriod(uint16 period) 
{
    #if(motor_r_pwm_UsingFixedFunction)
        CY_SET_REG16(motor_r_pwm_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(motor_r_pwm_PERIOD_LSB_PTR, period);
    #endif /* (motor_r_pwm_UsingFixedFunction) */
}

#if (motor_r_pwm_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void motor_r_pwm_WriteCompare(uint16 compare) \
                                       
    {
        #if(motor_r_pwm_UsingFixedFunction)
            CY_SET_REG16(motor_r_pwm_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(motor_r_pwm_COMPARE1_LSB_PTR, compare);
        #endif /* (motor_r_pwm_UsingFixedFunction) */

        #if (motor_r_pwm_PWMMode == motor_r_pwm__B_PWM__DITHER)
            #if(motor_r_pwm_UsingFixedFunction)
                CY_SET_REG16(motor_r_pwm_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(motor_r_pwm_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (motor_r_pwm_UsingFixedFunction) */
        #endif /* (motor_r_pwm_PWMMode == motor_r_pwm__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_WriteCompare1(uint16 compare) \
                                        
    {
        #if(motor_r_pwm_UsingFixedFunction)
            CY_SET_REG16(motor_r_pwm_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(motor_r_pwm_COMPARE1_LSB_PTR, compare);
        #endif /* (motor_r_pwm_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_WriteCompare2(uint16 compare) \
                                        
    {
        #if(motor_r_pwm_UsingFixedFunction)
            CY_SET_REG16(motor_r_pwm_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(motor_r_pwm_COMPARE2_LSB_PTR, compare);
        #endif /* (motor_r_pwm_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (motor_r_pwm_DeadBandUsed)


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!motor_r_pwm_DeadBand2_4)
            CY_SET_REG8(motor_r_pwm_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            motor_r_pwm_DEADBAND_COUNT &= ((uint8)(~motor_r_pwm_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(motor_r_pwm_DEADBAND_COUNT_SHIFT)
                motor_r_pwm_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << motor_r_pwm_DEADBAND_COUNT_SHIFT)) &
                                                    motor_r_pwm_DEADBAND_COUNT_MASK;
            #else
                motor_r_pwm_DEADBAND_COUNT |= deadtime & motor_r_pwm_DEADBAND_COUNT_MASK;
            #endif /* (motor_r_pwm_DEADBAND_COUNT_SHIFT) */

        #endif /* (!motor_r_pwm_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 motor_r_pwm_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!motor_r_pwm_DeadBand2_4)
            return (CY_GET_REG8(motor_r_pwm_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(motor_r_pwm_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(motor_r_pwm_DEADBAND_COUNT & motor_r_pwm_DEADBAND_COUNT_MASK)) >>
                                                                           motor_r_pwm_DEADBAND_COUNT_SHIFT));
            #else
                return (motor_r_pwm_DEADBAND_COUNT & motor_r_pwm_DEADBAND_COUNT_MASK);
            #endif /* (motor_r_pwm_DEADBAND_COUNT_SHIFT) */
        #endif /* (!motor_r_pwm_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (motor_r_pwm_UseStatus || motor_r_pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor_r_pwm_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(motor_r_pwm_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 motor_r_pwm_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(motor_r_pwm_STATUS_PTR));
    }

#endif /* (motor_r_pwm_UseStatus || motor_r_pwm_UsingFixedFunction) */


#if (motor_r_pwm_UseControl)


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 motor_r_pwm_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(motor_r_pwm_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(motor_r_pwm_CONTROL_PTR, control);
    }

#endif /* (motor_r_pwm_UseControl) */


#if (!motor_r_pwm_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 motor_r_pwm_ReadCapture(void) 
    {
        return (CY_GET_REG16(motor_r_pwm_CAPTURE_LSB_PTR));
    }

#endif /* (!motor_r_pwm_UsingFixedFunction) */


#if (motor_r_pwm_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 motor_r_pwm_ReadCompare(void) 
    {
        #if(motor_r_pwm_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(motor_r_pwm_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(motor_r_pwm_COMPARE1_LSB_PTR));
        #endif /* (motor_r_pwm_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 motor_r_pwm_ReadCompare1(void) 
    {
        return (CY_GET_REG16(motor_r_pwm_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 motor_r_pwm_ReadCompare2(void) 
    {
        return (CY_GET_REG16(motor_r_pwm_COMPARE2_LSB_PTR));
    }

#endif /* (motor_r_pwm_UseOneCompareMode) */


/*******************************************************************************
* Function Name: motor_r_pwm_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 motor_r_pwm_ReadPeriod(void) 
{
    #if(motor_r_pwm_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(motor_r_pwm_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(motor_r_pwm_PERIOD_LSB_PTR));
    #endif /* (motor_r_pwm_UsingFixedFunction) */
}

#if ( motor_r_pwm_KillModeMinTime)


    /*******************************************************************************
    * Function Name: motor_r_pwm_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void motor_r_pwm_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(motor_r_pwm_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: motor_r_pwm_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 motor_r_pwm_ReadKillTime(void) 
    {
        return (CY_GET_REG8(motor_r_pwm_KILLMODEMINTIME_PTR));
    }

#endif /* ( motor_r_pwm_KillModeMinTime) */

/* [] END OF FILE */
