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

CY_ISR(isr_1) {
    UART_1_PutString("In state 0!\n");
}
CY_ISR(isr_2) {
    UART_1_PutString("In state 1!\n");
}
CY_ISR(isr_3) {
    UART_1_PutString("In state 2!\n");
}
CY_ISR(isr_4) {
    UART_1_PutString("In state 3!\n");
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    isr_1_StartEx(isr_1);
    isr_2_StartEx(isr_2);
    isr_3_StartEx(isr_3);
    isr_4_StartEx(isr_4);
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
