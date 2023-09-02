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


// globals
extern NavStack v = { NULL, 0, 0 };


// API
void navstack_init(void) {
    v = (NavStack) {
        .ptr = malloc(sizeof(Movement) * INIT_CAPACITY),
        .len = 0,
        .capacity = INIT_CAPACITY
    };

    if (v.ptr == NULL)
        panic(CODE_NAV_STACK_ALLOC_ERROR);
}

void navstack_reserve(uint8 additional) {
    v.ptr = realloc(v.ptr, sizeof(Movement) * (v.capacity + additional));

    if (v.ptr == NULL)
        panic(CODE_NAV_STACK_ALLOC_ERROR);

    v.capacity += INIT_CAPACITY;
}

void navstack_push(Movement m) {
    if (v.capacity == v.len)
        navstack_reserve(INIT_CAPACITY);

    (v.ptr)[v.len] = m;
    v.len++;
}

Movement navstack_pop(void) {
    v.len--;
    return (v.ptr)[v.len];
}

Movement navstack_peek(uint8 num) {
    return (v.ptr)[v.len - num - 1];
}

void navstack_clear(void){
    v.len = 0;
}

bool navstack_is_empty(void){
    return (v.len == 0);
}

void navstack_simplify(void) {
    // TODO
}


/* [] END OF FILE */
