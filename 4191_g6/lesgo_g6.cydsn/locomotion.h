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
    STOP=0,
    FORWARD=1,
    REVERSE=2
} LinearMovement;

typedef enum __attribute__ ((__packed__)) Heading {
    POS_Y=0,
    NEG_Y=1,
    POS_X=2,
    NEG_X=3
} Heading;


// globals
extern volatile LinearMovement current_linear_movement;
extern volatile Heading heading;
extern volatile float pos_x;
extern volatile float pos_y;
extern bool track_linear_run;
extern volatile float linear_run;


// constants
extern int16 FORWARD_SLAVE_OFFSET;
extern int16 BACKWARD_SLAVE_OFFSET;
extern int16 FORWARD_SLAVE_OFFSET_SLOW;
extern int16 BACKWARD_SLAVE_OFFSET_SLOW;
extern int16 FORWARD_SLAVE_OFFSET_DAMN_SLOW;
extern int16 BACKWARD_SLAVE_OFFSET_DAMN_SLOW;
extern int16 TURN_FORWARD_SLAVE_OFFSET;
extern int16 TURN_BACKWARD_SLAVE_OFFSET;
extern float K_P;
extern float K_I;
extern float K_D;
extern float K_P_slow;
extern float K_I_slow;
extern float K_D_slow;
extern const char* HEADING_NAMES[4];


// ISRs
CY_ISR_PROTO(controller_update_isr);
CY_ISR_PROTO(set_controller_flag_isr);
CY_ISR_PROTO(limit_sw_l_isr);
CY_ISR_PROTO(limit_sw_r_isr);


// API
void locomotion_setup(void);
void stop(void);
void turn_left(void);
void turn_right(void);
void move_forward_by(float dist_cm);
void move_backward_by(float dist_cm);
void move_linear_by(float dist_cm);
void move_forward_by_counts(uint32 counts);
void move_backward_by_counts(uint32 counts);

void move_forward(void);
void move_backward(void);
void move_forward_slow(void);
void move_backward_slow(void);
void move_forward_slow_by(float dist_cm);
void move_backward_slow_by(float dist_cm);
void move_forward_damn_slow(void);
void move_forward_damn_slow_by(float dist_cm);

void reverse_to_align(void);
void rotate_to_align(void);
void rotate_to_align_l(void);
void rotate_to_align_r(void);

void clear_navstack(void);
void unwind_navstack_till(uint8 remaining);
void print_unwind(uint8 remaining);
void unwind_shortcut_navstack_till(uint8 remaining);
float get_latest_movement_dist(void);

// non-blocking versions
void stop_nb(void);
void turn_left_nb(void);
void turn_right_nb(void);

void move_forward_by_nb(float dist_cm);
void move_backward_by_nb(float dist_cm);
void move_forward_nb(void);
void move_backward_nb(void);

void move_forward_slow_nb(void);
void move_forward_slow_by_nb(float dist_cm);
void move_backward_slow_nb(void);
void move_forward_damn_slow_nb(void);
void move_forward_damn_slow_by_nb(float dist_cm);
void move_forward_special_nb(void);
void move_forward_special_by_nb(float dist_cm);


#endif  // LOCOMOTION_H

/* [] END OF FILE */
