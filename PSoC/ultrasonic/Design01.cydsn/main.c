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
// #include "ultrasonic.h"
#include <stdio.h>

CY_ISR(isr_3_handler) {
    Timer_3_ReadStatusRegister();
    trigger_2_Write(1);
    CyDelayUs(1);
    trigger_2_Write(0);
}

static volatile float dist = 0;
static volatile uint8 echo_select = 0;
CY_ISR(isr_4_handler) {
    static char str[30];
    Timer_4_ReadStatusRegister();
    dist = (UINT16_MAX - Timer_4_ReadCounter()) / 58.0;
    sprintf(str, "sel=%u, dist=%f\n", echo_select, dist);
    UART_1_PutString(str);

    if (echo_select == 0)
        echo_select = 1;
    else
        echo_select = 0;
    echo_mux_Write(echo_select);
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    // setup_ultrasonic();
    Timer_3_Start();
    Timer_4_Start();
    isr_3_StartEx(isr_3_handler);
    isr_4_StartEx(isr_4_handler);

    for(;;)
    {
        /* Place your application code here. */
        // CyDelay(100);
        // uint16_t us_1_dist = get_us_1_dist();
        // uint16_t us_2_dist = get_us_2_dist();
        // led_Write(us_1_dist < 5 && us_2_dist < 5);

    }
}

/* [] END OF FILE */
