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
#include "NavStack.h"

#define MY_DEBUG
#ifndef MY_DEBUG
#define bt_print (void)sizeof
#define bt_printf (void)sizeof
#define bt_block_on (void)sizeof
#endif

#define ALIGN_UPDATE_POS
#define OBSTACLE_AVOIDANCE(meas, react, recover, patience) obstacle_avoidance(meas, react, recover, patience, #meas)
#define REFRESH_OBSTACLE_AVOIDANCE(meas, react, recover, patience) refresh_obstacle_avoidance(meas, react, recover, patience, #meas)
#ifdef OBSTACLE_AVOIDANCE
bool obstacle_avoidance(float (*measurer)(void), void (*reaction)(void), void (*recovery)(void), uint8 patience, const char* measurer_name) {
    float dist = measurer();
    if (dist < OBSTACLE_DIST_THRESH) {
        reaction();

        uint8 num_wait = 0;
        while ((dist = measurer()) < OBSTACLE_DIST_THRESH && num_wait < 3) {
            num_wait++;
            bt_printf("Obstacle! %s()=%.2f (num_wait=%u)\n", measurer_name, dist, num_wait);
            CyDelay(4000);
        }

        if (num_wait == 3) return true;
        recovery();
    }
    return false;
}
bool refresh_obstacle_avoidance(float (*measurer)(void), void (*reaction)(void), void (*recovery)(void), uint8 patience, const char* measurer_name) {
    CyDelay(us_refresh());
    return obstacle_avoidance(measurer, reaction, recovery, patience, measurer_name);
}
#else
#define OBSTACLE_AVOIDANCE(...) false
#define REFRESH_OBSTACLE_AVOIDANCE(...) false
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
static CY_ISR(ir_b_handler) {
    stop_nb();
    puck_found = true;
}

static CY_ISR(ir_f_handler) {
    stop_nb();
    puck_found = true;
    led_r_Write(1);
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

void nop(void) {}

void unwind_shortcut_back_to_base(void) {
    unwind_shortcut_navstack_till(1);
    reverse_to_align();
    clear_navstack();
#ifdef ALIGN_UPDATE_POS
    bt_printf("pos_y: %.2f -> %.2f\n", pos_y, 0.0);
    pos_y = 0.0;
#endif
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
    // ir_sensor_setup(&ir_f_handler, &ir_b_handler);
    ir_sensor_setup(NULL, &ir_b_handler);
    // limit_sw_setup(sw_l_isr, sw_r_isr);

    led_r_Write(0);
    led_g_Write(0);
    if (sw_sreg_Read() == 0) {
        base = BASE_RIGHT;
        bt_print("Right Base\n");
        led_g_Write(1);
    }
    else {
        base = BASE_LEFT;
        bt_print("Left Base\n");
        led_r_Write(1);
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
    // BtResponse res;
    BtResponse res = bt_init();
    uint8 current_lvl = res.current_lvl;
    Color lvl_1_color = ZONE_TO_COLOR[res.lvl_1_zone - 1];
    uint8 lvl_1_zone = res.lvl_1_zone;
    Color lvl_2_color = res.lvl_2_color;

    // char res_str[3];
    bool enter_dbg =  false;
    // bt_print("remote control? [y/n]  ");
    // bt_scanf("%2s", res_str);
    // if (strcmp(res_str, "y") == 0)
    //     enter_dbg = true;.


    // main loop
    uint8 target_len = 0;
    uint8 zone_number = 0;
    Color puck_color = RED;
    bool flick_at_pindeck = false;
    uint8 puck_row = 1;
    State state = enter_dbg ? BT_DBG : INIT;
    // state = TEST;

    FSM(state) {
        STATE(BT_DBG) {
            panic(bt_dbg());
        } END_STATE

        STATE(TEST) {
            while (!puck_found) {
                ir_f_resume();
                CyDelay(200);
                move_forward_by_nb(20);
                while (!puck_found && current_linear_movement != STOP);
                stop_nb();
                ir_f_pause();

                if (!puck_found)
                    move_backward_by(20);
            }
            puck_found = false;
            CyDelay(1500);
            move_backward_by(1.0);
            flicker_shoot();
            panic(0);

            // int res = 0;
            // float kp, ki, kd;
            // int16 offset = 0;
            // while (res != 3) {
            //     bt_printf("%3.2f %3.2f %3.2f -> ", K_P, K_I, K_D);
            //     res = bt_scanf("%f %f %f", &kp, &ki, &kd);
            // }
            // K_P = kp; K_I = ki; K_D = kd;
            // K_P_slow = kp; K_I_slow = ki; K_D_slow = kd;

            // res = 0;
            // while (res != 1) {
            //     bt_print("FWD_SLAVE_OFFSET?  ");
            //     res = bt_scanf("%d", &offset);
            // }
            // FORWARD_SLAVE_OFFSET_DAMN_SLOW = offset;

            // move_forward_damn_slow_by(100);
            // // turn_right();
            // bt_printf("FWD %.2f %.2f %.2f offset %d", kp, ki, kd, FORWARD_SLAVE_OFFSET_DAMN_SLOW);
            // // bt_printf("RIGHT %.2f %.2f %.2f offset %d", kp, ki, kd, TURN_FORWARD_SLAVE_OFFSET);

            // // res = 0;
            // // while (res != 1) {
            // //     bt_print("BWD_SLAVE_OFFSET?  ");
            // //     res = bt_scanf("%d", &offset);
            // // }
            // // BACKWARD_SLAVE_OFFSET_DAMN_SLOW = offset;

            // // move_backward_damn_slow_by(100);
            // // turn_left();
            // // bt_printf("BWD %.2f %.2f %.2f offset %d", kp, ki, kd, BACKWARD_SLAVE_OFFSET_DAMN_SLOW);
            // // bt_printf("LEFT %.2f %.2f %.2f offset %d", kp, ki, kd, TURN_BACKWARD_SLAVE_OFFSET);

            // reverse_to_align();
        } END_STATE

        STATE(INIT) {
            target_len = 0;
            puck_row = 0;

            reverse_to_align();
            clear_navstack();
        #ifdef ALIGN_UPDATE_POS
            bt_printf("pos_y: %.2f -> %.2f\n", pos_y, 0.0);
            pos_y = 0.0;
        #endif
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

            if (flick_at_pindeck)
                move_forward_by(22);
                // move_forward_by(25);
            else
                move_forward_by(25);

            if (base == BASE_LEFT)
                turn_left();
            else
                turn_right();

            // rotate_to_align();
            move_forward_slow_nb();
            while (us_get_front_dist() > DIST_TO_SIDE_WALL);
            stop_nb();
            // rotate_to_align();

            state = FIND_PIN;
        } END_STATE

        STATE(FIND_PIN) {
            const float DEVIATION_TOL = 4;
            // float (*dist_measurer)(void) = (base == BASE_RIGHT && !flick_at_pindeck) ? &us_r_get_avg_dist : &us_l_get_avg_dist;
            float (*dist_measurer)(void) = (base == BASE_RIGHT && !flick_at_pindeck) ? &us_r_get_dist : &us_l_get_dist;

            // algorithm to find pin
            bool pin_found = false;
            float init_dist_to_wall = dist_measurer();
            float dist_to_wall = init_dist_to_wall;
            // const float dist_to_wall = 20.0;
            bt_printf("dist_to_wall = %.2f\n", init_dist_to_wall);
            if (dist_to_wall > PIN_DIST_TOL) {                              // only proceed if not too close to the back wall, otherwise retry
                if (base == BASE_LEFT) {
                    move_forward_special_nb();
                    while (us_get_front_dist() > 15 && !(pin_found = (dist_measurer() < dist_to_wall - PIN_DIST_TOL)));
                }
                else if (!flick_at_pindeck) {
                    move_forward_damn_slow_by_nb(55);
                    while (us_get_front_dist() > 15 && !(pin_found = (dist_measurer() < dist_to_wall - PIN_DIST_TOL)));
                }
                else {
                    move_forward_special_by_nb(55);
                    while (!(pin_found = (dist_measurer() < dist_to_wall - PIN_DIST_TOL)) && current_linear_movement != STOP);
                }
                stop_nb();
                bt_printf("distance %.2f, found puck!\n", dist_measurer());
            }


            // retry from begining if still not found
            if (!pin_found) {
                if (base == BASE_LEFT || !flick_at_pindeck) {
                    unwind_shortcut_back_to_base();
                    state = BASE_TO_PINDECK;
                }
                else {
                    reverse_to_align();
#ifdef ALIGN_UPDATE_POS
                    bt_printf("pos_x: %.2f -> %.2f\n", pos_x, MAX_X_2);
                    pos_x = MAX_X_2;
#endif
                }
                continue;
            }

            if (flick_at_pindeck)
                state = SHOOT_PUCK;
            else {
                flick_at_pindeck = true;            // will be flicking the next time we come back to the pin deck
                state = PINDECK_TO_PUCK_ZONE;

                float front_dist = us_get_front_avg_dist();
                if (front_dist < 23) {
                    zone_number = 1;
                    puck_color = RED;
                    bt_printf("front_dist = %.2f < 23.0 => zone %u\n", front_dist, zone_number);
                }
                else if (front_dist < 27) {
                    zone_number = 2;
                    puck_color = GREEN;
                    bt_printf("front_dist = %.2f < 27.0 => zone %u\n", front_dist, zone_number);
                }
                else if (front_dist < 31.5) {
                    zone_number = 3;
                    puck_color = BLUE;
                    bt_printf("front_dist = %.2f < 31.5 => zone %u\n", front_dist, zone_number);
                }
                else if (front_dist < 36) {
                    zone_number = 4;
                    puck_color = RED;
                    bt_printf("front_dist = %.2f < 36.0 => zone %u\n", front_dist, zone_number);
                }
                else if (front_dist < 42) {       // 40
                    zone_number = 5;
                    puck_color = GREEN;
                    bt_printf("front_dist = %.2f < 42.0 => zone %u\n", front_dist, zone_number);
                }
                else {
                    zone_number = 6;
                    puck_color = BLUE;
                    bt_printf("front_dist = %.2f > 42.0 => zone %u\n", front_dist, zone_number);
                }

                // if the robot gets too close to the pin deck, realign and return to the base
                // if (dist_to_wall < init_dist_to_wall - DEVIATION_TOL) {
                //     bt_block_on("go", "%5.2f < (init=%5.2f) - %3.1f, left alignment...\n", dist_to_wall, init_dist_to_wall, PIN_DIST_TOL);
                //     move_backward_by(7);
                //     rotate_to_align_r();
                //     unwind_shortcut_back_to_base();
                //     state = BASE_TO_PUCK_ZONE;
                // }
                // else if (dist_to_wall > init_dist_to_wall - DEVIATION_TOL) {
                //     bt_block_on("go", "%5.2f > (init=%5.2f) - %3.1f, right alignment...\n", dist_to_wall, init_dist_to_wall, PIN_DIST_TOL);
                //     move_backward_by(7);
                //     rotate_to_align_l();
                //     unwind_shortcut_back_to_base();
                //     state = BASE_TO_PUCK_ZONE;
                // }
            }
        } END_STATE

        STATE(PINDECK_TO_PUCK_ZONE) {
            // move_backward_slow_nb();
            // while (us_get_front_dist() < 30);
            // stop_nb();
            // rotate_to_align();

            // if (base == BASE_LEFT) {
            //     turn_right();
            //     move_forward_by(75);
            //     turn_right();
            // }
            // else {
            //     turn_left();
            //     move_forward_by(75);
            //     turn_left();
            // }

            // reverse_to_align();

            unwind_shortcut_back_to_base();
            state = BASE_TO_PUCK_ZONE;
            // state = SEARCH_PUCK;
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
                // rotate_to_align();

                turn_right();
                move_forward_by(58);
                turn_right();
            }
            else {
                turn_right();

                // move_forward_by(25);
                move_forward_slow_nb();
                while (us_get_front_dist() > DIST_TO_SIDE_WALL);
                stop_nb();
                // rotate_to_align();

                turn_left();
                move_forward_by(58);    // 50
                turn_left();
            }

            state = SEARCH_PUCK;
        } END_STATE

        STATE(SEARCH_PUCK) {
            // find desired puck
            bool change_row = false;
            bool wanted_puck_found = false;
            puck_row = 1;

            track_linear_run = true;
            linear_run = 0;
            bool is_persistent_obstacle = false;
            while (!wanted_puck_found) {
                // advance until puck found or end of the row is reached
                ir_b_resume();
                move_forward_nb();
                while (!puck_found) {
                    // obstacle avoidance
                    is_persistent_obstacle = OBSTACLE_AVOIDANCE(us_get_front_dist, stop_nb, move_forward_nb, 3);
                    // float front_dist = us_get_front_dist();
                    // if (front_dist < OBSTACLE_DIST_THRESH) {
                    //     stop_nb();
                    //     uint8 num_wait = 0;
                    //     while ((us_get_front_dist() < OBSTACLE_DIST_THRESH) && num_wait < 3) {
                    //         num_wait++;
                    //         bt_printf("obstacle %.2f ahead! (num_wait=%u)\n", front_dist, num_wait);
                    //         CyDelay(5000);
                    //     }
                    //     move_forward_nb();
                    // }

                    // if end of the row reached, switch to the next row
                    if (linear_run > 125 || is_persistent_obstacle) {
                        change_row = true;
                        stop_nb();
                        bt_printf("changing rows: linear_run=%.2f, is_persistent_obstacle? %c\n", linear_run, is_persistent_obstacle ? 'Y' : 'N');
                        break;
                    }
                }
                ir_b_pause();
                bt_printf("linear_run = %.2f\n", linear_run);

                // sense color of detected puck
                while (puck_found) {
#ifdef OBSTACLE_AVOIDANCE
                    while(us_get_front_dist() < OBSTACLE_DIST_THRESH);
#endif
                    move_forward_by(DETECTOR_TO_COLOR_SENSOR);

                    ir_b_pause();
                    CyDelay(100);
                    Color c = color_sense();
                    if (color_sense() == puck_color) {
                        puck_found = false;
                        wanted_puck_found = true;
                        break;
                    }

                    // let the undesired puck pass first and check for the next puck
                    if (c != INVALID) {
                        move_forward_by(PUCK_DIAMETER / 2 + 1);
                        ir_b_resume();
                        CyDelay(100);
                        puck_found = ir_b_is_detected();
                    }
                    else
                        puck_found = false;
                }
                ir_b_pause();

                // change to next row of pucks if all pucks on the line has been detected
                if (change_row) {
                    change_row = false;

                    // alignment
                    if (is_persistent_obstacle) {
                        move_backward_by(18);
                    }
                    else {
                        move_forward_nb();
                        while (us_get_front_dist() > 25) {
                            OBSTACLE_AVOIDANCE(us_get_front_dist, stop_nb, move_forward_nb, 2);
                        }
                        stop_nb();
                        rotate_to_align();
                    }

                    if ((base == BASE_LEFT && puck_row % 2 == 0) || (base == BASE_RIGHT && puck_row % 2 == 1)) {
                        REFRESH_OBSTACLE_AVOIDANCE(us_r_get_dist, nop, nop, 2);
                        turn_right();

                        REFRESH_OBSTACLE_AVOIDANCE(us_get_front_dist, nop, nop, 2);
                        move_forward_by(ROW_GAP * 1.0);

                        REFRESH_OBSTACLE_AVOIDANCE(us_r_get_dist, nop, nop, 2);
                        turn_right();

                        REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 2);
                        reverse_to_align();
#ifdef ALIGN_UPDATE_POS
                        bt_printf("pos_x: %.2f -> %.2f\n", pos_x, (base == BASE_LEFT) ? -MAX_X_2 : -MAX_X_1);
                        pos_x = (base == BASE_LEFT) ? -MAX_X_2 : -MAX_X_1;
                        bt_printf("new (%.2f)\n", pos_x);
#endif
                    }
                    else {
                        REFRESH_OBSTACLE_AVOIDANCE(us_l_get_dist, nop, nop, 2);
                        turn_left();

                        REFRESH_OBSTACLE_AVOIDANCE(us_get_front_dist, nop, nop, 2);
                        move_forward_by(ROW_GAP * 1.0);

                        REFRESH_OBSTACLE_AVOIDANCE(us_l_get_dist, nop, nop, 2);
                        turn_left();

                        REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 2);
                        reverse_to_align();
#ifdef ALIGN_UPDATE_POS
                        bt_printf("pos_x: %.2f -> %.2f\n", pos_x, (base == BASE_LEFT) ? MAX_X_1 : MAX_X_2);
                        pos_x = (base == BASE_LEFT) ? MAX_X_1 : MAX_X_2;
#endif
                    }

                    // if we take the middle row first
                    // if (base == BASE_LEFT) {
                    //     turn_right();
                    //     move_forward_by(ROW_GAP * 0.6);
                    //     turn_right();
                    //     reverse_to_align();
                    // }
                    // else {
                    //     turn_left();
                    //     move_forward_by(ROW_GAP * 0.6);
                    //     turn_left();
                    //     reverse_to_align();
                    // }

                    puck_row++;
                    linear_run = 0;
                }
            }
            track_linear_run = false;

            // clear nearby puck just in case
            // move_backward_by(COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER);
            REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 3);
            move_backward_by(10.5);

            // flick away nearby puck
            // ir_f_resume();
            // CyDelay(200);
            // if (ir_f_is_detected() || true)
            //     flicker_shoot();
            // ir_f_pause();

            // take puck
            // move_backward_by(COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER));
            // move_backward_by(6.9);
            REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 3);
            move_backward_by(10);

            gripper_open();
            lifter_down();

            REFRESH_OBSTACLE_AVOIDANCE(us_get_front_dist, nop, nop, 3);
            move_forward_by(5);

            gripper_hold_closed();
            CyDelay(800);
            lifter_up();
            gripper_unactuate();

            // alignment with side wall (left base has shortcut path back so unneeded)
            if (base == BASE_RIGHT) {
                if (puck_row % 2 == 1) {
                    REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 2);
                    reverse_to_align();
                }
                else {
                    move_forward_slow_nb();
                    while (us_get_front_dist() > 30) {
                        OBSTACLE_AVOIDANCE(us_get_front_dist, stop_nb, move_forward_slow_nb, 3);
                    }
                    stop_nb();
                    rotate_to_align();

                    // end with robot back facing side wall
                    REFRESH_OBSTACLE_AVOIDANCE(us_r_get_dist, nop, nop, 2);
                    turn_right();
                    REFRESH_OBSTACLE_AVOIDANCE(us_r_get_dist, nop, nop, 2);
                    turn_right();
                    REFRESH_OBSTACLE_AVOIDANCE(us_b_get_dist, nop, nop, 2);
                    reverse_to_align();
                }
#ifdef ALIGN_UPDATE_POS
                bt_printf("pos_x: %.2f -> %.2f\n", pos_x, MAX_X_2);
                pos_x = MAX_X_2;
#endif
            }
            // if (base == BASE_LEFT) {
            //     if (puck_row % 2  == 1) {
            //         reverse_to_align();
            //         move_forward_by(15);
            //         turn_left();
            //     }
            // }
            // else {
            //     if (puck_row % 2 == 1)
            //         reverse_to_align();
            //     else {
            //         move_forward_slow_nb();
            //         while (us_get_front_dist() > 30);
            //         stop_nb();
            //         rotate_to_align();

            //         // end with robot back facing side wall
            //         turn_right(); turn_right(); reverse_to_align();
            //     }
            // }

            state = PUCK_ZONE_TO_PINDECK;
        } END_STATE

        STATE(PUCK_ZONE_TO_PINDECK) {
            const float DIST_TO_BACK_WALL = 25;

            if (base == BASE_LEFT) {
                // const float DIST_TO_SIDE_WALL = 50;
                // const float err_tol = 3;

                // go back to base first
                if (puck_row % 2 == 1)
                    REFRESH_OBSTACLE_AVOIDANCE(us_l_get_dist, nop, nop, 3);
                else
                    REFRESH_OBSTACLE_AVOIDANCE(us_r_get_dist, nop, nop, 3);
                unwind_shortcut_back_to_base();

                state = BASE_TO_PINDECK;
            }
            else {
                // turn to have the back facing the back wall
                move_forward_by(12);
                turn_right();
                move_backward_by(40);

                // adjust distance of robot to the back wall behind and turn s.t. pindeck on the left
                move_backward_slow_nb();
                while (us_b_get_dist() > DIST_TO_BACK_WALL);
                stop_nb();
                turn_left();
                reverse_to_align();
#ifdef ALIGN_UPDATE_POS
                bt_printf("pos_x: %.2f -> %.2f\n", pos_x, MAX_X_2);
                pos_x = MAX_X_2;
#endif
                state = FIND_PIN;
            }
        } END_STATE

        STATE(SHOOT_PUCK) {
            // place down puck
            flicker_up();
            flicker_up();
            move_backward_by(US_TO_GRIPPER);
            lifter_down();
            gripper_open();
            move_backward_by(3);
            lifter_up();
            gripper_close();


            // if (base == BASE_RIGHT && zone_number == 1)
            //     move_forward_by(GRIPPER_TO_FLICKER + 3 - 2);
            // else
            //     move_forward_by(GRIPPER_TO_FLICKER + 3);

            // move s.t. puck is close to the ir sensor
            // move_forward_by(7);
            // CyDelay(1000);
            // while (!puck_found) {
            //     ir_f_resume();
            //     CyDelay(200);
            //     move_forward_nb(18);
            //     while (!puck_found && current_linear_movement != STOP);
            //     stop_nb();
            //     ir_f_pause();

            //     if (!puck_found)
            //         move_backward_by(18);
            // }
            // puck_found = false;
            ir_f_resume();
            CyDelay(200);
            bool puck_found_1 = false;
            uint8 my_counts = 0;
            while (!puck_found_1) {
                my_counts++;
                move_forward_by(3.0);
                CyDelay(800);
                puck_found_1 = ir_f_is_detected();

                if (my_counts == 5) {
                    move_backward_by(13.5);
                    my_counts = 0;
                }
            }

            CyDelay(1500);
            if (base == BASE_RIGHT && zone_number == 1)
                move_forward_by(1.2);
            else {
                flicker_shoot();
                move_forward_by(2.5);
            }
            flicker_shoot();

            state = RETURN_TO_BASE;
        } END_STATE

        STATE(RETURN_TO_BASE) {
            // unwind_navstack_till(1);
            // print_navstack();
            unwind_shortcut_back_to_base();
            // unwind_shortcut_navstack_till(2);/
            // move_backward_nb();
            // while (us_get_front_dist() < 60);
            // stop_nb();
            // if (base == BASE_LEFT)
            //     turn_right();
            // else
            //     turn_left();
            // reverse_to_align();
            // clear_navstack();
            pos_y = 0.0;
            pos_x = 0.0;

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
