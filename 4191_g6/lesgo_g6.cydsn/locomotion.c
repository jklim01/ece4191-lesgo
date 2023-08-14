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

#include "locomotion.h"
#include "motor_l_in1.h"
#include "motor_l_in2.h"
#include "motor_r_in1.h"
#include "motor_r_in2.h"
#include "motor_l_pwm.h"
#include "motor_r_pwm.h"
#include "motor_l_quaddec.h"
#include "motor_r_quaddec.h"
#include "p_controller_timer.h"
#include "p_controller_isr.h"


// TODO: tune these values
static const float K_P = 0.05;
static const uint16 MASTER_BASE_SPEED = 12500;
static const uint16 TURN_COUNT = 12500;
static const uint16 DIST_CM_TO_COUNT = 300;

static volatile uint16 target_count = 0;
LinearMovement current_linear_movement = STOP;


// internals
typedef enum __attribute__ ((__packed__)) WheelDir {
    FORWARD,
    REVERSE
} WheelDir;

void activate_controller(uint16 target);
void deactivate_controller(void);
void set_wheeldir_l(WheelDir dir);
void set_wheeldir_r(WheelDir dir);
void set_wheeldir(WheelDir dir_l, WheelDir dir_r);


CY_ISR(controller_update) {
    // clear timer interrupt flag
    p_controller_timer_ReadStatusRegister();

    // read shaft encoder counts for both wheels
    int32 slave_count = labs(motor_l_quaddec_GetCounter());
    int32 master_count = labs(motor_r_quaddec_GetCounter());

    // stop if achieved target
    if (master_count >= target_count) {
        deactivate_controller();
        return;
    }

    // calculate slave control signal with P controller (+ 0.5 is used for rounding during cast)
    int32 error = master_count - slave_count;
    uint16 slave_new_speed = MASTER_BASE_SPEED + (uint16)(K_P * error + 0.5);

    motor_l_pwm_WriteCompare(slave_new_speed);
    motor_l_quaddec_Cnt16_WriteCounter(0);
    motor_r_quaddec_Cnt16_WriteCounter(0);
}


void setup_locomotion(void) {
    // initialize all related blocks
    motor_l_pwm_Start();
    motor_r_pwm_Start();
    motor_l_quaddec_Start();
    motor_r_quaddec_Start();
    p_controller_timer_Start();
}

void stop(void) {
    deactivate_controller();
}

void turn_left(void) {
    set_wheeldir(REVERSE, FORWARD);
    activate_controller(TURN_COUNT);
}

void turn_right(void) {
    set_wheeldir(FORWARD, REVERSE);
    activate_controller(TURN_COUNT);
}

void move_forward_by(uint8 dist_cm) {
    set_wheeldir(FORWARD, FORWARD);
    current_linear_movement = FRONT;
    activate_controller(dist_cm * DIST_CM_TO_COUNT);
}

void move_backward_by(uint8 dist_cm) {
    set_wheeldir(REVERSE, REVERSE);
    current_linear_movement = BACK;
    activate_controller(dist_cm * DIST_CM_TO_COUNT);
}


// internals
void activate_controller(uint16 target) {
    // set target count value and initial wheel speeds
    target_count = target;
    motor_l_pwm_WriteCompare(MASTER_BASE_SPEED);
    motor_r_pwm_WriteCompare(MASTER_BASE_SPEED);

    // enable ISR for P controller
    p_controller_isr_StartEx(controller_update);
}

void deactivate_controller(void) {
    // disable ISR for P controller
    p_controller_isr_Stop();

    // set wheel speeds to zero
    motor_l_pwm_WriteCompare(0);
    motor_r_pwm_WriteCompare(0);
}

void set_wheeldir_l(WheelDir dir) {
    switch (dir) {
        case FORWARD: {
            motor_l_in1_Write(1);
            motor_l_in2_Write(0);
            break;
        }
        case REVERSE: {
            motor_l_in1_Write(0);
            motor_l_in2_Write(1);
            break;
        }
    }
}

void set_wheeldir_r(WheelDir dir) {
    switch (dir) {
        case FORWARD: {
            motor_r_in1_Write(1);
            motor_r_in2_Write(0);
            break;
        }
        case REVERSE: {
            motor_r_in1_Write(0);
            motor_r_in2_Write(1);
            break;
        }
    }
}

void set_wheeldir(WheelDir dir_l, WheelDir dir_r) {
    set_wheeldir_l(dir_l);
    set_wheeldir_r(dir_r);
}

/* [] END OF FILE */
