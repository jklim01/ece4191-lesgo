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

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H


#include "cytypes.h"

// typedefs
typedef enum __attribute__ ((__packed__)) Color {
    RED,
    GREEN,
    BLUE
} Color;


// globals
extern const char* COLOR_NAMES[3];


// ISRs
CY_ISR_PROTO(color_sensor_ready_isr);


// API
void color_sensor_setup(void);
Color color_sense(void);


#endif  // COLOR_SENSOR_H

/* [] END OF FILE */
