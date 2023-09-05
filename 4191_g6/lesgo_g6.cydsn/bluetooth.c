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
// #include <stdbool.h>
// #include <stdio.h>
// #include <string.h>

// char rx;
// char str[120];
// volatile bool stop = false;
// char* token_ptr;
// char task_info[3][20];
// CY_ISR(handler) {
//     static char* str_ptr = str;
//     rx = UART_2_GetChar();

//     if (rx != '!') {
//         *str = rx;
//         str_ptr++;
//     }
//     else {
//         *str = '\0';
//         str_ptr = str;
//         stop = true;
//     }
// }


// int main2(void) {
//     CyGlobalIntEnable;
//     UART_1_Start();
//     UART_2_Start();
//     isr_1_StartEx(handler);

//     for(;;) {
//         if (stop) {
//             if (strcmp(str, "Are you ready?") == 0)
//                 UART_2_PutString("Ready  ");
//             else if (strcmp(str, "Start") == 0)
//                 UART_2_PutString("OK  ");
//             else {
//                 uint8 num1;
//                 uint8 num2;
//                 char color[10];
//                 sscanf(str, "%u %*s %u", &num1, sizeof(color), color, &num2);

//                 int j = 0;
//                 token_ptr = strtok(str, " ");
//                 while (token_ptr != NULL) {
//                     strcpy(task_info[j], token_ptr);
//                     j++;
//                     token_ptr = strtok(NULL, " ");
//                 }

//                 sprintf(str, "%s %u %u\n", color, num1, num2);
//                 UART_1_PutString(str);
//                 UART_1_PutString("Received");
//             }

//             stop = false;
//         }
//     }
// }

// /* [] END OF FILE */
