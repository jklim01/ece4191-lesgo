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

#ifndef UTILS_H
#define UTILS_H


#include "cytypes.h"


// macros
#define FSM(state) switch((state)) { while(1)
#define END_FSM }
#define STATE(x) case (x): {
#define END_STATE break; }


// constants
const uint8 CODE_NAV_STACK_ALLOC_ERROR = 1;


// API
void panic(uint8 panic_code);
void show_code(uint8 code);

void lift_detected_puck(void);


#endif  // UTILS_H

/* [] END OF FILE */
