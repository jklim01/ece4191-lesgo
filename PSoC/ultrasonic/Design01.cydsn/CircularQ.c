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
        .head = -1,
        .tail = -1,
        .buf = { 0 }
    };
}

void enqueue(volatile CircularQ* q, uint16 elem) {
    CIRCULARQ_IDX_INC(q->head);
    q->buf[q->head] = elem;
}


uint16 dequeue(volatile CircularQ* q) {
    uint16 elem = q->buf[q->tail];
    CIRCULARQ_IDX_INC(q->tail);
    return elem;
}


uint16 peek(volatile CircularQ* q) {
    return q->buf[q->tail];
}



/* [] END OF FILE */
