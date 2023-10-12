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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "bluetooth.h"
#include "UART_1.h"
#include "color_sensor.h"
#include "isr_1.h"
// #define MY_DEBUG


// globals
char rx_str[120];


// static globals
static volatile bool rx_finished;


// ISRs
CY_ISR(uart_handler) {
    static char* str_ptr = rx_str;

    while (UART_1_GetRxBufferSize() > 0) {
        char c = UART_1_GetChar();
        if (c != '!') {
            *str_ptr = c;
            str_ptr++;
        }
        else {
            *str_ptr = '\0';
            str_ptr = rx_str;
            rx_finished = true;
            UART_1_ClearRxBuffer();

#ifdef MY_DEBUG
            bt_print("\nYou said:\t[");
            for (int i = 0; i < strlen(rx_str); i++) {
                bt_printf("%c(%u) ", rx_str[i], rx_str[i]);
            }
            bt_print(" \\0]\n");
#endif
        }
    }
}



// API
void bt_setup(void) {
    UART_1_Start();
    isr_1_StartEx(uart_handler);
#ifdef MY_DEBUG
    bt_setup("Bluetooth setup!\n");
#endif
}

void bt_print(const char* str) {
    UART_1_PutString(str);
    return;
}

int bt_printf(const char* fmt, ...) {
    static char buf[250];

    va_list va;
    va_start(va, fmt);
    int res = vsprintf(buf, fmt, va);
    va_end(va);

    UART_1_PutString(buf);
    return res;
}

int bt_scanf(const char* fmt, ...) {
    while (!rx_finished);
    rx_finished = false;

    va_list va;
    va_start(va, fmt);
    int res = vsscanf(rx_str, fmt, va);
    va_end(va);

    return res;
}

void bt_block_on(const char* match_str) {
    while (1) {
        if (rx_finished) {
            rx_finished = false;
            if (strcmp(rx_str, match_str) == 0) {
                return;
            }
        }
    }
}

BtResponse bt_init(void) {
    BtResponse res;

    bt_block_on("Are you ready?");
    bt_print("Ready\n");

    while (1) {
        char color[10];
        if (bt_scanf("%hu %9s %hu", &res.lvl_1_zone, color, &res.current_lvl) != 3)
            continue;
        if (!(1 <= res.lvl_1_zone && res.lvl_1_zone <= 6))
            continue;
        if (!(1 <= res.current_lvl && res.current_lvl <= 4))
            continue;

        if (strcmp(color, "red") == 0)
            res.lvl_2_color = RED;
        else if (strcmp(color, "green") == 0)
            res.lvl_2_color = GREEN;
        else if (strcmp(color, "blue") == 0)
            res.lvl_2_color = BLUE;
        else
            continue;

        bt_print("Received\n");
        break;
    }

    bt_block_on("Start");

    return res;
}

bool bt_rx_finished(void) {
    return rx_finished;
}

void bt_clear_rx_finished(void) {
    rx_finished = false;
    return;
}

void bt_block_till_rx(void) {
    while(!rx_finished);
    return;
}


/* [] END OF FILE */
