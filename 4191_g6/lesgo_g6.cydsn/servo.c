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

#include "cytypes.h"
#include "CyLib.h"
#include <stdlib.h>
#include "servo.h"

#include "gripper_pwm.h"
#include "shared_pwm.h"
#include "shared_pwm_reset.h"
#include "shared_pwm_demux_select.h"
#include "gripper_pwm_out.h"
#include "lifter_pwm_out.h"
#include "flicker_pwm_out.h"


// constants
static const uint16 SERVO_PERIOD = 1999;
static const uint8 LIFTER_SELECT = 0x01;
static const uint8 FLICKER_SELECT = 0x02;


// internals
void write_servo_shared(uint8 demux_select, uint16 cmp) {
    shared_pwm_Start();
    shared_pwm_WritePeriod(SERVO_PERIOD);
    shared_pwm_WriteCompare(cmp);
    shared_pwm_demux_select_Write(demux_select);

    // wait for changes to take place
    CyDelay(20);
    shared_pwm_reset_Write(0);


    // wait for servo action to complete
    CyDelay(800);

    shared_pwm_reset_Write(1);
    shared_pwm_Stop();
}

void write_servo_smooth_shared(uint8 demux_select, uint16 cmp_start, uint16 cmp_end) {
    shared_pwm_Start();
    shared_pwm_WritePeriod(SERVO_PERIOD);
    shared_pwm_WriteCompare(cmp_start);
    shared_pwm_demux_select_Write(demux_select);

    // wait for changes to take place
    CyDelay(20);
    shared_pwm_reset_Write(0);

    int16 inc = (cmp_end > cmp_start) ? 1 : -1;
    for (uint16 cmp = cmp_start; cmp != cmp_end; cmp += inc) {
        shared_pwm_WriteCompare(cmp);
        CyDelayUs(12500);
    }
    shared_pwm_WriteCompare(cmp_end);

    shared_pwm_reset_Write(1);
    shared_pwm_Stop();
}


// API
void servo_setup(void) {
    // reset servo positions
    lifter_up();
    gripper_close();
    flicker_up();
}

void gripper_hold_closed(void) {
    // const uint16 GRIPPER_CLOSE_CMP = 193;    // fully closed
    const uint16 GRIPPER_CLOSE_CMP = 202;       // just nice, if too tight can try 203
    gripper_pwm_Start();
    gripper_pwm_WriteCompare(GRIPPER_CLOSE_CMP);
}

void gripper_unactuate(void) {
    gripper_pwm_Stop();
}

void gripper_close(void) {
    gripper_hold_closed();
    CyDelay(800);           // wait for servo action to complete
    gripper_unactuate();
}

void gripper_open(void) {
    const uint16 GRIPPER_OPEN_CMP = 248;
    gripper_pwm_Start();
    gripper_pwm_WriteCompare(GRIPPER_OPEN_CMP);

    // wait for servo action to complete
    CyDelay(800);

    gripper_pwm_Stop();
}

void lifter_up(void) {
    const uint16 LIFTER_UP_CMP = 193;
    const uint16 LIFTER_DOWN_CMP = 120;
    // write_servo_shared(LIFTER_SELECT, LIFTER_UP_CMP);
    CyDelay(400);
    write_servo_smooth_shared(LIFTER_SELECT, LIFTER_DOWN_CMP, LIFTER_UP_CMP);
}

void lifter_down(void) {
    const uint16 LIFTER_DOWN_CMP = 120;
    write_servo_shared(LIFTER_SELECT, LIFTER_DOWN_CMP);
}

void flicker_up(void) {
    const uint16 FLICKER_UP_CMP = 145;
    const uint16 FLICKER_DOWN_CMP = 235;
    write_servo_shared(FLICKER_SELECT, FLICKER_UP_CMP);
    // CyDelay(400);
    // write_servo_smooth_shared(FLICKER_SELECT, FLICKER_DOWN_CMP, FLICKER_UP_CMP);
}

void flicker_down(void) {
    const uint16 FLICKER_DOWN_CMP = 230;
    write_servo_shared(FLICKER_SELECT, FLICKER_DOWN_CMP);
}

void flicker_shoot(void) {
    flicker_down();
    CyDelay(500);
    flicker_up();
}


#undef SERVO_CMP_TYPE
/* [] END OF FILE */
