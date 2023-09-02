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
#include "ultrasonic.h"
#include "limit_sw.h"
#include "UART_1.h"


static volatile bool moved = false;
static volatile bool found_puck = false;
CY_ISR(handler) {
    stop();
    found_puck = true;
}

CY_ISR(sw_isr) {
    static bool state = true;
    state = !state;
    led_Write(state);
}
CY_ISR(sw_l_isr) {
    static bool state = true;
    state = !state;
    if (state)
        lifter_down();
    else
        lifter_up();
}
CY_ISR(sw_r_isr) {
    static bool state = true;
    state = !state;
    if (state)
        gripper_open();
    else
        gripper_close();
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // locomotion_setup();
    // color_sensor_setup();
    // servo_setup();
    ultrasonic_setup();
    // ir_sensor_setup(&handler, NULL, NULL);
    limit_sw_setup(sw_l_isr, sw_r_isr);
    sw_isr_StartEx(sw_isr);

    UART_1_Start();
    UART_1_PutString("Im ready!\n");
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

        // char str[50];
        // sprintf(str, "L %3u\t R %3u\t FL %3u\t FR %3u\n",
        //     us_l_get_dist(), us_r_get_dist(), us_fl_get_dist(), us_fr_get_dist());
        // UART_1_PutString(str);
        // CyDelay(200);

        // led_Write(moved);
        // if (moved) {
        //     move_forward_by(10);
            // reverse_to_align();
            // CyDelay(700);
            // move_forward_by(25);
        //     moved = false;
        // }

        // if (!moved) {
        //     move_forward_by(30);
        //     moved = true;
        // }

        // if (found_puck) {
        //     ir_sensor_pause();
        //     show_code(0);

        //     move_forward_by(1.2);
        //     Color c = color_sense();
        //     // switch (c) {
        //     //     case RED: show_code(1); break;
        //     //     case GREEN: show_code(2); break;
        //     //     case BLUE: show_code(3); break;
        //     // }

        //     move_backward_by(17.8);
        //     gripper_open();
        //     lifter_down();
        //     gripper_close();
        //     lifter_up();

        //     move_forward_by(10);
        //     lifter_down();
        //     gripper_open();
        //     lifter_up();
        //     gripper_close();

        //     found_puck = false;
        //     moved = false;
        //     move_backward_by(15);
        //     ir_sensor_resume();
        // }
    }
}

/* [] END OF FILE */
