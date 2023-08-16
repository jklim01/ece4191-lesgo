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
#include "locomotion.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    setup_locomotion();
    move_backward_by(10);

    // UART_1_Start();
    // UART_1_PutString("Im ready!\n");
    // char str[20];
    // char* str_ptr = str;
    // char c;
    // while (1) {
    //     if (UART_1_GetRxBufferSize() > 0) {
    //         c = UART_1_GetChar();

    //         if (c == '!') {
    //             *str_ptr = '\0';
    //             break;
    //         }

    //         *str_ptr = c;
    //         str_ptr++;
    //     }
    // }
    // uint32 counts = strtoul(str, &str_ptr, 10);
    // sprintf(str, "trying %lu\n", counts);
    // UART_1_PutString(str);
    // move_forward_by_counts(counts * 10);

    for(;;)
    {
        /* Place your application code here. */
        // char str[60];
        // sprintf(str, "L: %8li\t R: %8li\n", motor_l_quaddec_GetCounter(), motor_r_quaddec_GetCounter());
        // UART_1_PutString(str);
        // CyDelay(10);
    }

}

/* [] END OF FILE */
