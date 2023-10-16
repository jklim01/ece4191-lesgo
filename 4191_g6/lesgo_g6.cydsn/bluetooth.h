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

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "cytypes.h"
#include <stdbool.h>
#include "color_sensor.h"


// typedefs
typedef struct BtResponse  {
    uint8 lvl_1_zone;
    Color lvl_2_color;
    uint8 current_lvl;
} BtResponse;


// globals
extern char rx_str[120];


// ISRs
CY_ISR_PROTO(uart_handler);


// API
void bt_setup(void);
void bt_print(const char* str);
int bt_printf(const char* fmt, ...);
int bt_scanf(const char* fmt, ...);
int bt_block_on(const char* match, const char* fmt, ...);
bool bt_rx_finished(void);
void bt_clear_rx_finished(void);
void bt_block_till_rx(void);
BtResponse bt_init(void);


#endif

/* [] END OF FILE */
