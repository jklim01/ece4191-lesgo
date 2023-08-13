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

#include "project.h"
#include <stdatomic.h>
#include "ultrasonic.h"

volatile _Atomic uint16 us_dist = 0;

CY_ISR(timer_isr) {
    Timer_1_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - Timer_1_ReadCounter();
    uint16 dist = (count + 58/2) / 58;       // perform integer rounding division of `count / 58`
    atomic_store(&us_dist, dist);
}

/* [] END OF FILE */
