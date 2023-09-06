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
#include "ir_l_isr.h"
#include "ir_r_isr.h"
#include "ir_mid_isr.h"
#include "ir_l_sreg.h"
#include "ir_r_sreg.h"
#include "ir_mid_sreg.h"
#include "ir_disable.h"

#include "ir_sensor.h"


// static globals
static void (*ir_mid_handler)(void) = NULL;


// API
void ir_sensor_setup(void (*ir_mid_handler_)(void)) {
    if (ir_mid_handler_ != NULL) {
        ir_mid_handler = ir_mid_handler_;
        ir_mid_isr_StartEx(ir_mid_handler);
    }

    ir_disable_Write(0);
}

void ir_sensor_pause(void) {
    ir_disable_Write(1);
    ir_mid_isr_Stop();
}

void ir_sensor_resume(void) {
    ir_mid_isr_ClearPending();
    ir_sensor_setup(ir_mid_handler);
}

bool ir_l_is_detected(void) {
    return ir_l_sreg_Read();
}


/* [] END OF FILE */
