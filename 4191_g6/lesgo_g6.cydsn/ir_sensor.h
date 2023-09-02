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
void ir_sensor_setup(
    void (*ir_mid_handler_)(void),
    void (*ir_l_handler_)(void),
    void (*ir_r_handler_)(void)
);

void ir_sensor_pause(void);
void ir_sensor_resume(void);
bool ir_mid_is_detected(void);
bool ir_l_is_detected(void);
bool ir_r_is_detected(void);


#endif  // IR_SENSOR_H

/* [] END OF FILE */
