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


static volatile bool sw_pressed = false;
static volatile bool found_puck = false;
static CY_ISR(ir_b_handler) {
    stop();
    found_puck = true;
}

static CY_ISR(ir_f_handler) {
    stop();
    found_puck = true;
    led_r_Write(1);
}

static CY_ISR(sw_isr) {
    sw_pressed = true;
}


int main_v(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    ir_sensor_setup(&ir_f_handler, &ir_b_handler);
    sw_isr_StartEx(sw_isr);

    bt_setup();
    // bt_print("Im ready!\n");

    while (!sw_pressed);
    for(;;)
    {
        /* Place your application code here. */
        ir_b_resume();
        move_forward();
        ir_b_pause();

        // const float DETECTOR_TO_COLOR_SENSOR = 1.2;
        move_forward_by(DETECTOR_TO_COLOR_SENSOR);
        Color c = color_sense();
        found_puck = false;

        switch (c) {
            case RED: {
                // align puck with gripper
                // const float COLOR_SENSOR_TO_GRIPPER = 17.9;
                move_backward_by(COLOR_SENSOR_TO_GRIPPER);

                // pick up puck
                gripper_open();
                lifter_down();
                gripper_hold_closed();
                CyDelay(800);
                lifter_up();
                gripper_unactuate();

                // deposit puck
                move_forward_by(10);
                lifter_down();
                gripper_open();
                lifter_up();
                gripper_close();
                unwind_navstack_till(0);

                break;
            }
            case GREEN: {
                // align puck with gripper
                // const float COLOR_SENSOR_TO_GRIPPER = 17.9;
                move_backward_by(COLOR_SENSOR_TO_GRIPPER);

                // pick up puck
                gripper_open();
                lifter_down();
                gripper_hold_closed();
                CyDelay(800);
                lifter_up();
                gripper_unactuate();

                // deposit puck
                move_forward_by(20);
                lifter_down();
                gripper_open();
                lifter_up();
                gripper_close();
                unwind_navstack_till(0);

                break;
            }
            case BLUE: {
                // // const float COLOR_SENSOR_TO_FLICKER = 4.5;
                // move_backward_by(COLOR_SENSOR_TO_FLICKER);
                // flicker_shoot();

                // flick nearby undesired pucks
                move_backward_by(10.5);
                flicker_shoot();

                // take puck
                move_backward_by(10);
                gripper_open();
                lifter_down();
                move_forward_by(5);
                gripper_hold_closed();
                CyDelay(800);
                lifter_up();
                gripper_unactuate();

                // place down puck
                flicker_up();
                flicker_up();
                // move_backward_by(US_TO_GRIPPER);
                lifter_down();
                gripper_open();
                move_backward_by(3);
                lifter_up();
                gripper_close();

                // align puck with flicker and flick
                ir_f_resume();
                move_forward_nb();
                while (!found_puck);
                stop_nb();
                ir_f_pause();
                CyDelay(1500);
                // move_forward_by(GRIPPER_TO_FLICKER + 3);
                flicker_shoot();

                break;
            }

            case INVALID: {
                panic(3);
            }
        }

        panic(0);
    }
}

/* [] END OF FILE */
