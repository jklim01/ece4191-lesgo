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
    UART_1_Start();
    setup_ultrasonic();

    for(;;)
    {
        /* Place your application code here. */
        // if (echo_Read() == 0) {
        //     trigger_Write(1);
        //     CyDelayUs(10);
        //     trigger_Write(0);
        // }

        CyDelay(100);

        uint16_t us_1_dist = get_us_1_dist();
        uint16_t us_2_dist = get_us_2_dist();
        led_Write(us_1_dist < 5 && us_2_dist < 5);
    }
}

/* [] END OF FILE */
