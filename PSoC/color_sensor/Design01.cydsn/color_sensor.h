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


// ISRs
CY_ISR_PROTO(color_sensor_ready_isr);


// API
void setup_color_sensor(void);
Color color_sense(void);


#endif  // COLOR_SENSOR_H

/* [] END OF FILE */
