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


// API
void ir_sensor_setup(void (*ir_f_handler_)(void), void (*ir_b_handler_)(void));

void ir_f_pause(void);
void ir_f_resume(void);
bool ir_f_is_detected(void);

void ir_b_pause(void);
void ir_b_resume(void);
bool ir_b_is_detected(void);


#endif  // IR_SENSOR_H

/* [] END OF FILE */
