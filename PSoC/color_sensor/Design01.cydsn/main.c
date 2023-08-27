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
#include <stdbool.h>
#include <stdio.h>

#include "color_sensor.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    setup_color_sensor();
    UART_1_Start();

    for(;;)
    {
        /* Place your application code here. */
        Color c = color_sense();
        switch (c) {
            case RED: {
                UART_1_PutString("(Red!) XD\n");
                break;
            }
            case GREEN: {
                UART_1_PutString("(Green!) :)\n");
                break;
            }
            case BLUE: {
                UART_1_PutString("(Blue!) :(\n");
                break;
            }
        }
    }
}

/* [] END OF FILE */
