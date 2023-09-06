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


#include <stdbool.h>
#include "cytypes.h"

#define INIT_CAPACITY 8
#define NO_MOVEMENT 0
#define GO_FORWARD 1
#define GO_BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4


// typedefs
typedef struct Movement {
    uint8 type;
    uint32 counts;
} Movement;


// API
void navstack_init(void);
void navstack_reserve(uint8 additional);
uint8 navstack_len(void);
void navstack_push(Movement m);
Movement navstack_pop(void);
Movement navstack_peek(void);
void navstack_clear(void);
bool navstack_is_empty(void);
bool try_merge_movements(Movement* m, Movement other);
void print_navstack(uint8 len);

#endif  // NAVSTACK_H

/* [] END OF FILE */
