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

#include <stdio.h>
#include <stdlib.h>
#include "us_trigger.h"
#include "us_trigger_pwm.h"
#include "us_l_timer.h"
#include "us_r_timer.h"
#include "us_f_timer.h"
#include "us_b_timer.h"
#include "us_l_isr.h"
#include "us_r_isr.h"
#include "us_f_isr.h"
#include "us_b_isr.h"
#include "us_l_echo.h"
#include "us_r_echo.h"
#include "us_f_echo.h"
#include "us_b_echo.h"
#include "cytypes.h"

#include "ultrasonic.h"
#include "CircularQ.h"

#define ROUNDING_DIV(a, b) (((a) + (b)/2) / (b))


// static globals
static volatile CircularQ us_l_buf = { 0, 0, {0} };
static volatile CircularQ us_r_buf = { 0, 0, {0} };
static volatile CircularQ us_f_buf = { 0, 0, {0} };
static volatile CircularQ us_b_buf = { 0, 0, {0} };


// internals
int cmp_func(const void* a, const void* b);
uint16 filter_buf(volatile CircularQ* q);


// ISRs
CY_ISR(us_l_isr) {
    us_l_timer_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - us_l_timer_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_l_buf, dist);
}

CY_ISR(us_r_isr) {
    us_r_timer_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - us_r_timer_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_r_buf, dist);
}

CY_ISR(us_f_isr) {
    us_f_timer_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - us_f_timer_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_f_buf, dist);
}

CY_ISR(us_b_isr) {
    us_b_timer_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - us_b_timer_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_b_buf, dist);
}


// API
void ultrasonic_setup(void) {
    us_trigger_pwm_Start();

    us_l_timer_Start();
    us_r_timer_Start();
    us_f_timer_Start();
    us_b_timer_Start();

    us_l_isr_StartEx(us_l_isr);
    us_r_isr_StartEx(us_r_isr);
    us_f_isr_StartEx(us_f_isr);
    us_b_isr_StartEx(us_b_isr);
}

uint16 us_l_get_dist(void) {
    return filter_buf(&us_l_buf);
}

uint16 us_r_get_dist(void) {
    return filter_buf(&us_r_buf);
}

uint16 us_f_get_dist(void) {
    return filter_buf(&us_f_buf);
}

uint16 us_b_get_dist(void) {
    return filter_buf(&us_b_buf);
}


// internals
int cmp_func(const void* a, const void* b) {
    uint16 a_val = *(uint16*)a;
    uint16 b_val = *(uint16*)b;

    if (a_val < b_val)
        return -1;

    if (a_val == b_val)
        return 0;

    return 1;
}

uint16 filter_buf(volatile CircularQ* q) {
    uint16 my_buf[CIRCULARQ_LEN];
    for (uint8 i = 0; i < CIRCULARQ_LEN; i++)
        my_buf[i] = q->buf[i];

    qsort(my_buf, CIRCULARQ_LEN, sizeof(uint16), &cmp_func);
    return my_buf[CIRCULARQ_LEN / 2];
}


#undef ROUNDING_DIV
/* [] END OF FILE */
