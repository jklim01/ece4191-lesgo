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

#ifndef LIMIT_SW_H
#define LIMIT_SW_H

#include <stdbool.h>
#include "cytypes.h"


// API
void setup_limit_sw(
    void (*limit_sw_l_handler_)(void),
    void (*limit_sw_r_handler_)(void)
);

void pause_limit_sw(void);
void resume_limit_sw(void);
bool is_limit_sw_l_on(void);
bool is_limit_sw_r_on(void);


#endif  // LIMIT_SW_H

/* [] END OF FILE */
