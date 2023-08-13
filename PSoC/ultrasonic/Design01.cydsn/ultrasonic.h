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

extern volatile _Atomic uint16 us_dist;
CY_ISR_PROTO(timer_isr);


#endif  // ULTRASONIC_H

/* [] END OF FILE */
