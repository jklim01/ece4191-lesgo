/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // MOVE FORWARD     = IN1 - 0, IN2 - 1
    // MOVE BACKWARD    = IN1 - 1, IN2 - 0
    
    // Left Wheel
    PWM_1_Start();
    Motor_1_IN_1_Write(1);
    Motor_1_IN_2_Write(0);
    PWM_1_WriteCompare(175);    // Left Wheel Speed
    // Right Wheel
    PWM_2_Start();
    Motor_1_IN_3_Write(0);
    Motor_1_IN_4_Write(1);
    PWM_2_WriteCompare(175);    // Right Wheel Speed
    
    // Shaft Encoder - 35:45
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
