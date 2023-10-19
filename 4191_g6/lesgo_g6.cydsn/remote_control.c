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

#include "project.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "servo.h"
#include "ir_sensor.h"
#include "ultrasonic.h"
#include "limit_sw.h"
#include "bt_debug.h"
#include "bluetooth.h"
#include "locomotion.h"
#include "color_sensor.h"


int main_rc(void) {
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    locomotion_setup();
    color_sensor_setup();
    servo_setup();
    ultrasonic_setup();
    ir_sensor_setup(NULL, &bt_dbg_ir_handler);
    bt_setup();

    panic(bt_dbg());
    return 0;
}


/* [] END OF FILE */
