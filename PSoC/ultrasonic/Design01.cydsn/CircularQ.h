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
#define CIRCULARQ_WINDOW_SIZE 8
#define CIRCULARQ_IDX_INC(expr) (expr) = ((expr) == CIRCULARQ_WINDOW_SIZE-1) ? 0 : ((expr) + 1)


#include "cytypes.h"

typedef struct CircularQ {
    int8 head;
    int8 tail;
    uint16 buf[CIRCULARQ_WINDOW_SIZE];
} CircularQ;


CircularQ circularq_new();
void enqueue(CircularQ* q, uint16 elem);
uint16 dequeue(CircularQ* q);
uint16 peek(CircularQ* q);


#endif  // CIRCULARQ_H

/* [] END OF FILE */
