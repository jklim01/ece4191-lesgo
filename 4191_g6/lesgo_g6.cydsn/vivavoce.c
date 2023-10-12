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
static CY_ISR(ir_handler) {
    stop();
    found_puck = true;
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
    ir_sensor_setup(&ir_handler);
    sw_isr_StartEx(sw_isr);

    // bt_setup();
    // bt_print("Im ready!\n");

    while (!sw_pressed);
    for(;;)
    {
        /* Place your application code here. */
        ir_sensor_resume();
        move_forward();
        while (!found_puck);
        ir_sensor_pause();

        // const float DETECTOR_TO_COLOR_SENSOR = 1.2;
        move_forward_by(DETECTOR_TO_COLOR_SENSOR);
        Color c = color_sense();

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
                unwind_navstack_till(0);

                break;
            }
            case BLUE: {
                const float COLOR_SENSOR_TO_FLICKER = 4.5;
                move_backward_by(COLOR_SENSOR_TO_FLICKER);
                flicker_shoot();

                break;
            }
        }

        panic(0);
    }
}

/* [] END OF FILE */
