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
#include <stdio.h>
#include <stdlib.h>

#include "ultrasonic.h"
#include "CircularQ.h"

#define ROUNDING_DIV(a, b) (((a) + (b)/2) / (b))


// static globals
static volatile CircularQ us_1_buf;
static volatile CircularQ us_2_buf;


// internals
int cmp_func(const void* a, const void* b);
uint16_t filter_buf(volatile CircularQ* q);


// ISRs
CY_ISR(timer_1_isr) {
    static char str[15];
    Timer_1_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - Timer_1_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_1_buf, dist);

    sprintf(str, "US1: %3u cm\t", dist);
    UART_1_PutString(str);
}

CY_ISR(timer_2_isr) {
    static char str[15];
    Timer_2_ReadStatusRegister();       // clears the irq

    uint16 count = UINT16_MAX - Timer_2_ReadCounter();
    uint16 dist = ROUNDING_DIV(count, 58);
    enqueue(&us_1_buf, dist);

    sprintf(str, "US2: %3u cm\n", dist);
    UART_1_PutString(str);
}


// API
void setup_ultrasonic(void) {
    us_trigger_Start();
    Timer_1_Start();
    Timer_2_Start();
    isr_1_StartEx(timer_1_isr);
    isr_2_StartEx(timer_2_isr);

    us_1_buf = circularq_new();
    us_2_buf = circularq_new();
}

uint16_t get_us_1_dist(void) {
    return filter_buf(&us_1_buf);
}

uint16_t get_us_2_dist(void) {
    return filter_buf(&us_2_buf);
}


// internals
int cmp_func(const void* a, const void* b) {
    uint16_t a_val = *(uint16_t*)a;
    uint16_t b_val = *(uint16_t*)b;

    if (a_val < b_val)
        return -1;

    if (a_val == b_val)
        return 0;

    return 1;
}

uint16_t filter_buf(volatile CircularQ* q) {
    uint16_t my_buf[CIRCULARQ_LEN];
    memcpy(my_buf, q->buf, sizeof(q->buf));
    qsort(my_buf, CIRCULARQ_LEN, sizeof(uint16_t), &cmp_func);
    return my_buf[CIRCULARQ_LEN / 2];
}


#undef ROUNDING_DIV
/* [] END OF FILE */
