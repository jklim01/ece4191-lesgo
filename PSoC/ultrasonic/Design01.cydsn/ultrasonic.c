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
#include <stdio.h>

#include "ultrasonic.h"
#include "CircularQ.h"

#define ROUNDING_DIV(a, b) (((a) + (b)/2) / (b))


volatile _Atomic uint16 us_dist_1 = 0;
volatile _Atomic uint16 us_dist_2 = 0;

CY_ISR(timer_1_isr) {
    static char str[15];
    Timer_1_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - Timer_1_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    atomic_store(&us_dist_1, dist);

    sprintf(str, "US1: %3u cm\t", dist);
    UART_1_PutString(str);
}


CY_ISR(timer_2_isr) {
    static char str[15];
    Timer_2_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - Timer_2_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    atomic_store(&us_dist_2, dist);

    sprintf(str, "US2: %3u cm\n", dist);
    UART_1_PutString(str);
}


#undef ROUNDING_DIV
/* [] END OF FILE */
