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

#include <stdatomic.h>
#include "cytypes.h"
#include "CircularQ.h"

extern volatile _Atomic uint16 us_dist_1;
extern volatile _Atomic uint16 us_dist_2;
CY_ISR_PROTO(timer_1_isr);
CY_ISR_PROTO(timer_2_isr);


#endif  // ULTRASONIC_H

/* [] END OF FILE */
