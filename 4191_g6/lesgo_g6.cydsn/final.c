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
#include <math.h>

#include "utils.h"
#include "color_sensor.h"
#include "servo.h"
#include "locomotion.h"
#include "ir_sensor.h"
#include "ultrasonic.h"
#include "limit_sw.h"
#include "bluetooth.h"


typedef enum __attribute__ ((__packed__)) State {
    TEST,
    INIT,
    BASE_TO_PUCK_ZONE,
    BASE_TO_FIND_PINS,
    SEARCH_PUCK,
    TO_PINDECK,
    SHOOT_PUCK,
    RETURN_TO_BASE
} State;


static CY_ISR(ir_handler) {
    stop();
}

static CY_ISR(sw_l_isr) {
    static bool state = true;
    state = !state;
    if (state) {
        flicker_up();
    }
    else{
        flicker_down();
    }
}

static CY_ISR(sw_r_isr) {
    gripper_open();
    lifter_down();
    gripper_hold_closed();
    CyDelay(800);
    lifter_up();
    gripper_unactuate();
}

int main_f(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    ultrasonic_setup();
    ir_sensor_setup(&ir_handler);
    bt_setup();
    // limit_sw_setup(sw_l_isr, sw_r_isr);

    led_r_Write(0);
    led_g_Write(0);
    if (sw_sreg_Read() == 0) {
        base = BASE_LEFT;
        // bt_print("Left Base\n");
        led_r_Write(1);
    }
    else {
        base = BASE_RIGHT;
        // bt_print("Right Base\n");
        led_g_Write(1);
    }


    // wait until button is released before starting
    while (sw_sreg_Read() == 0);
    CyDelay(1500);

    // while(1) {
    //     bt_printf("L: %8li\t R: %8li\n", motor_l_quaddec_GetCounter(), motor_r_quaddec_GetCounter());
    //     CyDelay(400);
    // }


    // receive info from bluetooth
    BtResponse res = bt_init();
    uint8 current_lvl = res.current_lvl;
    Color lvl_1_color = ZONE_TO_COLOR[res.lvl_1_zone - 1];
    uint8 lvl_1_zone = res.lvl_1_zone;
    Color lvl_2_color = res.lvl_2_color;


    // main loop
    uint8 target_len;
    uint8 zone_number;
    Color puck_color;
    State state = INIT;
    // State state = TEST;
    FSM(state) {
        STATE(TEST) {
            // place down puck
            // move_backward_by(US_TO_GRIPPER);
            lifter_down();
            gripper_open();
            move_backward_by(3);
            lifter_up();
            gripper_close();

            // align puck with flicker and flick
            move_forward_by(14.5 + 3);
            flicker_shoot();

            panic(END_SUCCESS);
        } END_STATE

        STATE(INIT) {
            target_len = 0;

            if (current_lvl == 1) {
                zone_number = lvl_1_color;
                puck_color = lvl_1_color;
                state = BASE_TO_PUCK_ZONE;
            }
            else if (current_lvl == 2) {
                zone_number = 0;
                puck_color = lvl_2_color;
                state = BASE_TO_PUCK_ZONE;
            }
            else {
                zone_number = 0;
                state = BASE_TO_FIND_PINS;
            }
        } END_STATE

        STATE(BASE_TO_FIND_PINS) {
            // TODO:

            unwind_navstack_till(0);
            reverse_to_align();
            navstack_clear();

            state = BASE_TO_PUCK_ZONE;
        } END_STATE

        STATE(BASE_TO_PUCK_ZONE) {
            move_forward_by(30);
            if (base == BASE_LEFT) {
                turn_left();
                move_forward_by(25);
                turn_right();
                target_len = navstack_len();
                move_forward_by(55);
                turn_right();
            }
            else {
                turn_right();
                move_forward_by(55);
                turn_left();
                target_len = navstack_len();
                move_forward_by(55);
                turn_left();
                move_forward_by(30);
            }

            state = SEARCH_PUCK;
        } END_STATE

        STATE(SEARCH_PUCK) {
            // find desired puck
            uint8 puck_row = 1;
            bool change_row = false;
            while (1) {
                ir_sensor_resume();
                move_forward_nb();
                while (current_linear_movement != STOP) {
                    // if end of the row reached, switch to the next row
                    if (get_latest_movement_dist() >= ROW_LEN) {
                        change_row = true;
                        stop_nb();
                        break;
                    }
                }
                ir_sensor_pause();

                // sense color of detected puck
                if (!change_row) {
                    move_forward_by(DETECTOR_TO_COLOR_SENSOR);
                    if (color_sense() == lvl_1_color)
                        break;
                }

                // change to next row of pucks if all pucks on the line has been detected
                if ((base == BASE_LEFT && puck_row % 2 == 0) || (base == BASE_RIGHT && puck_row % 2 == 1)) {
                    turn_right();
                    move_forward_by(ROW_GAP);
                    turn_right();
                }
                else {
                    turn_left();
                    move_forward_by(ROW_GAP);
                    turn_left();
                }
                puck_row++;
                change_row = false;
            }

            // clear nearby puck just in case
            move_backward_by(COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER);
            flicker_shoot();

            // take puck
            move_backward_by(COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER));
            gripper_open();
            lifter_down();
            gripper_hold_closed();
            CyDelay(800);
            lifter_up();
            gripper_unactuate();

            // alignment with walls
            if (puck_row % 2 == 1)
                reverse_to_align();
            else {
                move_forward_nb();
                while (us_get_front_dist() > 30);
                stop_nb();
                rotate_to_align();
            }

            state = TO_PINDECK;
        } END_STATE

        STATE(TO_PINDECK) {
            // unwind_shortcut_navstack_till(target_len);        // at the top right of pindeck with +y heading
            unwind_navstack_till(target_len);        // at the top right of pindeck with +y heading
            move_backward_by(8);

            if (base == BASE_LEFT) {
                const float WALL_TO_LEFT_PINDECK = 50;
                const float err_tol = 3;
                turn_left();

                // adjust distance with side wall
                float dist = us_get_front_dist();
                float err = dist - WALL_TO_LEFT_PINDECK;
                if (err > err_tol) {
                    move_forward_nb();
                    while (us_get_front_dist() > WALL_TO_LEFT_PINDECK);
                    stop_nb();
                }
                else if (err < -err_tol) {
                    move_backward_nb();
                    while (us_get_front_dist() < WALL_TO_LEFT_PINDECK);
                    stop_nb();
                }

                rotate_to_align();
            }
            else {
                // const float SIDE_WALL_TO_RIGHT_PINDECK = 12;
                turn_left();
                reverse_to_align();
                // move_forward_by(SIDE_WALL_TO_RIGHT_PINDECK);
            }

            state = SHOOT_PUCK;
        } END_STATE

        STATE(SHOOT_PUCK) {
            const float SIDE_WALL_TO_ZONE1 = 20.0;
            float dist_to_wall = us_l_get_avg_dist();

            // find correct zone
            if (base == BASE_LEFT) {
                // move according to zone number
                move_forward_by((7 - lvl_1_zone) * 5);

                // fine tune using ultrasonic measurements
                CyDelay(500);
                bool pin_found = us_l_get_dist() > dist_to_wall - PIN_DIST_TOL;
                float expected_dist = (SIDE_WALL_TO_ZONE1 + 5*lvl_1_zone - 2.5) - ROBOT_FRONT_TO_US_OFFSET;
                while (!pin_found) {
                    float forward_dist = us_get_front_dist();
                    float adjustment = forward_dist - expected_dist;

                    if (adjustment > 0)
                        move_forward_by(adjustment);
                    else
                        move_backward_by(adjustment);
                    rotate_to_align();

                    CyDelay(500);
                    pin_found = us_l_get_dist() > dist_to_wall - PIN_DIST_TOL;
                }
            }
            else {
                // move according to zone number
                // move_forward_by(lvl_1_zone * 5);

                // algorithm to ensure pin is found
                // increment forward till pin is found
                move_forward_by(19);
                CyDelay(500);
                bt_printf("us_l_get_dist()=%.4f, dist_to_wall=%.4f\n", us_l_get_dist(), dist_to_wall);
                bool pin_found = us_l_get_dist() < dist_to_wall - PIN_DIST_TOL;

                // check in front
                if (!pin_found) {
                    uint8 move_counts = 0;

                    while (!pin_found && move_counts < 15) {
                        bt_print("blocking...\n");
                        bt_block_on("go");
                        move_forward_by(2.5);
                        move_counts++;
                        CyDelay(500);
                        bt_printf("us_l_get_dist() = %.4f\n", us_l_get_dist());
                        pin_found = us_l_get_dist() < dist_to_wall - PIN_DIST_TOL;
                    }
                }

                // retry from begining if still not found
                if (!pin_found) {
                    reverse_to_align();
                    continue;
                }
            }

            // place down puck
            move_backward_by(US_TO_GRIPPER);
            lifter_down();
            gripper_open();
            move_backward_by(3);
            lifter_up();
            gripper_close();

            // align puck with flicker and flick
            move_forward_by(GRIPPER_TO_FLICKER + 3);
            flicker_shoot();

            state = RETURN_TO_BASE;
        } END_STATE

        STATE(RETURN_TO_BASE) {
            // unwind_shortcut_navstack_till(0);
            unwind_navstack_till(0);
            panic(END_SUCCESS);

            if (current_lvl == 4)
                panic(END_SUCCESS);

            current_lvl++;
            CyDelay(5000);
            continue;
        } END_STATE

    } END_FSM

    return 0;
}

/* [] END OF FILE */
