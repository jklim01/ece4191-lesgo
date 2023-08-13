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
#include <stdatomic.h>

#include "ultrasonic.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_1_Start();
    isr_1_StartEx(timer_isr);
    for(;;)
    {
        // /* Place your application code here. */
        if (echo_Read() == 0) {
            trigger_Write(1);
            CyDelayUs(10);
            trigger_Write(0);
        }

        CyDelay(100);

        if (atomic_load(&us_dist) < 5) {
            led_Write(1);
            continue;
        }
        led_Write(0);
    }
}

/* [] END OF FILE */
