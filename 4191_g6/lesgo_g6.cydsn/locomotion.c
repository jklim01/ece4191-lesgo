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

#include "locomotion.h"


void set_speed_l(uint8 speed, Direction dir) {
    motor_l_pwm_WriteCompare(speed);

    switch (dir) {
        case FORWARD: {
            motor_l_in1_Write(1);
            motor_l_in1_Write(0);
            break;
        }
        case REVERSE: {
            motor_l_in1_Write(0);
            motor_l_in1_Write(1);
            break;
        }
    }
}

void set_speed_r(uint8 speed, Direction dir) {
    motor_r_pwm_WriteCompare(speed);

    switch (dir) {
        case FORWARD: {
            motor_r_in1_Write(1);
            motor_r_in1_Write(0);
            break;
        }
        case REVERSE: {
            motor_r_in1_Write(0);
            motor_r_in1_Write(1);
            break;
        }
    }
}

void set_speed(uint8 speed_l, Direction dir_l, uint8 speed_r, Direction dir_r) {
    set_speed_l(speed_l, dir_l);
    set_speed_r(speed_r, dir_r);
}

// void set_movement(Movement movement) {

// }


void stop() {
    motor_l_in1_Write(0);
    motor_l_in2_Write(0);
    motor_r_in1_Write(0);
    motor_r_in2_Write(0);
}

void turn_left() {
    set_speed(L_TURN_SPEED, FORWARD, R_TURN_SPEED, REVERSE);
    CyDelay(TURN_TIME);
    stop();
}

void turn_right() {
    set_speed(L_TURN_SPEED, REVERSE, R_TURN_SPEED, FORWARD);
    CyDelay(TURN_TIME);
    stop();
}

void move_forward_by(unsigned int distance) {
    set_speed(L_SPEED, FORWARD, R_SPEED, FORWARD);
    CyDelay(distance * DIST_TO_TIME_MS);
    stop();
}

void move_backward_by(unsigned int distance) {
    set_speed(L_SPEED, REVERSE, R_SPEED, REVERSE);
    CyDelay(distance * DIST_TO_TIME_MS);
    stop();

}




/* [] END OF FILE */
