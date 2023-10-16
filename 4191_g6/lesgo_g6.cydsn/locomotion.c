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

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "utils.h"
#include "NavStack.h"
#include "locomotion.h"
#include "limit_sw.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "base_sw.h"
#include "motor_l_in1.h"
#include "motor_l_in2.h"
#include "motor_r_in1.h"
#include "motor_r_in2.h"
#include "motor_l_pwm.h"
#include "motor_r_pwm.h"
#include "motor_l_quaddec.h"
#include "motor_r_quaddec.h"
#include "controller_isr.h"
#include "controller_timer.h"
#include "controller_reset.h"

#define MY_DEBUG
// #define DEBUG_CONTROLLER


// globals
volatile LinearMovement current_linear_movement = STOP;
volatile Movement latest_movement = { .type = NO_MOVEMENT, .counts = 0 };
volatile Heading heading = POS_Y;
volatile float pos_x = 0.0;
volatile float pos_y = 0.0;


// static globals
static volatile uint32 target_val = 0;
static volatile bool controller_timer_flag = false;
static uint16 current_master_speed = 12500;
static bool push_movement_to_ns = true;

static uint32 prev_slave_val = 0;
static uint32 prev_master_val = 0;
static volatile int16 integral = 0;
static volatile int16 prev_err = 0;
static float k_p = 0.0;
static float k_i = 0.0;
static float k_d = 0.0;


// constants
#define COUNTS_PER_WHEEL_CYCLE (14600)
#define WHEEL_GAP_CM (22.7)
#define WHEEL_DIAMETER_CM (5.45)
#define TURN_OFFSET (-1000)
static const uint32 COUNTS_PER_CM = (uint32)((COUNTS_PER_WHEEL_CYCLE / WHEEL_DIAMETER_CM / 3.14159265358979323846) + 0.5);
static const uint32 TURN_COUNT = (uint32)((COUNTS_PER_WHEEL_CYCLE / 4 / WHEEL_DIAMETER_CM *  WHEEL_GAP_CM) + 0.5 + TURN_OFFSET);
const char* HEADING_NAMES[4] = { "POS_Y", "NEG_Y", "POS_X", "NEG_X" };


// parameters (TODO: tune these values)
static const uint16 MASTER_SLOW_SPEED = 12500;
static const uint16 MASTER_BASE_SPEED = 18000;
// static const float K_P = 0.5;
// static const float K_I = 0.3;
// static const float K_D = 0.2;
float K_P = 1.0;
float K_I = 0.8;
float K_D = 0.4;
float K_P_slow = 1.0;
float K_I_slow = 0.8;
float K_D_slow = 0.4;


// internals
typedef enum __attribute__ ((__packed__)) WheelDir {
    WHEEL_FORWARD,
    WHEEL_REVERSE
} WheelDir;

bool controller_update(void);
void setup_controller(uint32 target);
void setup_controller_slow(uint32 target);
void set_wheeldir_l(WheelDir dir);
void set_wheeldir_r(WheelDir dir);
void set_wheeldir(WheelDir dir_l, WheelDir dir_r);
void wait_for_controller_period(void);
void update_pos_heading(Movement m, float* pos_x, float* pos_y, Heading* heading);


// ISRs
CY_ISR(controller_update_isr) {
    // clear timer interrupt flag
    controller_timer_ReadStatusRegister();

    if (controller_update()) return;

    // stop periodically running controller updates with the ISR
    controller_isr_Stop();
}

CY_ISR(set_controller_flag_isr) {
    // clear timer interrupt flag
    controller_timer_ReadStatusRegister();
    controller_timer_flag = true;
    controller_isr_Stop();
}

CY_ISR(limit_sw_isr) {
    limit_sw_pause();   // prevent double stop()
    stop();
}


void locomotion_setup(void) {
    // initialize all related blocks
    motor_l_pwm_Start();
    motor_r_pwm_Start();
    motor_l_quaddec_Start();
    motor_r_quaddec_Start();
    controller_timer_Start();

    limit_sw_setup(&limit_sw_isr, &limit_sw_isr);
    limit_sw_pause();
    navstack_init();

    pos_x = 0;
    pos_y = 0;
    // HEADING_NAMES[POS_Y] = "POS_Y";
    // HEADING_NAMES[NEG_Y] = "NEG_Y";
    // HEADING_NAMES[POS_X] = "POS_X";
    // HEADING_NAMES[NEG_X] = "NEG_X";
}

void stop(void) {
    motor_l_pwm_WriteCompare(0);
    motor_r_pwm_WriteCompare(0);
    target_val = 0;
    current_linear_movement = STOP;
    if (latest_movement.type == GO_FORWARD || latest_movement.type == GO_BACKWARD)
        latest_movement.counts = (labs(motor_l_quaddec_GetCounter()) + labs(motor_r_quaddec_GetCounter())) / 2;

    update_pos_heading(latest_movement, &pos_x, &pos_y, &heading);
    if (push_movement_to_ns)
        navstack_push(latest_movement);

    latest_movement = (Movement){ .type=NO_MOVEMENT, .counts=0 };
}

void turn_left(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
    latest_movement.type = TURN_LEFT;

    setup_controller(TURN_COUNT);
    while (controller_update()) wait_for_controller_period();
}

void turn_right(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
    latest_movement.type = TURN_RIGHT;

    setup_controller(TURN_COUNT);
    while (controller_update()) wait_for_controller_period();
}

void move_forward_by(float dist_cm) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller((uint32)(dist_cm * COUNTS_PER_CM + 0.5));
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_backward_by(float dist_cm) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller((uint32)(dist_cm * COUNTS_PER_CM + 0.5));
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_linear_by(float dist_cm) {
    if (dist_cm > 0)
        move_forward_by(dist_cm);
    else if (dist_cm < 0);
        move_backward_by(-dist_cm);
}

void move_forward_by_counts(uint32 counts) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller(counts);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_backward_by_counts(uint32 counts) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller(counts);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_forward(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller(UINT32_MAX);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_backward(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller(UINT32_MAX);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_forward_slow(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller_slow(UINT32_MAX);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_backward_slow(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller_slow(UINT32_MAX);
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_forward_slow_by(float dist_cm) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller_slow((uint32)(dist_cm * COUNTS_PER_CM + 0.5));
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void move_backward_slow_by(float dist_cm) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller_slow((uint32)(dist_cm * COUNTS_PER_CM + 0.5));
    while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
}

void reverse_to_align(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;
    setup_controller(UINT32_MAX);

    // reverse full speed if none of the limit switches are pressed
    if (!(limit_sw_l_is_on() || limit_sw_r_is_on())) {
        limit_sw_resume();
        while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
        limit_sw_pause();
        // while (!(limit_sw_l_is_on() || limit_sw_r_is_on())) wait_for_controller_period();
        stop();
    }

    // turn till both limit switches are pressed
    if (limit_sw_l_is_on()) {
        set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
        motor_l_pwm_WriteCompare(2500);
        motor_r_pwm_WriteCompare(MASTER_SLOW_SPEED);
        while (!limit_sw_r_is_on());
    }
    else if (limit_sw_r_is_on()) {
        set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
        motor_l_pwm_WriteCompare(MASTER_SLOW_SPEED);
        motor_r_pwm_WriteCompare(2500);
        while (!limit_sw_l_is_on());
    }
    motor_l_pwm_WriteCompare(0);
    motor_r_pwm_WriteCompare(0);
}

void rotate_to_align(void) {
    const double FRONT_US_GAP_CM = 11.5;

    CyDelay(us_refresh());
    float fl_dist = us_fl_get_avg_dist();
    float fr_dist = us_fr_get_avg_dist();
    uint8 iter = 0;

#ifdef MY_DEBUG
    bt_printf("(%u) FL %.4f \t FR %.4f\n", iter, fl_dist, fr_dist);
#endif

    push_movement_to_ns = false;
    while (fabsf(fl_dist - fr_dist) > 1 && iter < 8) {

#ifdef MY_DEBUG
        bt_printf("(%u) FL %.4f \t FR %.4f\n", iter, fl_dist, fr_dist);
#endif
        float theta;
        if (fr_dist > fl_dist) {
            theta = atan2f(fr_dist-fl_dist, FRONT_US_GAP_CM);
            set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
        }
        else {
            theta = atan2f(fl_dist-fr_dist, FRONT_US_GAP_CM);
            set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
        }
        float count_f = theta * ((float)TURN_COUNT / 3.14159265358979323846 * 2);
        uint32 count = (uint32)roundf(count_f);
        setup_controller_slow(count);
        while (controller_update()) wait_for_controller_period();

        fl_dist = us_fl_get_avg_dist();
        fr_dist = us_fr_get_avg_dist();
        iter++;
    }
    push_movement_to_ns = true;
}

void rotate_to_align_l(void) {
    // turn left slightly before aligning
    const float theta_deg = 15;
    float count_f = (theta_deg / 90) * TURN_COUNT;
    uint32 count = (uint32)roundf(count_f);

    set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
    setup_controller(count);
    while (controller_update()) wait_for_controller_period();

    // fine tune
    rotate_to_align();
}

void rotate_to_align_r(void) {
    // turn right slightly before aligning
    const float theta_deg = 15;
    float count_f = (theta_deg / 90) * TURN_COUNT;
    uint32 count = (uint32)roundf(count_f);

    set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
    setup_controller(count);
    while (controller_update()) wait_for_controller_period();

    // fine tune
    rotate_to_align();
}

void clear_navstack(void) {
    navstack_clear();
}

void unwind_navstack_till(uint8 remaining) {
    // don't push the following movements to the navigation stack
    push_movement_to_ns = false;


    while (navstack_len() > remaining) {
        Movement m = navstack_pop();

        // try to merge with the prior movements to save time
        while (navstack_len() > remaining) {

            if (!try_merge_movements(&m, navstack_peek()))
                break;

            navstack_pop();
        }

        // reverse the movement
        switch (m.type) {
            case NO_MOVEMENT: break;
            case GO_FORWARD: move_backward_by_counts(m.counts); break;
            case GO_BACKWARD: move_forward_by_counts(m.counts); break;
            case TURN_LEFT: turn_right(); break;
            case TURN_RIGHT: turn_left(); break;
        }
    }

    // return to original value
    push_movement_to_ns = true;
}

void print_unwind(uint8 remaining) {
    uint8 num = 0;

    bt_printf("-- unwind result (%2u) --\n", num);
    while (navstack_len()-num > remaining) {
        Movement m = navstack_peek_till(num);
        num++;

        // try to merge with the prior movements to save time
        while (navstack_len()-num > remaining) {

            if (!try_merge_movements(&m, navstack_peek_till(num)))
                break;

            num++;
        }

        // reverse the movement
        switch (m.type) {
            case NO_MOVEMENT: break;
            case GO_FORWARD: m.type = GO_BACKWARD; break;
            case GO_BACKWARD: m.type = GO_FORWARD; break;
            case TURN_LEFT: m.type = TURN_RIGHT; break;
            case TURN_RIGHT: m.type = TURN_LEFT; break;
        }
        print_movement(m);
        bt_print("\n");
    }
    bt_print("------------------------\n");
}

void unwind_shortcut_navstack_till(uint8 remaining) {
    // don't push the following movements to the navigation stack
    push_movement_to_ns = false;

    // find the target position and heading
    float target_pos_x = 0.0;
    float target_pos_y = 0.0;
    Heading target_heading = POS_Y;
    for (uint8 i = 0; i < remaining; i++) {
        Movement m = navstack_get(i);
        update_pos_heading(m, &target_pos_x, &target_pos_y, &target_heading);
        print_movement(m);
        // bt_block_on("go", "-> target (%.2f, %.2f) %s\n", target_pos_x, target_pos_y, HEADING_NAMES[target_heading]);
    }
    // bt_printf("CURRENT: (%.2f, %.2f) %s\n", pos_x, pox_y, HEADING_NAMES[heading]);
    // bt_printf("TARGET : (%.2f, %.2f) %s\n", target_pos_x, target_pox_y, HEADING_NAMES[target_heading]);
    float diff_x = target_pos_x - pos_x;
    float diff_y = target_pos_y - pos_y;

    // move in x direction
    // - if turning is needed, always turn such that we reverse into the position
    switch (heading) {
        case POS_X: move_linear_by(diff_x); break;
        case NEG_X: move_linear_by(-diff_x); break;
        case POS_Y: {
            if (diff_x > 0) turn_left();
            else turn_right();
            move_backward_by(fabsf(diff_x));
            break;
        }
        case NEG_Y: {
            if (diff_x > 0) turn_right();
            else turn_left();
            move_backward_by(fabsf(diff_x));
            break;
        }
    }
    // bt_block_on("go", "completed x movement");

    // move in y direction:
    // - before moving, the robot can only have +x or -x heading
    // - if the target heading is +y or -y, turn the robot so that it has the same heading
    // - otherwise, turn such that we reverse into the position
    if (heading == POS_X) {
        if (target_heading == POS_Y) turn_left();
        else if (target_heading == NEG_Y) turn_right();
        else {
            if (diff_y > 0) turn_right();
            else turn_left();
        }
    }
    else  {
        if (target_heading == POS_Y) turn_right();
        else if (target_heading == NEG_Y) turn_left();
        else {
            if (diff_y > 0) turn_left();
            else turn_right();
        }
    }
    move_backward_by(fabsf(diff_y));
    // bt_block_on("go", "completed y movement");

    // final adjustment to heading if required
    // - before moving, the robot can only have +y or -y heading
    // bt_block_on("go", "%s -> %s", HEADING_NAMES[heading], HEADING_NAMES[target_heading]);
    if (target_heading == POS_X) {
        if (heading == POS_Y) turn_right();
        else turn_left();
    }
    else if (target_heading == NEG_X)  {
        if (heading == POS_Y) turn_left();
        else turn_right();
    }

    // clear movements on navstack that have been backtracked
    while (navstack_len() > remaining) navstack_pop();

    // return to original value
    push_movement_to_ns = true;
}

void stop_nb(void) {
    stop();
    controller_isr_Stop();
}

void turn_left_nb(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
    latest_movement.type = TURN_LEFT;

    setup_controller(TURN_COUNT);
    controller_isr_StartEx(&controller_update_isr);
}

void turn_right_nb(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
    latest_movement.type = TURN_RIGHT;

    setup_controller(TURN_COUNT);
    controller_isr_StartEx(&controller_update_isr);
}

void move_forward_by_nb(float dist_cm) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller((uint32)(dist_cm * COUNTS_PER_CM));
    controller_isr_StartEx(&controller_update_isr);
}

void move_backward_by_nb(float dist_cm) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller((uint32)(dist_cm * COUNTS_PER_CM));
    controller_isr_StartEx(&controller_update_isr);
}

void move_forward_nb(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller(UINT32_MAX);
    controller_isr_StartEx(&controller_update_isr);
}

void move_backward_nb(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller(UINT32_MAX);
    controller_isr_StartEx(&controller_update_isr);
}

void move_forward_slow_nb(void) {
    set_wheeldir(WHEEL_FORWARD, WHEEL_FORWARD);
    current_linear_movement = FORWARD;
    latest_movement.type = GO_FORWARD;

    setup_controller_slow(UINT32_MAX);
    controller_isr_StartEx(&controller_update_isr);

}

void move_backward_slow_nb(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;

    setup_controller_slow(UINT32_MAX);
    controller_isr_StartEx(&controller_update_isr);

}

float get_latest_movement_dist(void) {
    return latest_movement.counts / COUNTS_PER_CM;
}


// internals
bool controller_update(void) {
    int32 slave_val = labs(motor_l_quaddec_GetCounter());
    int32 master_val = labs(motor_r_quaddec_GetCounter());
    int32 slave_count = slave_val - prev_slave_val;
    int32 master_count = master_val - prev_master_val;


    // stop if achieved target
    if ((slave_val + master_val) >= 2 * target_val) {
        stop();
        return false;
    }

    // calculate slave control signal with controller
    int16 error = master_count - slave_count;
    int16 derivative = error - prev_err;
    integral += error;
    prev_err = error;
    int16 u = (int16)roundf(k_p * error + k_i * integral + k_d * derivative);
    uint16 slave_new_speed = (uint16)((int16)current_master_speed + u);

#ifdef DEBUG_CONTROLLER
    bt_printf("e=%d\t u=%d\tspeed=%u (master %u)\n", error, u, slave_new_speed, current_master_speed);
#endif

    motor_l_pwm_WriteCompare(slave_new_speed);
    motor_r_pwm_WriteCompare(current_master_speed);
    prev_slave_val = slave_val;
    prev_master_val = master_val;
    return true;
}

void setup_controller(uint32 target) {
    // set target count value and initial wheel speeds
    target_val = target;
    current_master_speed = MASTER_BASE_SPEED;
    motor_l_quaddec_SetCounter(0);
    motor_r_quaddec_SetCounter(0);

    prev_slave_val = 0;
    prev_master_val = 0;
    integral = 0;
    prev_err = 0;

    k_p = K_P, k_i = K_I, k_d = K_D;
}

void setup_controller_slow(uint32 target) {
    // set target count value and initial wheel speeds
    target_val = target;
    current_master_speed = MASTER_SLOW_SPEED;
    motor_l_quaddec_SetCounter(0);
    motor_r_quaddec_SetCounter(0);

    prev_slave_val = 0;
    prev_master_val = 0;
    integral = 0;
    prev_err = 0;

    k_p = K_P_slow, k_i = K_I_slow, k_d = K_D_slow;
}

void set_wheeldir_l(WheelDir dir) {
    switch (dir) {
        case WHEEL_FORWARD: {
            motor_l_in1_Write(0);
            motor_l_in2_Write(1);
            break;
        }
        case WHEEL_REVERSE: {
            motor_l_in1_Write(1);
            motor_l_in2_Write(0);
            break;
        }
    }
}

void set_wheeldir_r(WheelDir dir) {
    switch (dir) {
        case WHEEL_FORWARD: {
            motor_r_in1_Write(0);
            motor_r_in2_Write(1);
            break;
        }
        case WHEEL_REVERSE: {
            motor_r_in1_Write(1);
            motor_r_in2_Write(0);
            break;
        }
    }
}

void set_wheeldir(WheelDir dir_l, WheelDir dir_r) {
    set_wheeldir_l(dir_l);
    set_wheeldir_r(dir_r);
}

void wait_for_controller_period(void) {
    controller_timer_flag = false;

    controller_reset_Write(1);
    controller_isr_StartEx(&set_controller_flag_isr);
    controller_reset_Write(0);
    while (!controller_timer_flag);
}

void update_pos_heading(Movement m, float* pos_x, float* pos_y, Heading* heading) {
    switch (m.type) {
        case NO_MOVEMENT: break;
        case GO_FORWARD: {
            float dist = m.counts / (float)COUNTS_PER_CM;
            switch (*heading) {
                case POS_Y: *pos_y += dist; break;
                case NEG_Y: *pos_y -= dist; break;
                case POS_X: *pos_x += dist; break;
                case NEG_X: *pos_x -= dist; break;
            }
            break;
        }
        case GO_BACKWARD: {
            float dist = m.counts / (float)COUNTS_PER_CM;
            switch (*heading) {
                case POS_Y: *pos_y -= dist; break;
                case NEG_Y: *pos_y += dist; break;
                case POS_X: *pos_x -= dist; break;
                case NEG_X: *pos_x += dist; break;
            }
            break;
        }
        case TURN_LEFT: {
            switch (*heading) {
                case POS_Y: *heading = NEG_X; break;
                case NEG_Y: *heading = POS_X; break;
                case POS_X: *heading = POS_Y; break;
                case NEG_X: *heading = NEG_Y; break;
            }
            break;
        }
        case TURN_RIGHT: {
            switch (*heading) {
                case POS_Y: *heading = POS_X; break;
                case NEG_Y: *heading = NEG_X; break;
                case POS_X: *heading = NEG_Y; break;
                case NEG_X: *heading = POS_Y; break;
            }
            break;
        }
    }
}


#undef COUNTS_PER_WHEEL_CYCLE
#undef WHEEL_GAP_CM
#undef WHEEL_DIAMETER_CM
#undef MY_DEBUG
#undef DEBUG_CONTROLLER
/* [] END OF FILE */
