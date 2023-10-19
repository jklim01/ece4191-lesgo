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

#pragma once
#ifndef UTILS_H
#define UTILS_H


#include "cytypes.h"
#include "color_sensor.h"


// macros
#define ROUNDING_DIV(a, b) (((a) + (b)/2) / (b))
#define FSM(state) while(1) { switch((state))
#define END_FSM }
// #define STATE(x) case (x): { bt_printf("\n\n- - - - - - - - - - %-15s - - - - - - - - - -\n", #x); bt_clear_rx_finished();
#define STATE(x) case (x): { bt_printf("\n\n- - - - - - - - - - %-15s - - - - - - - - - -\n", #x);
#define END_STATE break; }


// typedefs
typedef enum __attribute__ ((__packed__)) BasePos {
    BASE_LEFT, BASE_RIGHT
} BasePos;


// constants
#define END_SUCCESS ((uint8)0)
#define CODE_NAV_STACK_ALLOC_ERROR ((uint8)1)

#define DETECTOR_TO_COLOR_SENSOR 1.2f
#define COLOR_SENSOR_TO_FLICKER 4.0f
#define COLOR_SENSOR_TO_GRIPPER 17.9f /*14.5f 17.9f */
#define ROW_GAP 12.0f
#define ROW_LEN 70.0f
#define PUCK_DIAMETER 4.0f
#define OBSTACLE_DIST_THRESH 13.0f

#define GRIPPER_TO_FLICKER 12.5f /* 18.0f */
#define PIN_DIST_TOL 5.0f
#define ROBOT_FRONT_TO_US_OFFSET 17.0f
#define US_TO_GRIPPER 17.5f

#define MAX_X_1 148
#define MAX_X_2 80


// globals
extern const Color ZONE_TO_COLOR[6];
extern const uint8 COLOR_TO_ZONES[3][2];
extern BasePos base;


// API
void panic(uint8 panic_code);
void show_code(uint8 code);


#endif  // UTILS_H

/* [] END OF FILE */
