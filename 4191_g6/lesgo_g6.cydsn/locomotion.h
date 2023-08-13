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

/*
    motor_*_pwm: pwm blocks used to control motor speeds

    Pin Connections to H Bridge:
        - motor_l_en    (out)   -> ENA
        - motor_r_en    (out)   -> ENB
        - motor_l_in1   (out)   -> IN1
        - motor_l_in2   (out)   -> IN2
        - motor_r_in1   (out)   -> IN3
        - motor_r_in2   (out)   -> IN4

    ! Setup:
    motor_l_pwm_Start();
    motor_r_pwm_Start();
*/


#ifndef LOCOMOTION_H
#define LOCOMOTION_H


#include "MOTOR_L_PWM.h"
#include "MOTOR_R_PWM.h"
#include "Clock_1.h"
#include "motor_l_en_aliases.h"
#include "motor_l_en.h"
#include "motor_r_en_aliases.h"
#include "motor_r_en.h"
#include "motor_l_in1_aliases.h"
#include "motor_l_in1.h"
#include "motor_l_in2_aliases.h"
#include "motor_l_in2.h"
#include "motor_r_in1_aliases.h"
#include "motor_r_in1.h"
#include "motor_r_in2_aliases.h"
#include "motor_r_in2.h"


#define L_SPEED 200
#define R_SPEED 200
#define L_TURN_SPEED 200
#define R_TURN_SPEED 200
#define TURN_TIME 100
#define DIST_TO_TIME_MS 100

// typedef enum {
//     STOP,
//     TURN_LEFT,
//     TURN_RIGHT,
//     FORWARD,
//     REVERSE
// } Movement;

typedef enum __attribute__ ((__packed__)) {
    FORWARD,
    REVERSE
} Direction;


void set_speed_l(uint8 speed, Direction dir);
void set_speed_r(uint8 speed, Direction dir);
void set_speed(uint8 speed_l, Direction dir_l, uint8 speed_r, Direction dir_r);
// void set_movement(Movement movement);

void stop();
void turn_left();
void turn_right();
void move_forward_by(unsigned int distance);
void move_backward_by(unsigned int distance);


#endif  // LOCOMOTION_H

/* [] END OF FILE */
