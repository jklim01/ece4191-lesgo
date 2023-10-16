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

#include "bt_debug.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "cytypes.h"
#include "CyLib.h"

#include "utils.h"
#include "servo.h"
#include "ir_sensor.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "locomotion.h"
#include "color_sensor.h"


#define CONTROL(x) case (control_##x): { bt_printf("\n\n- - - - - - - - - - %-15s - - - - - - - - - -\n", #x); bt_clear_rx_finished();
#define END_CONTROL control = control_idle; break; }


// - specify the command name inside `X(...)` (it's called an X macro)
// - every X macro line except the last must end with "\"
// - write the code for the control inside this: `CONTROL(front) {...} END_CONTROL`, it will auto print the current control (compile error if not done properly)
// - you can optionally pass one number into the command which will saved into the variable `float arg2` (defaults to 0 if not specified)
//      for example:
//          if I send "front 20.3" from the phone
//          20.3 will be saved inside `arg2`, and we will run the code inside `CONTROL(front) {...} END_CONTROL` and go back  to `idle` to wait for the next command
//
// - some distance constants are defined inside `utils.h`, and you can tune the values
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
    X(brk)              /* brk!                                     : break out of debug mode   */ \
    X(lenN)             /* lenN!                                    : prints navstack len   */ \
    X(printN)           /* printN!                                  : prints navstack   */ \
    X(unwind)           /* unwind %d!                               : unwinds to the target */ \
    X(printU)           /* printU %d!                               : prints how navstack will be unwinded to the target len    */ \
    X(unwindS)          /* unwindS %d!                              : unwinds to the target len with shortcut */ \
    X(flickP)           /* flickP!                                  : puts down and flick puck  */ \
    X(searchP)          /* searchP [%d]!                            : move forward to find puck for specified distance (default 50cm if not specified), and grip puck if found  */ \
    X(revA)             /* revA!                                    : reversal-based align  */ \
    X(rotA)             /* rotA!                                    : rotation-based align  */ \
    X(usL)              /* getUsL!                                  : print all ultrasonic readings except right until any message is sent  */ \
    X(usR)              /* getUsR!                                  : print all ultrasonic readings except left until any message is sent   */ \
    X(detectB)          /* detectB!                                 : move forward 15 times or until bowling pin detected (must start with ultrasonic facing wall)  */  \
    X(gripperC)         /* gripperC!                                : gripper close */  \
    X(gripperO)         /* gripperO!                                : gripper open  */  \
    X(lifterU)          /* lifterU!                                 : lifter up */  \
    X(lifterD)          /* lifterD!                                 : lifter down   */  \
    X(flickerU)         /* flickerU!                                : flicker up    */  \
    X(flickerD)         /* flickerD!                                : flicker down   */

#define X(name) control_##name,
typedef enum Control {
    LIST_OF_CONTROLS
} Control;
#undef X


// globals
bool bt_dbg_puck_found = false;


// ISRs
CY_ISR(bt_dbg_ir_handler) {
    bt_dbg_puck_found = true;
    stop();
}


int bt_dbg(void) {
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

            if (arg_num == 0)
                continue;
            // if (arg_num == 1)
            //     bt_printf("%s", arg1);
            if (arg_num == 2) {
                arg2 = atof(arg2_str);
                // bt_printf("%s %.2f", arg1, arg2);
            }

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

        CONTROL(brk) {
            return 0;
        } END_CONTROL

        CONTROL(lenN) {
            bt_printf("navstack_len() = %u\n", navstack_len());
        } END_CONTROL

        CONTROL(printN) {
            print_navstack();
        } END_CONTROL

        CONTROL(unwind) {
            unwind_navstack_till((uint8)arg2);
        } END_CONTROL

        CONTROL(printU) {
            print_unwind((uint8)arg2);
        } END_CONTROL

        CONTROL(unwindS) {
            unwind_shortcut_navstack_till((uint8)arg2);
        } END_CONTROL

        CONTROL(searchP) {
            if (arg2 == 0)
                arg2 = 50;

            ir_sensor_resume();
            move_forward_nb();
            while (!bt_dbg_puck_found) {
                if (get_latest_movement_dist() >= arg2) {
                    stop_nb();
                    break;
                }
            }
            ir_sensor_pause();

            if (bt_dbg_puck_found) {
                bt_dbg_puck_found = false;

                // always flick to clear nearby puck just in case even though might not be there (easier to always flick)
                float input;
                bt_printf("\nmove_backward_by (flick nearby puck, default %.2f)?  ", 11 - DETECTOR_TO_COLOR_SENSOR + PUCK_DIAMETER);
                if (bt_scanf("%f", &input) == 0)
                    // if nothing was successfully scanned (i.e. "!" was sent), use default value
                    move_backward_by(COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER);
                else {
                    bt_printf("moving by %.2f...\n", input);
                    move_backward_by(input);
                }

                flicker_shoot();


                // take puck
                bt_printf("\nmove_backward_by (take puck, default %.2f)?  ", COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER));
                if (bt_scanf("%f", &input) == 0)
                    move_backward_by(COLOR_SENSOR_TO_GRIPPER - (COLOR_SENSOR_TO_FLICKER + PUCK_DIAMETER));
                else {
                    bt_printf("moving by %.2f...\n", input);
                    move_backward_by(input);
                }

                gripper_open();
                lifter_down();
                gripper_hold_closed();
                CyDelay(800);
                lifter_up();
                gripper_unactuate();
            }
        } END_CONTROL

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
            bt_printf("\nmove_forward_by (align puck, default %.2f)?  ", GRIPPER_TO_FLICKER + 3);
            if (bt_scanf("%f", &input) == 0)
                move_forward_by(GRIPPER_TO_FLICKER + 3);
            else
                move_forward_by(input);
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
                CyDelay(us_refresh());
            }
        } END_CONTROL

        CONTROL(usR) {
            while (!bt_rx_finished()) {
                bt_printf("R %5.2f \t B %5.2f \t FL %5.2f \t FR %5.2f\n",
                    us_r_get_dist(), us_b_get_dist(), us_fl_get_dist(), us_fr_get_dist());
                CyDelay(us_refresh());
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

        CONTROL(gripperC) {
            gripper_close();
        } END_CONTROL

        CONTROL(gripperO) {
            gripper_open();
        } END_CONTROL

        CONTROL(lifterU) {
            lifter_up();
        } END_CONTROL

        CONTROL(lifterD) {
            lifter_down();
        } END_CONTROL

        CONTROL(flickerU) {
            flicker_up();
        } END_CONTROL

        CONTROL(flickerD) {
            flicker_down();
        } END_CONTROL

    } END_FSM

    return 0;
}


#undef CONTROL
#undef END_CONTROL
#undef LIST_OF_CONTROL

/* [] END OF FILE */
