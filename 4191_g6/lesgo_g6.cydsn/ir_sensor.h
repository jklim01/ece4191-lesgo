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
void setup_ir_sensor(
    void (*ir_l_handler_)(void),
    void (*ir_r_handler_)(void),
    void (*ir_side_handler_)(void)
);

void pause_ir_sensor(void);
void resume_ir_sensor(void);
bool is_ir_l_detected(void);
bool is_ir_r_detected(void);
bool is_ir_side_detected(void);


#endif  // IR_SENSOR_H

/* [] END OF FILE */
