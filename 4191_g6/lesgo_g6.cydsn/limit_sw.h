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
void limit_sw_setup(
    void (*limit_sw_l_handler_)(void),
    void (*limit_sw_r_handler_)(void)
);

void limit_sw_pause(void);
void limit_sw_resume(void);
bool limit_sw_l_is_on(void);
bool limit_sw_r_is_on(void);


#endif  // LIMIT_SW_H

/* [] END OF FILE */
