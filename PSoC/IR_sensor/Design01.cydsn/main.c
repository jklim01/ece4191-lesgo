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

#include "ir_sensor.h"

volatile int detection_count = 0;

void my_ir_detected_posedge_handler(void) {
    led_Write(1);
}

void my_ir_detected_negedge_handler(void) {
    /* for (int i = 0; i < 2 * (1000 / 200); i++) { */
    /*     led_Write(1); */
    /*     CyDelay(100); */
    /*     led_Write(0); */
    /*     CyDelay(100); */
    /* } */
    led_Write(0);
    detection_count++;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* setup_ir_sensor(NULL); */
    setup_ir_sensor(&my_ir_detected_posedge_handler, &my_ir_detected_negedge_handler);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        /* led_Write(ir_detected); */
        /* CyDelay(10); */   
        if (detection_count >= 5) {
            for (int i = 0; i < 2 * (1000 / 200); i++) {
                led_Write(1);
                CyDelay(100);
                led_Write(0);
                CyDelay(100);
            }

            pause_ir_sensor();
            CyDelay(5000);
            for (int i = 0; i < 2 * (1000 / 200); i++) {
                led_Write(1);
                CyDelay(100);
                led_Write(0);
                CyDelay(100);
            }

            resume_ir_sensor();
            detection_count = 0;
        }
    }
}

/* [] END OF FILE */
