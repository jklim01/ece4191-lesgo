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
#include <string.h>

#include "utils.h"
#include "servo.h"
#include "ir_sensor.h"
#include "ultrasonic.h"
#include "limit_sw.h"

#define SPECIAL_DEBUG
#include "bluetooth.h"

#define MY_DEBUG
#include "locomotion.h"
#include "color_sensor.h"

#define CONTROL(x) case (control_##x): { bt_printf("\n----- %-15s -----", #x); bt_clear_rx_finished();
#define END_CONTROL control = control_idle; }


// - specify the command name inside `X(...)` (it's called an X macro)
// - every X macro line except the last must end with "\"
// - write the code for the control inside this: `CONTROL(front) {...} END_CONTROL`, it will auto print the current control
//      for example:
//          if I send "front 20.3" from the phone
//          20.3 will be saved inside `arg2`, and we will run the code inside `CONTROL(front) {...} END_CONTROL` and go back `idle` to wait for the next command
//
// - some distance constants are defined inside `utils.h`, and you can tune the values
// - you can optionally pass one number into the command which will saved into the variable `float arg2` (defaults to 0 if not specified)
// - can use `bt_printf` and `bt_print` for logging (always end the printed string with "\n" or "  " (2 spaces), otherwise it won't appear on the phone)
// - can accept user input using `bt_scanf`
// - can also use `bt_scanf` to check if the user sent some message, for example: `bt_scanf("lol") == 1` checks if the user sent "lol"
//
// - every line that is sent through the phone must end with "!" (as shown below)
#define LIST_OF_CONTROLS    \
    X(idle)             /* idle!                                    */ \
    X(front)            /* front %f!                                */ \
    X(back)             /* back %f!                                 */ \
    X(left)             /* left!                                    */ \
    X(right)            /* right!                                   */ \
    X(navstackLen)      /* navstackLen!                             : prints navstack len   */ \
    X(unwind)           /* unwind %d!                               : unwinds to target len */ \
    X(flickP)           /* flickP!                                  : puts down and flick puck  */ \
    X(searchP)          /* seachP [%d]!                             : move forward to find puck for specified distance (default 50cm if not specified), and grip puck if found  */ \
    X(revA)             /* revA!                                    : reversal-based align  */ \
    X(rotA)             /* rotA!                                    : rotation-based align  */ \
    X(usL)              /* getUsL!                                  : print all ultrasonic readings except right until any message is sent  */ \
    X(usR)              /* getUsR!                                  : print all ultrasonic readings except left until any message is sen*/ \
    X(detectB)          /* detectB!                                 : move forward 15 times or until bowling pin detected (must start with ultrasonic facing wall) */

#define X(name) control_##name,
typedef enum Control {
    LIST_OF_CONTROLS
} Control;
#undef X


static bool puck_found = false;
CY_ISR(ir_handler) {
    puck_found = true;
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
    ir_sensor_setup(&ir_handler);
    bt_setup();

    // variables
    int arg_num = 0;
    char arg1[50];
    char arg2_str[50];
    float arg2;
    Control control = control_idle;
    FSM(control) {
        CONTROL(idle) {
            arg1[0] = 0;
            arg2_str[0] = 0;
            arg2 = 0;
            arg_num = bt_scanf("%49s %49s", arg1, arg2_str);

            if (arg_num == 2)
                arg2 = atof(arg2_str);

            // change to chosen control
#define X(name) \
            if (strcmp(arg1, #name) == 0) { \
                control = control_##name;   \
                continue;   \
            }
LIST_OF_CONTROLS
#undef X

        } END_CONTROL

        CONTROL(front) {
            move_forward_by(arg2);
        } END_CONTROL

        CONTROL(back) {
            move_backward_by(arg2);
        } END_CONTROL

        CONTROL(left) {
            turn_left();
        } END_CONTROL

        CONTROL(right) {
            turn_right();
        } END_CONTROL

        CONTROL(navstackLen) {
            bt_printf("target_len = %u\n", navstack_len);
        } END_CONTROL

        CONTROL(unwind) {
            unwind_navstack_till((uint8)arg2);
        } END_CONTROL

        CONTROL(searchP) {
            if (arg2 == 0)
                arg2 = 50;

            ir_sensor_resume();
            move_forward_nb();
            while (!puck_found) {
                if (get_latest_movement_dist() >= arg2) {
                    stop_nb();
                    break;
                }
            }
            ir_sensor_pause();

            if (puck_found) {
                // always flick to clear nearby puck just in case even though might not be there (easier to always flick)
                float input;
                bt_printf("\nmove_backward_by (flick nearby puck, default %.2f)?  ", COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER);
                if (bt_scanf("%f", &input) == 0)
                    // if nothing was successfully scanned (i.e. "!" was sent), use default value
                    move_backward_by(COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER);
                else
                    move_backward_by(input);

                flicker_shoot();


                // take puck
                bt_printf("\nmove_backward_by (take puck, default %.2f)?  ", COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER));
                if (bt_scanf("%f", &input) == 0)
                    move_backward_by(COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_SLIT + PUCK_DIAMETER));
                else
                    move_backward_by(input);

                gripper_open();
                lifter_down();
                gripper_hold_closed();
                CyDelay(800);
                lifter_up();
                gripper_unactuate();
            }
        }

        CONTROL(flickP) {
            // place down puck
            float input;
            bt_printf("\nmove_backward_by (place puck, default %.2f)?  ", US_TO_GRIPPER);
            if (bt_scanf("%f", &input) == 0)
                move_backward_by(US_TO_GRIPPER);
            else
                move_backward_by(input);

            lifter_down();
            gripper_open();
            move_backward_by(3);
            lifter_up();
            gripper_close();

            // align puck with flicker and flick
            move_forward_by(GRIPPER_TO_FLICKER + 3);
            flicker_shoot();

        } END_CONTROL

        CONTROL(revA) {
            reverse_to_align();
        } END_CONTROL

        CONTROL(rotA) {
            rotate_to_align();
        } END_CONTROL

        CONTROL(usL) {
            while (!bt_rx_finished()) {
                bt_printf("L %5.2f \t B %5.2f \t FL %5.2f \t FR %5.2f\n",
                    us_l_get_dist(), us_b_get_dist(), us_fl_get_dist(), us_fr_get_dist());
            }
        } END_CONTROL

        CONTROL(usR) {
            while (!bt_rx_finished()) {
                bt_printf("R %5.2f \t B %5.2f \t FL %5.2f \t FR %5.2f\n",
                    us_r_get_dist(), us_b_get_dist(), us_fl_get_dist(), us_fr_get_dist());
            }
        } END_CONTROL

        CONTROL(detectB) {
            CyDelay(500);
            float dist_to_wall = us_l_get_avg_dist();
            bt_printf("dist_to_wall = %.4f\n", dist_to_wall);

            bool pin_found = false;
            uint8 move_counts = 0;
            while (!pin_found && move_counts < 15) {
                move_forward_by(2.5);
                move_counts++;

                CyDelay(500);
                float meas = us_l_get_dist();
                pin_found = us_l_get_dist() < dist_to_wall - PIN_DIST_TOL;
                bt_printf("%5.2f %s\n", meas, pin_found ? "FOUND!" : "X");
            }
        } END_CONTROL


    } END_FSM

    return 0;
}

/* [] END OF FILE */



/* [] END OF FILE */
