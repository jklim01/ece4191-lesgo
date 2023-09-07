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
#include "CircularQ.h"


CircularQ circularq_new() {
    return (CircularQ) {
        .head = 0,
        .tail = 0,
        .buf = { 0 }
    };
}

void enqueue(volatile CircularQ* q, float elem) {
    CIRCULARQ_IDX_INC(q->head);
    q->buf[q->head] = elem;
}


float dequeue(volatile CircularQ* q) {
    float elem = q->buf[q->tail];
    CIRCULARQ_IDX_INC(q->tail);
    return elem;
}


float peek(volatile CircularQ* q) {
    return q->buf[q->tail];
}



/* [] END OF FILE */
