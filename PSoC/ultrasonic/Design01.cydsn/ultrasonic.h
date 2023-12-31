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

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "cytypes.h"


// ISRs
CY_ISR_PROTO(timer_1_isr);
CY_ISR_PROTO(timer_2_isr);


// API
void setup_ultrasonic(void);
uint16_t get_us_1_dist(void);
uint16_t get_us_2_dist(void);



#endif  // ULTRASONIC_H

/* [] END OF FILE */
