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

#ifndef NAVSTACK_H
#define NAVSTACK_H


#include "cytypes.h"

#define INIT_CAPACITY 8
#define GO_FORWARD 1
#define GO_BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4


// typedefs
typedef struct Movement {
    uint8 type;
    uint32 counts;
} Movement;

typedef struct NavStack {
    Movement* ptr;
    uint8 len;
    uint8 capacity;
} NavStack;


// globals
extern NavStack nav_stack;


// API
void navstack_init(void);
void navstack_reserve(uint8 additional);
void navstack_push(Movement m);
Movement navstack_pop(void);
Movement navstack_peek(uint8 num);
void navstack_clear(void);
void navstack_is_empty(void);
void navstack_simplify(void);

#endif  // NAVSTACK_H

/* [] END OF FILE */
