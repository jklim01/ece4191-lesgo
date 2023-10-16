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
CY_ISR_PROTO(us_lr_isr);
CY_ISR_PROTO(us_fl_isr);
CY_ISR_PROTO(us_fr_isr);
CY_ISR_PROTO(us_b_isr);


// API
void ultrasonic_setup(void);
uint32 us_refresh(void);

float us_l_get_dist(void);
float us_r_get_dist(void);
float us_fl_get_dist(void);
float us_fr_get_dist(void);
float us_b_get_dist(void);

float us_get_front_dist(void);
float us_get_front_avg_dist(void);
float us_l_get_avg_dist(void);
float us_r_get_avg_dist(void);
float us_fl_get_avg_dist(void);
float us_fr_get_avg_dist(void);
float us_b_get_avg_dist(void);



#endif  // ULTRASONIC_H

/* [] END OF FILE */
