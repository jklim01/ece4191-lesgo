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
// #include "UART_1.h"


typedef enum __attribute__ ((__packed__)) State {
    LEAVE_BASE,
    FIND_SLIT,
    FIND_PUCK,
    DEPOSIT_PUCK,
    RETURN_TO_BASE
} State;


CY_ISR(ir_handler) {
    stop();
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    ultrasonic_setup();
    ir_sensor_setup(&ir_handler, NULL, NULL);
    ir_sensor_pause();

    // UART_1_Start();
    // UART_1_PutString("Im ready!\n");
    // char str[20];
    State state = LEAVE_BASE;
    FSM(state) {
        STATE(LEAVE_BASE) {
            const uint16 OBSTACLE_DIST_THRESH = 20;
            const float OBSTACLE_Y_POS = 100;

            // move forwrad till distance to obstacle is below threshold
            move_forward_nb();
            while (us_fl_get_dist() > OBSTACLE_DIST_THRESH && us_fr_get_dist() > OBSTACLE_DIST_THRESH);
            stop_nb();

            if (pos_y > OBSTACLE_Y_POS)
                // if the robot has already entered the slit
                state = FIND_PUCK;
            else
                state = FIND_SLIT;
        } END_STATE

        STATE(FIND_SLIT) {
            const uint16 WALL_DIST_THRESH = 8;
            const uint16 SLIT_DIST_THRESH = 20;
            const float us_sensor_offset = 12;
            bool slit_found = false;

            // turn to face direction with larger distance and reverse
            if (us_l_get_dist() > us_r_get_dist())
                turn_left();
            else
                turn_right();

            // repeat algorithm until the slit is found
            while (!slit_found) {
                // move forward along the obstacle, stopping and setting the flag if the slit is found
                move_forward_nb();
                while (us_fl_get_dist() > WALL_DIST_THRESH && us_fr_get_dist() > WALL_DIST_THRESH) {
                    if (us_r_get_dist() > SLIT_DIST_THRESH) {
                        slit_found = true;
                        break;
                    }
                }
                stop_nb();

                // reverse to wall to prepare for next try
                reverse_to_align();
            }

            // move forward slightly to ensure the robot can pass through the slit
            move_forward_by(us_sensor_offset);

            // turn to face the slit
            turn_right();

            state = FIND_PUCK;
        } END_STATE

        STATE(FIND_PUCK) {
            const float DETECTOR_TO_COLOR_SENSOR = 1.2;
            const float COLOR_SENSOR_TO_GRIPPER = 17.8;

            // move forward until puck detected
            ir_sensor_resume();
            move_forward();
            while (current_linear_movement != STOP);
            ir_sensor_pause();

            // align color sensor with puck and detect color
            move_forward_by(DETECTOR_TO_COLOR_SENSOR);
            Color c = color_sense();

            // align gripper with puck and pick up
            move_backward_by(COLOR_SENSOR_TO_GRIPPER);
            gripper_open();
            lifter_down();
            gripper_close();
            lifter_up();

            state = DEPOSIT_PUCK;
        } END_STATE

        STATE(DEPOSIT_PUCK) {


        } END_STATE

        STATE(RETURN_TO_BASE) {
            unwind_navstack_till(0);
            panic(END_SUCCESS);
        } END_STATE

    } END_FSM
}

/* [] END OF FILE */
