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

#include <stddef.h>
#include "limit_sw_l_isr.h"
#include "limit_sw_r_isr.h"
#include "limit_sw_l_sreg.h"
#include "limit_sw_r_sreg.h"

#include "limit_sw.h"


// static globals
static void (*limit_sw_l_handler)(void) = NULL;
static void (*limit_sw_r_handler)(void) = NULL;


// API
void limit_sw_setup(
    void (*limit_sw_l_handler_)(void),
    void (*limit_sw_r_handler_)(void)
) {
    if (limit_sw_l_handler_ != NULL) {
        limit_sw_l_handler = limit_sw_l_handler_;
        limit_sw_l_isr_StartEx(limit_sw_l_handler);
    }

    if (limit_sw_r_handler_ != NULL) {
        limit_sw_r_handler = limit_sw_r_handler_;
        limit_sw_r_isr_StartEx(limit_sw_r_handler);
    }
}

void limit_sw_pause(void) {
    limit_sw_l_isr_Stop();
    limit_sw_r_isr_Stop();
}

void limit_sw_resume(void) {
    limit_sw_setup(
        limit_sw_l_handler,
        limit_sw_r_handler
    );
}

bool limit_sw_l_is_on(void) {
    return limit_sw_l_sreg_Read();
}

bool limit_sw_r_is_on(void) {
    return limit_sw_r_sreg_Read();
}


/* [] END OF FILE */
