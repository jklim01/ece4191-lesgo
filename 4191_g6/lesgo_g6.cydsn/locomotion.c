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

#include "NavStack.h"
#include "locomotion.h"
#include "limit_sw.h"
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

#include "led_r.h"
#include "led_g.h"
#include "led_b.h"
#include "led.h"


// globals
volatile LinearMovement current_linear_movement = STOP;
volatile Movement latest_movement = { .type = NO_MOVEMENT, .counts = 0 };
volatile Heading heading = NORTH;
volatile float pos_x = 0.0;
volatile float pos_y = 0.0;


// static globals
static volatile uint32 target_count = 0;
static volatile bool controller_timer_flag = false;
static bool push_movement_to_ns = true;


// constants
#define COUNTS_PER_WHEEL_CYCLE (14600)
#define WHEEL_GAP_CM (22.7)
#define WHEEL_DIAMETER_CM (5.45)
static const uint32 COUNTS_PER_CM = (uint32)((COUNTS_PER_WHEEL_CYCLE / WHEEL_DIAMETER_CM / 3.14159265358979323846) + 0.5);
static const uint32 TURN_COUNT = (uint32)((COUNTS_PER_WHEEL_CYCLE / 4 / WHEEL_DIAMETER_CM *  WHEEL_GAP_CM) + 0.5);
static const uint16 controller_period_ms = 50;


// parameters (TODO: tune these values)
static const uint16 MASTER_BASE_SPEED = 12500;

#if CONTROLLER_TYPE == P
static const double K_P = 0.05;

#elif CONTROLLER_TYPE == PI
static const double K_P = 0.05;
static const double K_I = 0.05;

#elif CONTROLLER_TYPE == PD
static const double K_P = 0.05;
static const double K_D = 0.05;

#elif CONTROLLER_TYPE == PID
static const double K_P = 0.05;
static const double K_I = 0.05;
static const double K_D = 0.05;

#endif


// internals
typedef enum __attribute__ ((__packed__)) WheelDir {
    WHEEL_FORWARD,
    WHEEL_REVERSE
} WheelDir;

bool controller_update(void);
bool alignment_controller_update(void);
void setup_controller(uint32 target);
void set_wheeldir_l(WheelDir dir);
void set_wheeldir_r(WheelDir dir);
void set_wheeldir(WheelDir dir_l, WheelDir dir_r);
void wait_for_controller_period(void);
void update_pos_heading(Movement m);


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
    stop();
}


void locomotion_setup(void) {
    // initialize all related blocks
    motor_l_pwm_Start();
    motor_r_pwm_Start();
    motor_l_quaddec_Start();
    motor_r_quaddec_Start();
    controller_timer_Start();
    controller_timer_WritePeriod(controller_period_ms * 1000);  // 1MHz locomotion_clk -> 1000 ticks per ms

    navstack_init();
    if (base_sw_Read() == 0)
        pos_x = -2;
    else
        pos_y = 2;
}

void stop(void) {
    motor_l_pwm_WriteCompare(0);
    motor_r_pwm_WriteCompare(0);
    target_count = 0;
    current_linear_movement = STOP;

    update_pos_heading(latest_movement);
    if (push_movement_to_ns)
        navstack_push(latest_movement);
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
    latest_movement.type = GO_FORWARD;

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

void reverse_to_align(void) {
    set_wheeldir(WHEEL_REVERSE, WHEEL_REVERSE);
    current_linear_movement = REVERSE;
    latest_movement.type = GO_BACKWARD;
    led_r_Write(0);
    led_g_Write(0);
    led_b_Write(0);

    setup_controller(UINT32_MAX);
    limit_sw_setup(&limit_sw_isr, &limit_sw_isr);
    led_g_Write(1);
    // while (current_linear_movement != STOP && controller_update()) wait_for_controller_period();
    while (controller_update()) wait_for_controller_period();
    limit_sw_pause();
    led_r_Write(1);

    if (limit_sw_l_is_on()) {
        set_wheeldir(WHEEL_FORWARD, WHEEL_REVERSE);
        motor_l_pwm_WriteCompare(2500);
        motor_r_pwm_WriteCompare(MASTER_BASE_SPEED);
    }
    else if (limit_sw_r_is_on()) {
        set_wheeldir(WHEEL_REVERSE, WHEEL_FORWARD);
        motor_l_pwm_WriteCompare(MASTER_BASE_SPEED);
        motor_r_pwm_WriteCompare(2500);
    }
    while (!limit_sw_l_is_on() || !limit_sw_r_is_on());
    stop();

    led_r_Write(0);
    led_g_Write(0);
    led_b_Write(0);
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


// internals
bool controller_update(void) {
    int32 slave_count = labs(motor_l_quaddec_GetCounter());
    int32 master_count = labs(motor_r_quaddec_GetCounter());

    // stop if achieved target
    if (master_count >= target_count) {
        stop();
        return false;
    }

    // calculate slave control signal with controller
    int16 error = master_count - slave_count;
#if CONTROLLER_TYPE == P
    int16 u = (int16)(K_P * error);

#elif CONTROLLER_TYPE == PI
    static double integral = 0;
    integral += controller_period_ms * error;
    int16 u = (int16)(K_P * error + K_I * integral);

#elif CONTROLLER_TYPE == PD
    static double prev_err = 0;
    double derivative = (error - prev_err) / controller_period_ms;
    prev_err = error;
    int16 u = (int16)(K_P * error + K_D * derivative);

#elif CONTROLLER_TYPE == PID
    static double integral = 0;
    static double prev_err = 0;
    integral += controller_period_ms * error;
    double derivative = (error - prev_err) / controller_period_ms;
    prev_err = error;
    int16 u = (int16)(K_P * error + K_I * integral + K_D * derivative);

#endif
    uint16 slave_new_speed = (uint16)((int16)MASTER_BASE_SPEED + u);

    motor_l_pwm_WriteCompare(slave_new_speed);
    motor_r_pwm_WriteCompare(MASTER_BASE_SPEED);
    return true;
}

void setup_controller(uint32 target) {
    // set target count value and initial wheel speeds
    target_count = target;
    motor_l_quaddec_SetCounter(0);
    motor_r_quaddec_SetCounter(0);
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

void update_pos_heading(Movement m) {
    switch (m.type) {
        case GO_FORWARD: {
            m.counts = (motor_l_quaddec_GetCounter() + motor_r_quaddec_GetCounter()) / 2;
            float dist = m.counts / COUNTS_PER_CM;
            switch (heading) {
                case NORTH: pos_y += dist; break;
                case SOUTH: pos_y -= dist; break;
                case EAST: pos_x += dist; break;
                case WEST: pos_x -= dist; break;
            }
            break;
        }
        case GO_BACKWARD: {
            m.counts = (motor_l_quaddec_GetCounter() + motor_r_quaddec_GetCounter()) / 2;
            float dist = m.counts / COUNTS_PER_CM;
            switch (heading) {
                case NORTH: pos_y -= dist; break;
                case SOUTH: pos_y += dist; break;
                case EAST: pos_x -= dist; break;
                case WEST: pos_x += dist; break;
            }
            break;
        }
        case TURN_LEFT: {
            switch (heading) {
                case NORTH: heading = WEST; break;
                case SOUTH: heading = EAST; break;
                case EAST: heading = SOUTH; break;
                case WEST: heading = NORTH; break;
            }
            break;
        }
        case TURN_RIGHT: {
            switch (heading) {
                case NORTH: heading = EAST; break;
                case SOUTH: heading = WEST; break;
                case EAST: heading = NORTH; break;
                case WEST: heading = SOUTH; break;
            }
            break;
        }
    }
}


#undef COUNTS_PER_WHEEL_CYCLE
#undef WHEEL_GAP_CM
#undef WHEEL_DIAMETER_CM
/* [] END OF FILE */
