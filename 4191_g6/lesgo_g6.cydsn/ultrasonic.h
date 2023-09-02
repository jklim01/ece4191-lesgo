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
CY_ISR_PROTO(usr_fl_isr);
CY_ISR_PROTO(usr_fr_isr);


// API
void ultrasonic_setup(void);
uint16 us_l_get_dist(void);
uint16 us_r_get_dist(void);
uint16 us_fl_get_dist(void);
uint16 us_fr_get_dist(void);



#endif  // ULTRASONIC_H

/* [] END OF FILE */
