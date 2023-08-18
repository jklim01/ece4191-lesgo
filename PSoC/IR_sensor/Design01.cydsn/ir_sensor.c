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

#include "ir_sensor.h"
#include "ir_posedge_isr.h"
#include "ir_negedge_isr.h"


// globals
volatile bool ir_detected = false;


// static globals
static void (*ir_posedge_user_handler)(void) = NULL;
static void (*ir_negedge_user_handler)(void) = NULL;


// ISRs
CY_ISR_PROTO(ir_posedge_handler) {
    ir_detected = false;
}

CY_ISR_PROTO(ir_negedge_handler) {
    ir_detected = true;
}

CY_ISR_PROTO(ir_posedge_handler_extended) {
    ir_posedge_handler();
    (*ir_posedge_user_handler)();
}

CY_ISR_PROTO(ir_negedge_handler_extended) {
    ir_negedge_handler();
    (*ir_negedge_user_handler)();
}


// API
void setup_ir_sensor(void (*ir_detected_posedge_handler)(void), void (*ir_detected_negedge_handler)(void)) {
    if (ir_detected_posedge_handler == NULL)
        ir_negedge_isr_StartEx(ir_negedge_handler);
    else {
        ir_negedge_isr_StartEx(ir_negedge_handler_extended);
        ir_negedge_user_handler = ir_detected_posedge_handler;
    }

    if (ir_detected_negedge_handler == NULL)
        ir_posedge_isr_StartEx(ir_posedge_handler);
    else {
        ir_posedge_isr_StartEx(ir_posedge_handler_extended);
        ir_posedge_user_handler = ir_detected_negedge_handler;
    }
}

void pause_ir_sensor(void) {
    ir_negedge_isr_Stop();
    ir_posedge_isr_Stop();
}

void resume_ir_sensor(void) {
    setup_ir_sensor(ir_negedge_user_handler, ir_posedge_user_handler);
}

/* [] END OF FILE */
