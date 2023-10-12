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
#include "bluetooth.h"
#include "UART_1.h"


static volatile bool moved = false;
static volatile bool found_puck = false;
static volatile bool flag = false;
static CY_ISR(ir_handler) {
    stop();
    found_puck = true;
}

static CY_ISR(sw_isr) {
    static bool state = true;
    state = !state;
    led_Write(state);
}
static CY_ISR(sw_l_isr) {
    static bool state = true;
    state = !state;
    if (state) {
        // lifter_down();
        // flicker_up();
        flicker_shoot();
    }
    else{
        // lifter_up();
        // flicker_down();
        flicker_shoot();
    }
}
static CY_ISR(sw_r_isr) {
    static bool state = true;
    state = !state;
    if (state) {
        gripper_open();
        lifter_down();
        gripper_hold_closed();
        CyDelay(800);
        lifter_up();
        gripper_unactuate();

        // gripper_close();
    }
    else {
        lifter_down();
        gripper_open();
        lifter_up();
    }
    //flag = true;
    //led_r_Write(1);
}


int main_m(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // locomotion_setup();
    // color_sensor_setup();
    // servo_setup();
    ultrasonic_setup();
    // ir_sensor_setup(&ir_handler);
    // limit_sw_setup(sw_l_isr, sw_r_isr);
    // sw_isr_StartEx(sw_isr);
    bt_setup();

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
    // bt_printf("trying %lu\n", counts);
    // move_forward_by_counts(counts * 10);

    for(;;)
    {
        /* Place your application code here. */
        // Color c = color_sense();
        // switch (c) {
        //     case RED: {
        //         UART_1_PutString("(Red!) XD\n");
        //         break;
        //     }
        //     case GREEN: {
        //         UART_1_PutString("(Green!) :)\n");
        //         break;
        //     }
        //     case BLUE: {
        //         UART_1_PutString("(Blue!) :(\n");
        //         break;
        //     }
        // }

        // bt_printf("L: %8li\t R: %8li\n", motor_l_quaddec_GetCounter(), motor_r_quaddec_GetCounter());
        // CyDelay(10);
        // if (flag) {
        //     rotate_to_align();
        //     flag = false;
        //     led_r_Write(0);
        // }

        bt_printf("L %3.2f \t R %3.2f \t FL %3.2f \t FR %3.2f\n",
            us_l_get_dist(), us_r_get_dist(), us_fl_get_dist(), us_fr_get_dist());
        CyDelay(us_refresh());

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
        //     // gripper_hold_closed();
        //     // CyDelay(800);
        //     lifter_up();
        //     gripper_unactuate();

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
