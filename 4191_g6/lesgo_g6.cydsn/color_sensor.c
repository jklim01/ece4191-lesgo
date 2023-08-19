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

#include <stdbool.h>
#include "color_sensor_pwm.h"
#include "color_sensor_creg.h"
#include "color_sensor_ready_isr.h"
#include "color_sensor_counter.h"
#include "color_sensor_s0.h"
#include "color_sensor_s1.h"
#include "color_sensor_s2.h"
#include "color_sensor_s3.h"
#include "color_sensor_led.h"

#include "cytypes.h"
#include "color_sensor.h"


// static globals
static volatile bool ready = false;


// internals
uint32 single_sense(uint8 s2, uint8 s3) {
    ready = false;
    color_sensor_s2_Write(s2);
    color_sensor_s3_Write(s3);
    CyDelay(20);                    // ensure signals have been written

    // 1ms high pulse on reset pins
    color_sensor_creg_Write(1);
    CyDelay(1);
    color_sensor_creg_Write(0);

    while (!ready);
    return color_sensor_counter_ReadCapture();
}


// ISRs
CY_ISR(color_sensor_ready_isr) {
    color_sensor_pwm_ReadStatusRegister();
    ready = true;
}


// API
void setup_color_sensor(void) {
    color_sensor_pwm_Start();
    color_sensor_counter_Start();
    color_sensor_ready_isr_StartEx(color_sensor_ready_isr);
}

Color color_sense(void) {
    color_sensor_s0_Write(1);
    color_sensor_s1_Write(1);
    color_sensor_led_Write(1);

    uint32 red = single_sense(0, 0);
    uint32 blue = single_sense(0, 1);
    // uint32 clear = single_sense(1, 0);
    uint32 green = single_sense(1, 1);

    if ((blue >= red) && (blue >= green))
        return BLUE;
    if (green >= red)
        return GREEN;
    return RED;
}

/* [] END OF FILE */
