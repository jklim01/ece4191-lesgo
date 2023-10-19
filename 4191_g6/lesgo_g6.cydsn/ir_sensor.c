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
#include "ir_f_isr.h"
#include "ir_f_sreg.h"
#include "ir_f_disable.h"
#include "ir_b_isr.h"
#include "ir_b_sreg.h"
#include "ir_b_disable.h"

#include "ir_sensor.h"


// static globals
static void (*ir_f_handler)(void) = NULL;
static void (*ir_b_handler)(void) = NULL;


// API
void ir_sensor_setup(void (*ir_f_handler_)(void), void (*ir_b_handler_)(void)) {
    if (ir_f_handler_ != NULL)
        ir_f_handler = ir_f_handler_;

    if (ir_b_handler_ != NULL)
        ir_b_handler = ir_b_handler_;

    ir_f_disable_Write(1);
    ir_b_disable_Write(1);
}

void ir_f_pause(void) {
    ir_f_isr_Stop();
    ir_f_disable_Write(1);
}

void ir_f_resume(void) {
    ir_f_disable_Write(0);

    if (ir_f_handler == NULL)
        return;
    ir_f_isr_ClearPending();
    ir_f_isr_StartEx(ir_f_handler);
}

bool ir_f_is_detected(void) {
    return ir_f_sreg_Read() == 0;
}

void ir_b_pause(void) {
    ir_b_isr_Stop();
    ir_b_disable_Write(1);
}

void ir_b_resume(void) {
    ir_b_disable_Write(0);

    if (ir_b_handler == NULL) {
        return;
    }
    CyDelay(200);
    ir_b_isr_ClearPending();
    ir_b_isr_StartEx(ir_b_handler);
}

bool ir_b_is_detected(void) {
    return ir_b_sreg_Read() == 0;
}


/* [] END OF FILE */
