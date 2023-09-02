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

#include "CyLib.h"
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


// API
void servo_setup(void) {
    // reset lifter position
    lifter_up();

    // reset gripper position
    gripper_close();
}

void gripper_close(void) {
    const uint16 GRIPPER_CLOSE_CMP = 205;
    write_servo(GRIPPER_SELECT, GRIPPER_CLOSE_CMP);
}

void gripper_open(void) {
    const uint16 GRIPPER_OPEN_CMP = 260;
    write_servo(GRIPPER_SELECT, GRIPPER_OPEN_CMP);
}

void lifter_up(void) {
    const uint16 LIFTER_UP_CMP = 210;
    write_servo(LIFTER_SELECT, LIFTER_UP_CMP);
}

void lifter_down(void) {
    const uint16 LIFTER_DOWN_CMP = 115;
    write_servo(LIFTER_SELECT, LIFTER_DOWN_CMP);
}

void flicker_shoot(void) {
    const uint16 FLICKER_INIT_CMP = 875;
    const uint16 FLICKER_FINAL_CMP = 900;

    write_servo(FLICKER_SELECT, FLICKER_FINAL_CMP);
    write_servo(FLICKER_SELECT, FLICKER_INIT_CMP);
}


#undef SERVO_CMP_TYPE
/* [] END OF FILE */
