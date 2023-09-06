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

#pragma once
#ifndef UTILS_H
#define UTILS_H


#include "cytypes.h"


// macros
#define FSM(state) while(1) { switch((state))
#define END_FSM }
#define STATE(x) case (x): {
#define END_STATE break; }


// typedefs
typedef enum __attribute__ ((__packed__)) BasePos {
    BASE_LEFT, BASE_RIGHT
} BasePos;


// constants
#define END_SUCCESS ((uint8)0)
#define CODE_NAV_STACK_ALLOC_ERROR ((uint8)1)


// globals
extern BasePos base;


// API
void panic(uint8 panic_code);
void show_code(uint8 code);


#endif  // UTILS_H

/* [] END OF FILE */
