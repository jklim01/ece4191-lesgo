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
#include "shared_pwm_demux_select.h"
#include "gripper_pwm_out.h"
#include "lifter_pwm_out.h"
#include "flicker_pwm_out.h"


// constants
static const uint16 SERVO_PERIOD = 1999;
static const uint8 GRIPPER_SELECT = 0x01;
static const uint8 LIFTER_SELECT = 0x02;
static const uint8 FLICKER_SELECT = 0x03;


// internals
void write_servo(uint8 demux_select, uint16 cmp) {
    shared_pwm_demux_select_Write(demux_select);
    shared_pwm_WritePeriod(SERVO_PERIOD);
    shared_pwm_WriteCompare(cmp);
    CyDelay(800);
}


// API
void setup_servo(void) {
    shared_pwm_Start();

    // reset gripper position
    gripper_close();

    // reset lifter position
    lifter_up();

    shared_pwm_Sleep();
}

void gripper_close(void) {
    const uint16 GRIPPER_CLOSE_CMP = 875;

    shared_pwm_Wakeup();
    write_servo(GRIPPER_SELECT, GRIPPER_CLOSE_CMP);
    shared_pwm_Sleep();
}

void gripper_open(void) {
    const uint16 GRIPPER_OPEN_CMP = 875;

    shared_pwm_Wakeup();
    write_servo(GRIPPER_SELECT, GRIPPER_OPEN_CMP);
    shared_pwm_Sleep();
}

void lifter_up(void) {
    const uint16 LIFTER_UP_CMP = 875;

    shared_pwm_Wakeup();
    write_servo(LIFTER_SELECT, LIFTER_UP_CMP);
    shared_pwm_Sleep();
}

void lifter_down(void) {
    const uint16 LIFTER_DOWN_CMP = 875;

    shared_pwm_Wakeup();
    write_servo(LIFTER_SELECT, LIFTER_DOWN_CMP);
    shared_pwm_Sleep();
}

void flicker_shoot(void) {
    const uint16 FLICKER_INIT_CMP = 875;
    const uint16 FLICKER_FINAL_CMP = 900;

    shared_pwm_Wakeup();
    write_servo(FLICKER_SELECT, FLICKER_FINAL_CMP);
    write_servo(FLICKER_SELECT, FLICKER_INIT_CMP);
    shared_pwm_Sleep();
}


#undef SERVO_CMP_TYPE
/* [] END OF FILE */
