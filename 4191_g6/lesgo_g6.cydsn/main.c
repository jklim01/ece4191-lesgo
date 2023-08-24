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
#include "utils.h"
#include "limit_sw.h"
#include "color_sensor.h"
#include "servo.h"
#include "locomotion.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void increment_select(void) {
    static uint8 demux_select = 0x00;
    static bool is_gripper_open = false;
    static bool is_lifter_up = false;
    static char str[200];

    shared_pwm_demux_select_Write(demux_select);
    switch (demux_select) {
        case 0x00: {
            shared_pwm_WritePeriod(1999);
            if (is_gripper_open) {
                is_gripper_open = false;
                gripper_close();
            }
            else {
                is_gripper_open = true;
                gripper_open();
            }

            sprintf(str, "[State 1] select = %x, gripper %s\n", shared_pwm_demux_select_Read(), is_gripper_open ? "open" : "close");
            UART_1_PutString(str);

            demux_select = 0x01;
            break;
        }
        case 0x01: {
            if (is_lifter_up) {
                is_lifter_up = false;
                lifter_down();
            }
            else {
                is_lifter_up = true;
                lifter_up();
            }

            sprintf(str, "[State 2] select = %x, lifter %s\n", shared_pwm_demux_select_Read(), is_lifter_up ? "up" : "down");
            UART_1_PutString(str);

            demux_select = 0x02;
            break;
        }
        case 0x02: {
            sprintf(str, "[State 3] select = %x\n", shared_pwm_demux_select_Read());
            UART_1_PutString(str);

            demux_select = 0x03;
            break;
        }
        case 0x03: {
            sprintf(str, "[State 4] select = %x\n", shared_pwm_demux_select_Read());
            UART_1_PutString(str);

            Color color = color_sense();
            switch (color) {
                case RED: {
                    UART_1_PutString("(RED!)\n");
                    break;
                }
                case GREEN: {
                    UART_1_PutString("(GREEN!)\n");
                    break;
                }
                case BLUE: {
                    UART_1_PutString("(BLUE!)\n");
                    break;
                }
            }

            demux_select = 0x00;
            break;
        }
    }
}

void increment_select_1(void) {
    static bool state = false;

    state = !state;
    if (state)
        //gripper_open();
        lifter_down();
    else
        lifter_up();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // setup_locomotion();
    // move_forward_by(10);
    setup_color_sensor();
    setup_servo();
    setup_limit_sw(&increment_select, NULL);

    UART_1_Start();
    UART_1_PutString("Im ready!\n");
    // char str[50];
    // sprintf(str, "%u, CMP=%u\n", shared_pwm_ReadPeriod(), shared_pwm_ReadCompare());
    // UART_1_PutString(str);

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
    }
}

/* [] END OF FILE */
