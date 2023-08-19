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

#ifndef CIRCULARQ_H
#define CIRCULARQ_H
#define CIRCULARQ_LEN 7
#define CIRCULARQ_IDX_INC(expr) (expr) = ((expr) == CIRCULARQ_LEN-1) ? 0 : ((expr) + 1)


#include "cytypes.h"

typedef struct CircularQ {
    int8 head;
    int8 tail;
    uint16 buf[CIRCULARQ_LEN];
} CircularQ;


CircularQ circularq_new();
void enqueue(volatile CircularQ* q, uint16 elem);
uint16 dequeue(volatile CircularQ* q);
uint16 peek(volatile CircularQ* q);


#endif  // CIRCULARQ_H

/* [] END OF FILE */
