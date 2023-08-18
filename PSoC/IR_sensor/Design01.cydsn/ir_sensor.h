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

#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdbool.h>
#include "cytypes.h"


// globals
extern volatile bool ir_detected;


// ISRs
CY_ISR_PROTO(ir_posedge_handler);
CY_ISR_PROTO(ir_negedge_handler);
CY_ISR_PROTO(ir_posedge_handler_extended);
CY_ISR_PROTO(ir_negedge_handler_extended);


// API
void setup_ir_sensor(void (*ir_detected_posedge_handler)(void), void (*ir_detected_negedge_handler)(void));
void pause_ir_sensor(void);
void resume_ir_sensor(void);


#endif

/* [] END OF FILE */
