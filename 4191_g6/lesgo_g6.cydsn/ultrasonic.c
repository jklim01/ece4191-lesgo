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

#include <stdlib.h>
#include "us_trigger.h"
#include "us_trigger_pwm.h"
#include "us_l_timer.h"
#include "us_r_timer.h"
#include "us_fl_timer.h"
#include "us_fr_timer.h"
#include "us_l_isr.h"
#include "us_r_isr.h"
#include "us_fl_isr.h"
#include "us_fr_isr.h"
#include "us_l_echo.h"
#include "us_r_echo.h"
#include "us_fl_echo.h"
#include "us_fr_echo.h"
#include "cytypes.h"

#include "ultrasonic.h"
#include "CircularQ.h"

#define ROUNDING_DIV(a, b) (((a) + (b)/2) / (b))


// static globals
static volatile CircularQ us_l_buf = { 0, 0, {0.0} };
static volatile CircularQ us_r_buf = { 0, 0, {0.0} };
static volatile CircularQ us_fl_buf = { 0, 0, {0.0} };
static volatile CircularQ us_fr_buf = { 0, 0, {0.0} };


// internals
int cmp_func(const void* a, const void* b);
float filter_buf(volatile CircularQ* q);


// ISRs
CY_ISR(us_l_isr) {
    us_l_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_l_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_l_buf, dist);
}

CY_ISR(us_r_isr) {
    us_r_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_r_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_r_buf, dist);
}

CY_ISR(us_fl_isr) {
    us_fl_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_fl_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_fl_buf, dist);
}

CY_ISR(us_fr_isr) {
    us_fr_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_fr_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_fr_buf, dist);
}


// API
void ultrasonic_setup(void) {
    us_trigger_pwm_Start();

    us_l_timer_Start();
    us_r_timer_Start();
    us_fl_timer_Start();
    us_fr_timer_Start();

    us_l_isr_StartEx(us_l_isr);
    us_r_isr_StartEx(us_r_isr);
    us_fl_isr_StartEx(us_fl_isr);
    us_fr_isr_StartEx(us_fr_isr);
    CyDelay(600);
}

float us_l_get_dist(void) {
    return filter_buf(&us_l_buf);
}

float us_r_get_dist(void) {
    return filter_buf(&us_r_buf);
}

float us_fl_get_dist(void) {
    return filter_buf(&us_fl_buf);
}

float us_fr_get_dist(void) {
    return filter_buf(&us_fr_buf);
}


// internals
int cmp_func(const void* a, const void* b) {
    float a_val = *(float*)a;
    float b_val = *(float*)b;

    if (a_val < b_val)
        return -1;

    if (a_val == b_val)
        return 0;

    return 1;
}

float filter_buf(volatile CircularQ* q) {
    float my_buf[CIRCULARQ_LEN];
    for (uint8 i = 0; i < CIRCULARQ_LEN; i++)
        my_buf[i] = q->buf[i];

    qsort(my_buf, CIRCULARQ_LEN, sizeof(float), &cmp_func);
    return my_buf[CIRCULARQ_LEN / 2];
}


#undef ROUNDING_DIV
/* [] END OF FILE */
