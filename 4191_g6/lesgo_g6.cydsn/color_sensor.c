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
#include "shared_pwm.h"
#include "shared_pwm_demux_select.h"
#include "shared_pwm_reset.h"
#include "color_sensor_ready_isr.h"
#include "color_sensor_counter.h"
#include "color_sensor_s0.h"
#include "color_sensor_s1.h"
#include "color_sensor_s2.h"
#include "color_sensor_s3.h"
#include "color_sensor_led.h"
#include "led_r.h"
#include "led_g.h"
#include "led_b.h"

#include "cytypes.h"
#include "color_sensor.h"

#define MY_DEBUG

#ifdef MY_DEBUG
#include "bluetooth.h"
#endif


// constants
static const uint16 COLOR_SENSOR_PERIOD = 49999;
static const uint16 COLOR_SENSOR_CMP = 25000;


// globals
const char* COLOR_NAMES[4] = { "RED", "GREEN", "BLUE", "INVALID" };


// static globals
static volatile bool ready = false;


// internals
uint32 single_sense(uint8 s2, uint8 s3) {
    ready = false;
    color_sensor_ready_isr_StartEx(color_sensor_ready_isr);
    color_sensor_s2_Write(s2);
    color_sensor_s3_Write(s3);
    CyDelay(20);                    // ensure signals have been written

    // 1ms high pulse on reset pins
    shared_pwm_reset_Write(1);
    CyDelay(1);
    shared_pwm_reset_Write(0);

    while (!ready);
    return color_sensor_counter_ReadCapture();
}


// ISRs
CY_ISR(color_sensor_ready_isr) {
    shared_pwm_reset_Write(1);
    color_sensor_counter_ReadStatusRegister();
    color_sensor_ready_isr_Stop();
    ready = true;
}


// API
void color_sensor_setup(void) {
    // color_sensor_ready_isr_StartEx(color_sensor_ready_isr);
}

Color color_sense(void) {
    // gain control of shared pwm resource
    color_sensor_counter_Start();
    shared_pwm_Start();
    shared_pwm_WritePeriod(COLOR_SENSOR_PERIOD);
    shared_pwm_WriteCompare(COLOR_SENSOR_CMP);
    shared_pwm_demux_select_Write(0x03);

    // turn off indicator leds
    led_r_Write(0);
    led_g_Write(0);
    led_b_Write(0);

    // turn on LED and set frequency scaling
    color_sensor_s0_Write(1);
    color_sensor_s1_Write(1);
    color_sensor_led_Write(1);

    // sense values for each filter
    uint32 red = single_sense(0, 0);
    uint32 blue = single_sense(0, 1);
    // uint32 clear = single_sense(1, 0);
    uint32 green = single_sense(1, 1);

#ifdef MY_DEBUG
    bt_printf("R: %6lu, G: %6lu, B: %6lu  ", red, green, blue);
#endif


    // switch color sensor back to standy mode
    color_sensor_s0_Write(0);
    color_sensor_s1_Write(0);
    color_sensor_led_Write(0);
    shared_pwm_reset_Write(1);
    shared_pwm_Stop();
    color_sensor_counter_Stop();

    // set appropriate indicator led and return detected color
    if ((blue >= red) && (blue >= green) && (blue > 9500)) {
        led_b_Write(1);
#ifdef MY_DEBUG
        bt_printf("(%s)\n", COLOR_NAMES[BLUE]);
#endif
        return BLUE;
    }
    if ((green >= red) && (green > 9500)) {
        led_g_Write(1);
#ifdef MY_DEBUG
        bt_printf("(%s)\n", COLOR_NAMES[GREEN]);
#endif
        return GREEN;
    }
    if (red > 9000) {
        led_r_Write(1);
#ifdef MY_DEBUG
        bt_printf("(%s)\n", COLOR_NAMES[RED]);
#endif
        return RED;
    }
#ifdef MY_DEBUG
        bt_printf("(%s)\n", COLOR_NAMES[INVALID]);
#endif
    return INVALID;
}


#undef COLOR_SENSOR_CMP_TYPE
#undef MY_DEBUG
/* [] END OF FILE */
