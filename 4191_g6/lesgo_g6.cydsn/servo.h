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

#ifndef SERVO_H
#define SERVO_H


// API
void servo_setup(void);
void gripper_hold_closed(void);
void gripper_unactuate(void);
void gripper_close(void);
void gripper_open(void);
void lifter_up(void);
void lifter_down(void);
void flicker_up(void);
void flicker_down(void);
void flicker_shoot(void);

#endif

/* [] END OF FILE */
