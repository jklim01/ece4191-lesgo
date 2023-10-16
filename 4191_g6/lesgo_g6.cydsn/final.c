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
#include "bt_debug.h"

#define MY_DEBUG
#ifndef MY_DEBUG
#define bt_print (void)sizeof
#define bt_printf (void)sizeof
#define bt_block_on (void)sizeof
#endif


#define LIST_OF_STATES  \
    X(BT_DBG)   \
    X(TEST) \
    X(INIT) \
    X(BASE_TO_PINDECK)    \
    X(FIND_PIN)   \
    X(BASE_TO_PUCK_ZONE)    \
    X(PINDECK_TO_PUCK_ZONE)  \
    X(SEARCH_PUCK)  \
    X(PUCK_ZONE_TO_PINDECK)   \
    X(SHOOT_PUCK)   \
    X(RETURN_TO_BASE)


#define X(name) name,
typedef enum State {
    LIST_OF_STATES
} State;
#undef X


static bool puck_found = false;
static CY_ISR(ir_handler) {
    stop_nb();
    puck_found = true;
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

void unwind_shortcut_back_to_base(void) {
    unwind_shortcut_navstack_till(1);
    reverse_to_align();
    clear_navstack();
    pos_y = 0.0;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    bt_setup();
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    ultrasonic_setup();
    ir_sensor_setup(&ir_handler);
    // limit_sw_setup(sw_l_isr, sw_r_isr);

    led_r_Write(0);
    led_g_Write(0);
    if (sw_sreg_Read() == 0) {
        base = BASE_LEFT;
        bt_print("Left Base\n");
        led_r_Write(1);
    }
    else {
        base = BASE_RIGHT;
        bt_print("Right Base\n");
        led_g_Write(1);
    }

    // gripper_open();
    // lifter_down();
    // gripper_hold_closed();
    // CyDelay(800);
    // lifter_up();
    // gripper_unactuate();


    // wait until button is released before starting
    while (sw_sreg_Read() == 0);
    CyDelay(1500);
    led_r_Write(0);
    led_g_Write(0);


    // while (1) {
    //     bt_printf("L: %8li\t R: %8li\n", motor_l_quaddec_GetCounter(), motor_r_quaddec_GetCounter());
    //     CyDelay(400);
    // }


    // receive info from bluetooth
    BtResponse res = bt_init();
    // BtResponse res;
    uint8 current_lvl = res.current_lvl;
    Color lvl_1_color = ZONE_TO_COLOR[res.lvl_1_zone - 1];
    uint8 lvl_1_zone = res.lvl_1_zone;
    Color lvl_2_color = res.lvl_2_color;

    // char res_str[3];
    bool enter_dbg =  false;
    // bt_print("remote control? [y/n]  ");
    // bt_scanf("%2s", res_str);
    // if (strcmp(res_str, "y") == 0)
    //     enter_dbg = true;


    // main loop
    uint8 target_len = 0;
    uint8 zone_number = 0;
    Color puck_color = RED;
    bool flick_at_pindeck = false;
    uint8 puck_row = 1;
    State state = enter_dbg ? BT_DBG : INIT;
    FSM(state) {
        STATE(BT_DBG) {
            panic(bt_dbg());
        } END_STATE

        STATE(TEST) {
            int res = 0;
            float kp, ki, kd;
            while (res != 3) {
                bt_printf("%3.2f %3.2f %3.2f -> ", K_P, K_I, K_D);
                res = bt_scanf("%f %f %f", &kp, &ki, &kd);
            }
            K_P = kp; K_I = ki; K_P = kp;

            move_forward_by(150);
            bt_block_on("go", "");
            move_backward_by(150);
        } END_STATE

        STATE(INIT) {
            target_len = 0;
            puck_row = 0;

            if (current_lvl == 1) {
                zone_number = lvl_1_zone;
                puck_color = lvl_1_color;
                flick_at_pindeck = true;
                state = BASE_TO_PUCK_ZONE;
            }
            else if (current_lvl == 2) {
                zone_number = 0;
                puck_color = lvl_2_color;
                flick_at_pindeck = true;
                state = BASE_TO_PUCK_ZONE;
            }
            else {
                flick_at_pindeck = false;
                state = BASE_TO_PINDECK;
            }
        } END_STATE

        STATE(BASE_TO_PINDECK) {
            const float DIST_TO_SIDE_WALL = 60;

            move_forward_by(30);
            if (base == BASE_LEFT)
                turn_left();
            else
                turn_right();

            move_forward_slow_nb();
            while (us_get_front_dist() > DIST_TO_SIDE_WALL);
            stop_nb();
            rotate_to_align();

            state = PINDECK_TO_PUCK_ZONE;
        } END_STATE

        STATE(FIND_PIN) {
            const float DEVIATION_TOL = 4;
            float (*dist_measurer)(void) = (base == BASE_LEFT) ? &us_l_get_avg_dist : &us_r_get_avg_dist;

            // algorithm to find pin
            bool pin_found = false;
            float init_dist_to_wall = dist_measurer();
            float dist_to_wall = init_dist_to_wall;
            if (dist_to_wall > PIN_DIST_TOL) {                              // only proceed if not too close to the back wall, otherwise retry
                while (us_get_front_dist() > 15) {
                    move_forward_slow_by(2.5);
                    rotate_to_align();
                    float dist = dist_measurer();
                    pin_found = dist < dist_to_wall - PIN_DIST_TOL;
                    bt_block_on("go", "%5.2f %c (dist_to_wall=%5.2f) - %3.1f  %c\n", dist, pin_found ? '<' : '>',
                        dist_to_wall, PIN_DIST_TOL, pin_found ? "!" : "X");

                    // account for the robot moving further or close to the pin deck
                    dist_to_wall = dist_measurer();
                }
            }

            // retry from begining if still not found
            if (!pin_found) {
                unwind_shortcut_back_to_base();
                state = BASE_TO_PINDECK;
                continue;
            }

            if (flick_at_pindeck)
                state = SHOOT_PUCK;
            else {
                flick_at_pindeck = true;            // will be flicking the next time we come back to the pin deck
                state = PINDECK_TO_PUCK_ZONE;

                // if the robot gets too close to the pin deck, realign and return to the base
                if (dist_to_wall < init_dist_to_wall - DEVIATION_TOL) {
                    bt_block_on("go", "%5.2f < (init=%5.2f) - %3.1f, left alignment...\n", dist_to_wall, init_dist_to_wall, PIN_DIST_TOL);
                    move_backward_by(7);
                    rotate_to_align_l();
                    unwind_shortcut_back_to_base();
                    state = BASE_TO_PUCK_ZONE;
                }
                else if (dist_to_wall > init_dist_to_wall - DEVIATION_TOL) {
                    bt_block_on("go", "%5.2f > (init=%5.2f) - %3.1f, right alignment...\n", dist_to_wall, init_dist_to_wall, PIN_DIST_TOL);
                    move_backward_by(7);
                    rotate_to_align_r();
                    unwind_shortcut_back_to_base();
                    state = BASE_TO_PUCK_ZONE;
                }
                else {

                }
            }
        } END_STATE

        STATE(PINDECK_TO_PUCK_ZONE) {
            move_forward_slow_nb();
            while (us_get_front_dist() > 25);
            stop_nb();
            rotate_to_align();

            if (base == BASE_LEFT) {
                turn_right();
                move_forward_by(40);
                turn_right();
            }
            else {
                turn_left();
                move_forward_by(40);
                turn_left();
            }

            reverse_to_align();

            state = SEARCH_PUCK;
        } END_STATE

        STATE(BASE_TO_PUCK_ZONE) {
            const float DIST_TO_SIDE_WALL = 35;

            move_forward_by(40);
            if (base == BASE_LEFT) {
                turn_left();

                // move_forward_by(25);
                move_forward_slow_nb();
                while (us_get_front_dist() > DIST_TO_SIDE_WALL);
                stop_nb();
                rotate_to_align();

                turn_right();
                move_forward_by(50);
                turn_right();
            }
            else {
                turn_right();

                // move_forward_by(25);
                move_forward_slow_nb();
                while (us_get_front_dist() > DIST_TO_SIDE_WALL);
                stop_nb();
                rotate_to_align();

                turn_left();
                move_forward_by(50);
                turn_left();
            }

            state = SEARCH_PUCK;
        } END_STATE

        STATE(SEARCH_PUCK) {
            // find desired puck
            bool change_row = false;
            bool wanted_puck_found = false;
            puck_row = 1;

            while (!wanted_puck_found) {
                // advance until puck found or end of the row is reached
                ir_sensor_resume();
                move_forward_nb();
                while (current_linear_movement != STOP) {
                    // if end of the row reached, switch to the next row
                    if (fabsf(pos_x) >= ROW_LEN) {
                        change_row = true;
                        stop_nb();
                        break;
                    }
                }
                ir_sensor_pause();

                // sense color of detected puck
                while (puck_found) {
                    move_forward_by(DETECTOR_TO_COLOR_SENSOR);

                    ir_sensor_pause();
                    CyDelay(100);
                    if (color_sense() == puck_color) {
                        puck_found = false;
                        wanted_puck_found = true;
                        break;
                    }

                    // let the undesired puck pass first and check for the next puck
                    move_forward_by(PUCK_DIAMETER / 2 + 1);
                    ir_sensor_resume();
                    CyDelay(100);
                    puck_found = ir_is_detected();
                }

                // change to next row of pucks if all pucks on the line has been detected
                if (change_row) {
                    change_row = false;

                    // alignment
                    move_forward_nb();
                    while (us_get_front_dist() > 30);
                    stop_nb();
                    rotate_to_align();

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
                }
            }

            // clear nearby puck just in case
            // move_backward_by(COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER - 0.5);
            // move_backward_by(COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER + 1);
            move_backward_by(8.5);
            flicker_shoot();

            // take puck
            // move_backward_by(COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER));
            // move_backward_by(6.9);

            // move_backward_by(8);
            // gripper_open();
            // lifter_down();
            // gripper_hold_closed();
            // CyDelay(800);
            // lifter_up();
            // gripper_unactuate();
            move_backward_by(10);
            gripper_open();
            lifter_down();
            move_forward_by(4);
            gripper_hold_closed();
            CyDelay(800);
            lifter_up();
            gripper_unactuate();

            // alignment with side wall (left base has shortcut path back so unneeded)
            if (base == BASE_RIGHT) {
                if (puck_row % 2 == 1)
                    reverse_to_align();
                else {
                    move_forward_slow_nb();
                    while (us_get_front_dist() > 30);
                    stop_nb();
                    rotate_to_align();

                    // end with robot back facing side wall
                    turn_right(); turn_right(); reverse_to_align();
                }
            }

            state = PUCK_ZONE_TO_PINDECK;
        } END_STATE

        STATE(PUCK_ZONE_TO_PINDECK) {
            const float DIST_TO_BACK_WALL = 28;

            if (base == BASE_LEFT) {
                // const float DIST_TO_SIDE_WALL = 50;
                // const float err_tol = 3;

                // go back to base first
                unwind_shortcut_back_to_base();

                state = BASE_TO_PINDECK;
            }
            else {
                // turn to have the back facing the back wall
                move_forward_by(15);
                turn_right();
                move_backward_by(40);

                // adjust distance of robot to the back wall behind and turn s.t. pindeck on the left
                move_backward_slow_nb();
                while (us_b_get_dist() > DIST_TO_BACK_WALL);
                stop_nb();
                turn_left();

                move_forward_by(19);
            }

            state = FIND_PIN;
        } END_STATE

        STATE(SHOOT_PUCK) {
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
            // unwind_navstack_till(1);
            unwind_shortcut_back_to_base();
            panic(END_SUCCESS);

            if (current_lvl == 4)
                panic(END_SUCCESS);

            current_lvl++;
            CyDelay(5000);
            state = INIT;
            continue;
        } END_STATE

    } END_FSM

    return 0;
}


#undef LIST_OF_STATES
#undef MY_DEBUG
#undef bt_print
#undef bt_printf
#undef bt_block_on
/* [] END OF FILE */
