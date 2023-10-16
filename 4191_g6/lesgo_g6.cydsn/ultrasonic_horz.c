// /* ========================================
//  *
//  * Copyright YOUR COMPANY, THE YEAR
//  * All Rights Reserved
//  * UNPUBLISHED, LICENSED SOFTWARE.
//  *
//  * CONFIDENTIAL AND PROPRIETARY INFORMATION
//  * WHICH IS THE PROPERTY OF your company.
//  *
//  * ========================================
// */
// #include "project.h"
// #include <stdlib.h>
// #include <stdbool.h>

// #include "utils.h"
// #include "color_sensor.h"
// #include "servo.h"
// #include "locomotion.h"
// #include "ir_sensor.h"
// #include "ultrasonic.h"
// #include "limit_sw.h"
// #include "bluetooth.h"


// typedef enum __attribute__ ((__packed__)) State {
//     DETECT_PIN,
//     SEARCH_PUCK,
//     RETURN_TO_BASE
// } State;


// static CY_ISR(ir_handler) {
//     stop();
// }


// int main1(void)
// {
//     CyGlobalIntEnable; /* Enable global interrupts. */

//     /* Place your initialization/startup code here (e.g. MyInst_Start()) */
//     locomotion_setup();
//     color_sensor_setup();
//     servo_setup();
//     ultrasonic_setup();
//     ir_sensor_setup(&ir_handler);
//     ir_sensor_pause();

//     bt_setup();
//     bt_print("Im ready!\n");
//     // while (1) {
//     //     bt_printf("L %3f\t R %3f\t FL %3f\t FR %3f\n",
//     //         us_l_get_dist(), us_r_get_dist(), us_fl_get_dist(), us_fr_get_dist());
//     //     CyDelay(200);
//     // }
//     led_r_Write(0);
//     led_g_Write(0);
//     if (sw_Read() == 0) {
//         led_r_Write(1);
//         base = BASE_LEFT;
//     }
//     else {
//         base = BASE_RIGHT;
//         led_g_Write(1);
//     }


//     // wait until button is released before starting
//     while (sw_Read() == 0);
//     CyDelay(1500);

//     Color zone_color = RED;
//     uint8 zone_number = 0;
//     State state = DETECT_PIN;
//     FSM(state) {
//         STATE(DETECT_PIN) {
//             const float WALL_TO_BASE_OFFSET = 40;
//             reverse_to_align();

//             // detect distance to pin
//             float dist_to_pin = 0.0;
//             if (base == BASE_LEFT) {
//                 for (uint8 i = 0; i < 4; i++) {
//                     dist_to_pin += us_l_get_dist() / 4;
//                     CyDelay(500);
//                 }
//             }
//             else {
//                 for (uint8 i = 0; i < 4; i++) {
//                     dist_to_pin += us_r_get_dist() / 4;
//                     CyDelay(500);
//                 }
//             }

//             // move forward slightly to detect distance to wall
//             move_forward_by(30);
//             float dist_to_wall = 0.0;
//             if (base == BASE_LEFT) {
//                 for (uint8 i = 0; i < 4; i++) {
//                     dist_to_wall += us_l_get_dist() / 4;
//                     CyDelay(500);
//                 }
//             }
//             else {
//                 for (uint8 i = 0; i < 4; i++) {
//                     dist_to_wall += us_r_get_dist() / 4;
//                     CyDelay(500);
//                 }
//             }

//             bt_printf("dist_to_pin = %8.2f, dist_to_wall = %6.2f, offsetted distance = %6.2f\n", dist_to_pin, dist_to_wall, dist_to_pin - (dist_to_wall - WALL_TO_BASE_OFFSET));
//             dist_to_pin -= dist_to_wall - WALL_TO_BASE_OFFSET;
//             if (dist_to_pin < 15) {
//                 zone_number = 6;
//                 zone_color = BLUE;
//             }
//             else if (dist_to_pin < 20) {
//                 zone_number = 5;
//                 zone_color = GREEN;
//             }
//             else if (dist_to_pin < 25) {
//                 zone_number = 4;
//                 zone_color = RED;
//             }
//             else if (dist_to_pin < 30) {
//                 zone_number = 3;
//                 zone_color = BLUE;
//             }
//             else if (dist_to_pin < 35) {
//                 zone_number = 2;
//                 zone_color = GREEN;
//             }
//             else {
//                 zone_number = 1;
//                 zone_color = RED;
//             }

//             bt_printf("detetcted at zone %u (%s)\n", zone_number, COLOR_NAMES[zone_color]);

//             state = SEARCH_PUCK;
//         } END_STATE

//         STATE(SEARCH_PUCK) {
//             // panic(END_SUCCESS);

//             state = RETURN_TO_BASE;
//         } END_STATE

//         STATE(RETURN_TO_BASE) {
//             unwind_navstack_till(0);
//             panic(END_SUCCESS);
//         } END_STATE

//     } END_FSM

//     return 0;
// }

// /* [] END OF FILE */
