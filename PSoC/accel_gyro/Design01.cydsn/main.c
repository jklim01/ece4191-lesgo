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
#include <stdio.h>

static const uint8 MPU6050 = 0x68;
static const uint8 PWR_MGMT_1 = 0x6B;
static const uint8 GYRO_CONFIG = 0x1B;
static const uint8 CONFIG = 0x1A;
static const uint8 GYRO_XOUT_H = 0x43;
static volatile int16 buf[3] = { 0 };
static volatile double yaw = 0;
static volatile double yaw_rate = 0;
static volatile int16 yaw_rate_i16 = 0;

CY_ISR(handler) {
    Timer_1_ReadStatusRegister();

    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(GYRO_XOUT_H);
    I2C_1_MasterSendStop();

    //I2C_1_MasterReadBuf(MPU6050, buf, sizeof(buf), I2C_1_MODE_COMPLETE_XFER);
    volatile uint8* buf_ptr = (volatile uint8*)buf;
    I2C_1_MasterSendStart(MPU6050, 1);
    for (uint8 i = sizeof(buf)-1; i > 0; i--)
        buf_ptr[i] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buf_ptr[0] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();

    // int16 yaw_rate_i16 = (buf_ptr[4] << 8 | buf_ptr[5]);
    static int16 prev_yaw_rate = 0;
    yaw_rate_i16 = buf[0];
    yaw_rate = yaw_rate_i16 / 65.536;
    yaw += 0.5 * (prev_yaw_rate + yaw_rate) * 0.1;
    prev_yaw_rate = yaw_rate;
}
int main(void)
{

    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_1_Start();
    Timer_1_Start();
    isr_1_StartEx(handler);
    UART_1_Start();

    // wake from sleep mode
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(PWR_MGMT_1);
    I2C_1_MasterWriteByte(0x00);
    I2C_1_MasterSendStop();

    // set gyro scaling
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(GYRO_CONFIG);
    I2C_1_MasterWriteByte(0x08);
    I2C_1_MasterSendStop();

    // setup DLPF (digital LPF)
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(CONFIG);
    I2C_1_MasterWriteByte(0x05);
    I2C_1_MasterSendStop();

    for(;;)
    {
        /* Place your application code here. */
        char str[200];
        sprintf(str, "yaw: %.4lf (yaw_rate=%d)\n", yaw, yaw_rate_i16);
        // sprintf(str, "yaw: %.4lf (yaw_rate=%.4lf\t=%d)\n", yaw, yaw_rate, yaw_rate_i16);
        UART_1_PutString(str);

        CyDelay(100);
    }
}

/* [] END OF FILE */
