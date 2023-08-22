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

#include "CyLib.h"
#include "led.h"

#include "utils.h"


void panic(uint8 panic_code) {
    while(1) {
        // fast blink for 3 seconds
        for (uint8 i = 0; i < 2 * (1000 / 200); i++) {
            led_Write(1);
            CyDelay(100);
            led_Write(0);
            CyDelay(100);
        }

        // display panic code
        CyDelay(1000);
        for (uint8 i = 0; i < panic_code; i++) {
            led_Write(1);
            CyDelay(500);
            led_Write(0);
            CyDelay(500);
        }
    }
}

void show_code(uint8 code) {
    // fast blink for 3 seconds
    for (uint8 i = 0; i < 2 * (1000 / 200); i++) {
        led_Write(1);
        CyDelay(100);
        led_Write(0);
        CyDelay(100);
    }

    // display panic code
    CyDelay(1000);
    for (uint8 i = 0; i < code; i++) {
        led_Write(1);
        CyDelay(500);
        led_Write(0);
        CyDelay(500);
    }
}


/* [] END OF FILE */
