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
CY_ISR_PROTO(usr_l_isr);
CY_ISR_PROTO(usr_r_isr);
CY_ISR_PROTO(usr_f_isr);
CY_ISR_PROTO(usr_b_isr);


// API
void setup_ultrasonic(void);
uint16 get_us_l_dist(void);
uint16 get_us_r_dist(void);
uint16 get_us_f_dist(void);
uint16 get_us_rb_dist(void);



#endif  // ULTRASONIC_H

/* [] END OF FILE */
