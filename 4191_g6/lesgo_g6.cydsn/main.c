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
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "color_sensor.h"
#include "servo.h"
#include "locomotion.h"
#include "ir_sensor.h"
#include "UART_1.h"


static volatile bool found_puck = false;
CY_ISR(handler) {
    stop();
    found_puck = true;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    // color_sensor_setup();
    // servo_setup();
    ir_sensor_setup(&handler, NULL, NULL);
    // UART_1_Start();

    // UART_1_PutString("Im ready!\n");
    move_forward_by(30);

    // char str[20];
    // char* str_ptr = str;
    // char c;
    // while (1) {
    //     if (UART_1_GetRxBufferSize() > 0) {
    //         c = UART_1_GetChar();

    //         if (c == '!') {
    //             *str_ptr = '\0';
    //             break;
    //         }

    //         *str_ptr = c;
    //         str_ptr++;
    //     }
    // }
    // uint32 counts = strtoul(str, &str_ptr, 10);
    // sprintf(str, "trying %lu\n", counts);
    // UART_1_PutString(str);
    // move_forward_by_counts(counts * 10);

    for(;;)
    {
        /* Place your application code here. */
        // char str[60];
        // sprintf(str, "L: %8li\t R: %8li\n", motor_l_quaddec_GetCounter(), motor_r_quaddec_GetCounter());
        // UART_1_PutString(str);
        // CyDelay(10);

        if (found_puck) {
            stop();
            ir_sensor_pause();
            show_code(0);

            move_forward_by(9);
            Color c = color_sense();
            switch (c) {
                case RED: panic(1); break;
                case GREEN: panic(2); break;
                case BLUE: panic(3); break;
            }
        }
    }
}

/* [] END OF FILE */
