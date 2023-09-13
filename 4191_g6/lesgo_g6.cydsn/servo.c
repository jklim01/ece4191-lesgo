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

#include "shared_pwm.h"
#include "shared_pwm_reset.h"
#include "shared_pwm_demux_select.h"
#include "gripper_pwm_out.h"
#include "lifter_pwm_out.h"
#include "flicker_pwm_out.h"


// constants
static const uint16 SERVO_PERIOD = 1999;
static const uint8 GRIPPER_SELECT = 0x00;
static const uint8 LIFTER_SELECT = 0x01;
static const uint8 FLICKER_SELECT = 0x02;


// internals
void write_servo(uint8 demux_select, uint16 cmp) {
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

void write_servo_smooth(uint8 demux_select, uint16 cmp_start, uint16 cmp_end) {
    shared_pwm_Start();
    shared_pwm_WritePeriod(SERVO_PERIOD);
    shared_pwm_demux_select_Write(demux_select);
    shared_pwm_reset_Write(0);

    int16 inc = (cmp_end > cmp_start) ? 1 : -1;
    for (uint16 cmp = cmp_start; cmp != cmp_end; cmp += inc) {
        // shared_pwm_reset_Write(0);
        shared_pwm_WriteCompare((uint16)cmp);
        CyDelayUs(3500);
        // shared_pwm_reset_Write(1);
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

void gripper_close(void) {
    const uint16 GRIPPER_CLOSE_CMP = 193;
    write_servo(GRIPPER_SELECT, GRIPPER_CLOSE_CMP);
}

void gripper_open(void) {
    const uint16 GRIPPER_OPEN_CMP = 248;
    write_servo(GRIPPER_SELECT, GRIPPER_OPEN_CMP);
}

void lifter_up(void) {
    const uint16 LIFTER_UP_CMP = 198;
    const uint16 LIFTER_DOWN_CMP = 115;
    // write_servo(LIFTER_SELECT, LIFTER_UP_CMP);
    write_servo_smooth(LIFTER_SELECT, LIFTER_DOWN_CMP, LIFTER_UP_CMP);
}

void lifter_down(void) {
    const uint16 LIFTER_DOWN_CMP = 115;
    write_servo(LIFTER_SELECT, LIFTER_DOWN_CMP);
    // write_servo_smooth(LIFTER_SELECT, 198, 115);
}

void flicker_up(void) {
    const uint16 LIFTER_UP_CMP = 139;
    write_servo(FLICKER_SELECT, LIFTER_UP_CMP);
}

void flicker_down(void) {
    const uint16 LIFTER_DOWN_CMP = 240;
    write_servo(FLICKER_SELECT, LIFTER_DOWN_CMP);
}

void flicker_shoot(void) {
    flicker_up();
    flicker_down();
    flicker_up();
}


#undef SERVO_CMP_TYPE
/* [] END OF FILE */
