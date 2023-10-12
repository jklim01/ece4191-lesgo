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

#include "project.h"
#include <stdbool.h>
#include <string.h>
#include "color_sensor.h"
#include "utils.h"
#include "bluetooth.h"

int main4(void) {
    CyGlobalIntEnable;
    bt_setup();
    bt_print("I'm ready!\n");
    isr_1_StartEx(uart_handler);

    for(;;) {
        bt_init();

        // if (rx_finished) {
        //     bt_print("\nYou said:\t[");
        //     for (int i = 0; i < strlen(rx_str); i++) {
        //         bt_printf("%c(%u) ", rx_str[i], rx_str[i]);
        //     }
        //     bt_print(" \\0]\n");

        //     if (strcmp(rx_str, "Are you ready?") == 0)
        //         bt_print("Ready\n");
        //     else if (strcmp(rx_str, "Start") == 0)
        //         bt_print("OK\n");
        //     else {
        //         int num1;
        //         int num2;
        //         char color[10];
        //         uint8 num = sscanf(rx_str, "%d %9s %d", &num1, color, &num2);

        //         bt_printf("(%u) %s %u %u\n", num, color, num1, num2);
        //         bt_print("Received\n");
        //     }

        //     rx_finished = false;
        // }
    }
}

/* [] END OF FILE */
