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


static volatile bool compare_ready = false;
CY_ISR(color_sensor_isr) {
    PWM_1_ReadStatusRegister();
    compare_ready = true;
}

CY_ISR(counter_tc_isr) {
    UART_1_PutString("\nOVERFLOW!\n");
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_1_Start();
    Counter_1_Start();
    UART_1_Start();
    isr_1_StartEx(color_sensor_isr);
    isr_2_StartEx(counter_tc_isr);

    color_sensor_s0_Write(1);
    color_sensor_s1_Write(1);
    color_sensor_led_Write(1);
    CyDelay(20);                    // ensure signals have been written


    int mode = 1;
    for(;;)
    {
        /* Place your application code here. */
        switch (mode) {
            case 1: {
                compare_ready = false;
                color_sensor_s2_Write(0);
                color_sensor_s3_Write(0);
                CyDelay(20);

                // 1ms high pulse on reset pins
                Control_Reg_1_Write(1);
                CyDelay(1);
                Control_Reg_1_Write(0);

                while (!compare_ready);
                uint32 count = Counter_1_ReadCapture();

                char str[20];
                sprintf(str, "red: %8lu\t", count);
                UART_1_PutString(str);
                mode = 2;

                break;
            }
            case 2: {
                compare_ready = false;
                color_sensor_s2_Write(1);
                color_sensor_s3_Write(1);
                CyDelay(20);

                // 1ms high pulse on reset pins
                Control_Reg_1_Write(1);
                CyDelay(1);
                Control_Reg_1_Write(0);

                while (!compare_ready);
                uint32 count = Counter_1_ReadCapture();

                char str[20];
                sprintf(str, "green: %8lu\t", count);
                UART_1_PutString(str);
                mode = 3;

                break;
            }
            case 3: {
                compare_ready = false;
                color_sensor_s2_Write(0);
                color_sensor_s3_Write(1);
                CyDelay(20);

                // 1ms high pulse on reset pins
                Control_Reg_1_Write(1);
                CyDelay(1);
                Control_Reg_1_Write(0);

                while (!compare_ready);
                uint32 count = Counter_1_ReadCapture();

                char str[20];
                sprintf(str, "blue: %8lu\n", count);
                UART_1_PutString(str);
                mode = 1;

                break;
            }
        }
    }
}

/* [] END OF FILE */
