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

#include "cytypes.h"
#include <stdlib.h>

#include "us_trigger.h"
#include "us_trigger_pwm.h"
#include "us_lr_timer.h"
#include "us_fl_timer.h"
#include "us_fr_timer.h"
#include "us_b_timer.h"
#include "us_lr_isr.h"
#include "us_fl_isr.h"
#include "us_fr_isr.h"
#include "us_b_isr.h"
#include "us_l_echo.h"
#include "us_r_echo.h"
#include "us_fl_echo.h"
#include "us_fr_echo.h"
#include "us_b_echo.h"
#include "us_lr_mux_sel.h"

#include "utils.h"
#include "ultrasonic.h"
#include "CircularQ.h"


// static globals
static volatile CircularQ us_lr_buf = { 0, 0, {0.0} };
static volatile CircularQ us_fl_buf = { 0, 0, {0.0} };
static volatile CircularQ us_fr_buf = { 0, 0, {0.0} };
static volatile CircularQ us_b_buf = { 0, 0, {0.0} };
static uint32 us_refresh_period = 0;


// constants
static const uint32 US_CLK_FREQ = 1000000;
static const uint8 US_L_SEL = 0x0;
static const uint8 US_R_SEL = 0x1;


// internals
int cmp_func(const void* a, const void* b);
float filter_buf(volatile CircularQ* q);


// ISRs
CY_ISR(us_lr_isr) {
    us_lr_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_lr_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_lr_buf, dist);
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

CY_ISR(us_b_isr) {
    us_b_timer_ReadStatusRegister();       // clears the irq

    float count = UINT16_MAX - us_b_timer_ReadCounter();
    /* uint16 dist = ROUNDING_DIV(count, 58); */
    float dist = count / 58;
    enqueue(&us_b_buf, dist);
}


// API
void ultrasonic_setup(void) {
    us_trigger_pwm_Start();

    us_lr_timer_Start();
    us_fl_timer_Start();
    us_fr_timer_Start();
    us_b_timer_Start();

    us_lr_isr_StartEx(us_lr_isr);
    us_fl_isr_StartEx(us_fl_isr);
    us_fr_isr_StartEx(us_fr_isr);
    us_b_isr_StartEx(us_b_isr);

    us_refresh_period = ROUNDING_DIV(CIRCULARQ_LEN * (us_trigger_pwm_ReadPeriod() + 1) * 1000, US_CLK_FREQ);
    if (us_refresh_period != 459)
        panic(5);
    CyDelay(us_refresh_period);
}

uint32 us_refresh(void) {
    return us_refresh_period;
}

float us_l_get_dist(void) {
    if (us_lr_mux_sel_Read() == US_R_SEL) {
        us_lr_mux_sel_Write(US_L_SEL);
        CyDelay(us_refresh_period);
    }

    return filter_buf(&us_lr_buf);
}

float us_r_get_dist(void) {
    if (us_lr_mux_sel_Read() == US_L_SEL) {
        us_lr_mux_sel_Write(US_R_SEL);
        CyDelay(us_refresh_period);
    }

    return filter_buf(&us_lr_buf);
}

float us_fl_get_dist(void) {
    return filter_buf(&us_fl_buf);
}

float us_fr_get_dist(void) {
    return filter_buf(&us_fr_buf);
}

float us_b_get_dist(void) {
    return filter_buf(&us_b_buf);
}

float us_get_front_dist(void) {
    return (us_fl_get_dist() + us_fr_get_dist()) / 2;
}

float us_l_get_avg_dist(void) {
    if (us_lr_mux_sel_Read() == US_R_SEL) {
        us_lr_mux_sel_Write(US_L_SEL);
        CyDelay(us_refresh_period);
    }

    float dist = 0;
    for (uint8 i = 0;  i < 4; i++) {
        CyDelay(us_refresh_period / 2);
        dist += filter_buf(&us_lr_buf);
    }

    return dist / 4;
}

float us_r_get_avg_dist(void) {
    if (us_lr_mux_sel_Read() == US_L_SEL) {
        us_lr_mux_sel_Write(US_R_SEL);
        CyDelay(us_refresh_period);
    }

    float dist = 0;
    for (uint8 i = 0;  i < 4; i++) {
        CyDelay(us_refresh_period / 2);
        dist += filter_buf(&us_lr_buf);
    }

    return dist / 4;
}

float us_fl_get_avg_dist(void) {
    float dist = 0;
    for (uint8 i = 0;  i < 4; i++) {
        CyDelay(us_refresh_period / 2);
        dist += us_fl_get_dist();
    }

    return dist;
}

float us_fr_get_avg_dist(void) {
    float dist = 0;
    for (uint8 i = 0;  i < 4; i++) {
        CyDelay(us_refresh_period / 2);
        dist += us_fr_get_dist();
    }

    return dist;
}

float us_b_get_avg_dist(void) {
    float dist = 0;
    for (uint8 i = 0;  i < 4; i++) {
        CyDelay(us_refresh_period / 2);
        dist += us_b_get_dist();
    }

    return dist / 4;
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


/* [] END OF FILE */
