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
    motor_*_pwm         : PWM blocks used to control motor speeds
    motor_*_quaddec     : QuadDec blocks used to count the shaft encoder ticks
    p_controller_timer  : timer block used to periodically run the P controller

    Pin Connections:
        - motor_l_en        (out)   -> ENA
        - motor_r_en        (out)   -> ENB
        - motor_l_in1       (out)   -> IN1
        - motor_l_in2       (out)   -> IN2
        - motor_r_in1       (out)   -> IN3
        - motor_r_in2       (out)   -> IN4
        - motor_l_phaseA    (out)   -> Left Pin 5
        - motor_l_phaseB    (out)   -> Left Pin 6
        - motor_r_phaseA    (out)   -> Right Pin 5
        - motor_r_phaseB    (out)   -> Right Pin 6
*/


#ifndef LOCOMOTION_H
#define LOCOMOTION_H


#include "cytypes.h"


// typedefs
typedef enum __attribute__ ((__packed__)) LinearMovement {
    STOP,
    FRONT,
    BACK
} LinearMovement;


// globals
extern LinearMovement current_linear_movement;


// ISRs
CY_ISR_PROTO(controller_update);


// API
void setup_locomotion(void);
void stop(void);
void turn_left(void);
void turn_right(void);
void move_forward_by(uint8 dist_cm);
void move_backward_by(uint8 dist_cm);


#endif  // LOCOMOTION_H

/* [] END OF FILE */
