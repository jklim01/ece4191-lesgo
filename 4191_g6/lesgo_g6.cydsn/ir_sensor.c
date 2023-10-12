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
#include "ir_isr.h"
#include "ir_sreg.h"
#include "ir_disable.h"

#include "ir_sensor.h"


// static globals
static void (*ir_handler)(void) = NULL;


// API
void ir_sensor_setup(void (*ir_handler_)(void)) {
    if (ir_handler_ != NULL) {
        ir_handler = ir_handler_;
        ir_isr_ClearPending();
    }
}

void ir_sensor_pause(void) {
    ir_isr_Stop();
    ir_disable_Write(1);
}

void ir_sensor_resume(void) {
    ir_disable_Write(0);
    ir_isr_StartEx(ir_handler);
    // ir_isr_Start();
}

bool ir_is_detected(void) {
    return ir_sreg_Read();
}


/* [] END OF FILE */
