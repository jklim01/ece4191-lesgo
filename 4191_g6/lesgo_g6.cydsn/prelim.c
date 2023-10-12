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


typedef enum __attribute__ ((__packed__)) State {
    LEAVE_BASE,
    FIND_SLIT,
    FIND_PUCK,
    DEPOSIT_PUCK,
    RETURN_TO_BASE
} State;


static CY_ISR(ir_handler) {
    stop();
}

volatile bool ready_to_print = false;
static CY_ISR(sw_handler) {
    ready_to_print = true;
}


int main_p(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    panic(END_SUCCESS);
    ultrasonic_setup();
    ir_sensor_setup(&ir_handler);
    ir_sensor_pause();

    bt_setup();
    bt_print("Im ready!\n");
    // while (1) {
    //     bt_printf("L %3f\t R %3f\t FL %3f\t FR %3f\n",
    //         us_l_get_dist(), us_r_get_dist(), us_fl_get_dist(), us_fr_get_dist());
    //     CyDelay(200);
    // }
    led_r_Write(0);
    led_g_Write(0);
    if (sw_Read() == 0) {
        led_r_Write(1);
        base = BASE_LEFT;
    }
    else {
        base = BASE_RIGHT;
        led_g_Write(1);
    }


    // wait until button is released before starting
    while (sw_Read() == 0);
    CyDelay(1500);

    uint8 target_len = 0;
    State state = LEAVE_BASE;
    FSM(state) {
        STATE(LEAVE_BASE) {
            const float OBSTACLE_DIST_THRESH = 25;
            const float OBSTACLE_Y_POS = 100;

            // move forward till distance to obstacle is below threshold
            reverse_to_align();
            move_forward_nb();
            while (us_fl_get_dist() > OBSTACLE_DIST_THRESH && us_fr_get_dist() > OBSTACLE_DIST_THRESH);
            stop_nb();

            state = FIND_SLIT;
        } END_STATE

        STATE(FIND_SLIT) {
            const float WALL_DIST_THRESH = 20;
            const float SLIT_DIST_THRESH = 80;
            const float US_SENSOR_OFFSET = 21;

            // turn to be parallel with the obstacle
            if (base == BASE_LEFT)
                turn_right();
            else
                turn_left();


            // repeat algorithm until the slit is found
            bool slit_found = false;
            while (!slit_found) {
                // reverse to wall to prepare for next try
                if (base == BASE_RIGHT)
                    target_len = navstack_len();

                print_navstack();
                reverse_to_align();
                print_navstack();

                // move forward along the obstacle, stopping and setting the flag if the slit is found
                move_forward_nb();
                while (us_fl_get_dist() > WALL_DIST_THRESH && us_fr_get_dist() > WALL_DIST_THRESH) {
                    if (base == BASE_LEFT) {
                        if (us_l_get_dist() > SLIT_DIST_THRESH) {
                            slit_found = true;
                            break;
                        }
                    }
                    else if (us_r_get_dist() > SLIT_DIST_THRESH) {
                        slit_found = true;
                        break;
                    }
                }
                stop_nb();
                print_navstack();
            }

            // move forward slightly to ensure the robot can pass through the slit
            led_r_Write(1);
            move_forward_by(US_SENSOR_OFFSET);
            led_r_Write(0);

            // turn to face the slit
            if (base == BASE_LEFT)
                turn_left();
            else
                turn_right();

            state = FIND_PUCK;
        } END_STATE

        STATE(FIND_PUCK) {
            // const float DETECTOR_TO_COLOR_SENSOR = 1.2;
            // const float COLOR_SENSOR_TO_GRIPPER = 17.9;

            // move forward until puck detected
            // TODO: handle case if puck not found by the time the robot reaches the wall
            ir_sensor_resume();
            move_forward();
            // while (current_linear_movement != STOP);
            ir_sensor_pause();

            // align color sensor with puck and detect color
            move_forward_by(DETECTOR_TO_COLOR_SENSOR);
            Color c = color_sense();

            // align puck with gripper
            move_backward_by(COLOR_SENSOR_TO_GRIPPER);

            // pick up puck
            gripper_open();
            lifter_down();
            gripper_hold_closed();
            lifter_up();
            gripper_unactuate();

            state = DEPOSIT_PUCK;
        } END_STATE

        STATE(DEPOSIT_PUCK) {
            // move to puck landing zone
            print_navstack();
            print_unwind_result(target_len);
            unwind_navstack_till(target_len);
            panic(END_SUCCESS);
            // reverse_to_align();
            if (base == BASE_LEFT) {
                reverse_to_align();
                move_forward_by(20);
                turn_left();
                move_forward_nb();
                while ((us_fl_get_dist() + us_fr_get_dist())/2 > 50);
                rotate_to_align();
                stop_nb();
            }
            else {
                // parallel to obstacle with back facing wall now
                unwind_navstack_till(target_len);
                reverse_to_align();
                move_forward_by(15);
                turn_left();
                move_forward_nb();
                while ((us_fl_get_dist() + us_fr_get_dist())/2 > 10);
                rotate_to_align();
                stop_nb();
                turn_right();
            }

            // drop puck
            lifter_down();
            gripper_open();
            lifter_up();
            gripper_close();

            // align puck with flicker, then flick
            move_forward_by(13);
            flicker_shoot();

            state = RETURN_TO_BASE;
        } END_STATE

        STATE(RETURN_TO_BASE) {
            // unwind_navstack_till(0);
            if (base == BASE_LEFT) {
                move_backward_by(35);
                turn_right();
            }
            else {
                move_forward_by(55);
                turn_right();
            }
            reverse_to_align();


            // DEBUGGING
            // sw_isr_ClearPending();
            // sw_isr_StartEx(sw_handler);
            // led_Write(1);
            // while (!ready_to_print) {
            //     print_navstack(len);
            // }
            panic(END_SUCCESS);

        } END_STATE

    } END_FSM

    return 0;
}

/* [] END OF FILE */
