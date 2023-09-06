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
#include <stdio.h>
#include <string.h>

char str[120];
bool rx_finished = false;
CY_ISR(uart_handler) {
    static char* str_ptr = str;
    char rx = UART_1_GetChar();

    if (rx != '!') {
        *str_ptr = rx;
        str_ptr++;
    }
    else {
        *str_ptr = '\0';
        str_ptr = str;
        rx_finished = true;
        UART_1_ClearRxBuffer();
    }
}


int main2(void) {
    CyGlobalIntEnable;
    UART_1_Start();
    UART_1_PutString("I'm done!");
    isr_1_StartEx(uart_handler);

    for(;;) {
        if (rx_finished) {
            UART_1_PutString("\nYou said:\t[");
            for (int i = 0; i < strlen(str); i++) {
                char my_str[10];
                sprintf(my_str, "%c(%u) ", str[i], str[i]);
                UART_1_PutString(my_str);
            }
            UART_1_PutString(" \\0]\n");

            if (strcmp(str, "Are you ready?") == 0)
                UART_1_PutString("Ready  ");
            else if (strcmp(str, "Start") == 0)
                UART_1_PutString("OK  ");
            else {
                int num1;
                int num2;
                char color[10];
                uint8 num = sscanf(str, "%d %s %d", &num1, color, &num2);

                sprintf(str, "(%u) %s %u %u\n", num, color, num1, num2);
                UART_1_PutString(str);
                UART_1_PutString("Received");
            }

            rx_finished = false;
        }
    }
}

/* [] END OF FILE */
