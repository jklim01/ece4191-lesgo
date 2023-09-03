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
#include <stdbool.h>
#include "cytypes.h"

#include "NavStack.h"
#include "utils.h"


// typedef
struct NavStack {
    Movement* ptr;
    uint8 len;
    uint8 capacity;
};


// globals
NavStack ns = { NULL, 0, 0 };


// API
void navstack_init(void) {
    ns = (NavStack) {
        .ptr = malloc(sizeof(Movement) * INIT_CAPACITY),
        .len = 0,
        .capacity = INIT_CAPACITY
    };

    if (ns.ptr == NULL)
        panic(CODE_NAV_STACK_ALLOC_ERROR);
}

void navstack_reserve(uint8 additional) {
    ns.ptr = realloc(ns.ptr, sizeof(Movement) * (ns.capacity + additional));

    if (ns.ptr == NULL)
        panic(CODE_NAV_STACK_ALLOC_ERROR);

    ns.capacity += INIT_CAPACITY;
}

void navstack_push(Movement m) {
    if (ns.capacity == ns.len)
        navstack_reserve(INIT_CAPACITY);

    (ns.ptr)[ns.len] = m;
    ns.len++;
}

Movement navstack_pop(void) {
    ns.len--;
    return (ns.ptr)[ns.len];
}

Movement navstack_peek(void) {
    return (ns.ptr)[ns.len - 1];
}

void navstack_clear(void){
    ns.len = 0;
}

bool navstack_is_empty(void){
    return (ns.len == 0);
}

bool try_merge_movements(Movement* m, Movement other) {
    if (other.type == NO_MOVEMENT)
        return true;

    switch (m->type) {
        case GO_FORWARD: {
            if (other.type == GO_FORWARD) {
                m->counts += other.counts;
                return true;
            }

            if (other.type == GO_BACKWARD) {
                int32 res_counts = (int32)(m->counts) - (int32)(other.counts);
                if (res_counts > 0)
                    m->counts = (uint32)res_counts;
                else {
                    m->type = GO_BACKWARD;
                    m->counts = (uint32)(-res_counts);
                }

                return true;
            }

            return false;
        }
        case GO_BACKWARD: {
            if (other.type == GO_BACKWARD) {
                m->counts += other.counts;
                return true;
            }

            if (other.type == GO_FORWARD) {
                int32 res_counts = (int32)(m->counts) - (int32)(other.counts);
                if (res_counts > 0)
                    m->counts = (uint32)res_counts;
                else {
                    m->type = GO_FORWARD;
                    m->counts = (uint32)(-res_counts);
                }

                return true;
            }

            return false;
        }
        case TURN_LEFT: {
            if (other.type == TURN_RIGHT) {
                m->type = NO_MOVEMENT;
                return true;
            }

            return false;
        }
        case TURN_RIGHT: {
            if (other.type == TURN_LEFT) {
                m->type = NO_MOVEMENT;
                return true;
            }

            return false;
        }
    }
}


/* [] END OF FILE */
