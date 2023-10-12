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

const uint8 MPU6050 = 0x68;
const uint8 PWR_MGMT_1 = 0x6B;
const uint8 GYRO_CONFIG = 0x1B;
const uint8 CONFIG = 0x1A;
const uint8 GYRO_XOUT_H = 0x43;
static volatile int16 buf[3] = { 0 };
static volatile double yaw = 0;
static volatile double yaw_rate = 0;
static volatile int16 yaw_rate_i16 = 0;
static uint16 dmpPacketSize;


CY_ISR(handler) {
    Timer_1_ReadStatusRegister();

    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(GYRO_XOUT_H);
    I2C_1_MasterSendStop();


    // uint8 my_buf[6];
    // I2C_1_MasterReadBuf(MPU6050, my_buf, sizeof(my_buf), I2C_1_MODE_COMPLETE_XFER);
    // CyDelay(100);
    volatile uint8* buf_ptr = (volatile uint8*)buf;
    I2C_1_MasterSendStart(MPU6050, 1);
    for (uint8 i = sizeof(buf)-1; i > 0; i--)
        buf_ptr[i] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);
    buf_ptr[0] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    I2C_1_MasterSendStop();

    static int16 prev_yaw_rate = 0;
    // yaw_rate_i16 = (my_buf[4] << 8) | my_buf[5];
    yaw_rate_i16 = buf[0];
    yaw_rate = yaw_rate_i16 / 65.536;
    yaw += 0.5 * (prev_yaw_rate + yaw_rate) * 0.1;
    prev_yaw_rate = yaw_rate;
}

void init_dmp(void) {
    // I2Cdev::writeBit(devAddr,0x6B, 7, (val = 1), wireObj); //PWR_MGMT_1: reset with 100ms delay
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6B);
    I2C_1_MasterSendRestart(MPU6050, 1);
    uint8 val = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    val = (val & ~(0b1 << 7)) | (0b1 << 7);
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6B);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();
	CyDelay(100);

	// I2Cdev::writeBits(devAddr,0x6A, 2, 3, (val = 0b111), wireObj); // full SIGNAL_PATH_RESET: with another 100ms delay
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterSendRestart(MPU6050, 1);
    val = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    uint8 mask = 0b111 << (2 - 3 + 1);
    val = (val & ~mask) | (0b111 << (2-3+1));
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();
	CyDelay(100);

    //I2Cdev::writeBytes(devAddr,0x6B, 1, &(val = 0x01), wireObj); // 1000 0001 PWR_MGMT_1:Clock Source Select PLL_X_gyro
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6B);
    I2C_1_MasterWriteByte(0x01);    // FIXME: 1000 0001 has DEVICE_RESET high

	// I2Cdev::writeBytes(devAddr,0x38, 1, &(val = 0x00), wireObj); // 0000 0000 INT_ENABLE: no Interrupt
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x38);
    I2C_1_MasterWriteByte(0x00);

	// I2Cdev::writeBytes(devAddr,0x23, 1, &(val = 0x00), wireObj); // 0000 0000 MPU FIFO_EN: (all off) Using DMP's FIFO instead
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x23);
    I2C_1_MasterWriteByte(0x00);

	// I2Cdev::writeBytes(devAddr,0x1C, 1, &(val = 0x00), wireObj); // 0000 0000 ACCEL_CONFIG: 0 =  Accel Full Scale Select: 2g
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x1C);
    I2C_1_MasterWriteByte(0x00);

	// I2Cdev::writeBytes(devAddr,0x37, 1, &(val = 0x80), wireObj); // 1001 0000 INT_PIN_CFG: ACTL The logic level for int pin is active low. and interrupt status bits are cleared on any read
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x37);
    I2C_1_MasterWriteByte(0x80);    // FIXME
    // I2C_1_MasterWriteByte(0x90);

	// I2Cdev::writeBytes(devAddr,0x6B, 1, &(val = 0x01), wireObj); // 0000 0001 PWR_MGMT_1: Clock Source Select PLL_X_gyro
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6B);
    I2C_1_MasterWriteByte(0x01);

	// I2Cdev::writeBytes(devAddr,0x19, 1, &(val = 0x04), wireObj); // 0000 0100 SMPLRT_DIV: Divides the internal sample rate 400Hz ( Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV))
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x19);
    I2C_1_MasterWriteByte(0x04);    // FIXME: 1kHz / (1 + 4) = 200Hz

	// I2Cdev::writeBytes(devAddr,0x1A, 1, &(val = 0x01), wireObj); // 0000 0001 CONFIG: Digital Low Pass Filter (DLPF) Configuration 188HZ  //Im betting this will be the beat
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x1A);
    // I2C_1_MasterWriteByte(0x01);
    I2C_1_MasterWriteByte(0x05);

	// if (!writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) return 1; // Loads the DMP image into the MPU6050 Memory // Should Never Fail
    writeProgMemoryBlock(dmpMemory, sizeof(dmpMemory), 0, 0, true, true);

	// I2Cdev::writeWords(devAddr, 0x70, 1, &(ival = 0x0400), wireObj); // DMP Program Start Address
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x70);
    I2C_1_MasterWriteByte(0x00);
    I2C_1_MasterWriteByte(0x04);    // TODO: check if the byte order is like this

	// I2Cdev::writeBytes(devAddr,0x1B, 1, &(val = 0x18), wireObj); // 0001 1000 GYRO_CONFIG: 3 = +2000 Deg/sec
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x1B);
    I2C_1_MasterWriteByte(0x18);    // FIXME
    // I2C_1_MasterWriteByte(0x08);

	// I2Cdev::writeBytes(devAddr,0x6A, 1, &(val = 0xC0), wireObj); // 1100 1100 USER_CTRL: Enable Fifo and Reset Fifo
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterWriteByte(0xC0);    // FIXME
    // I2C_1_MasterWriteByte(0xCC);

	// I2Cdev::writeBytes(devAddr,0x38, 1, &(val = 0x02), wireObj); // 0000 0010 INT_ENABLE: RAW_DMP_INT_EN on
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x38);
    I2C_1_MasterWriteByte(0x02);

	// I2Cdev::writeBit(devAddr,0x6A, 2, 1, wireObj);      // Reset FIFO one last time just for kicks. (MPUi2cWrite reads 0x6A first and only alters 1 bit and then saves the byte)
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterSendRestart(MPU6050, 1);
    val = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    val = (val & ~(0b1 << 2)) | (0b1 << 2);
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();

    // setDMPEnabled(false); // disable DMP for compatibility with the MPU6050 library
    //     I2Cdev::writeBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_EN_BIT, enabled, wireObj);
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterSendRestart(MPU6050, 1);
    val = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    val = (val & ~(0b1 << 7)) | (false << 7);
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();

    dmpPacketSize = 28;
}

int main(void)
{

    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_1_Start();
    Timer_1_Start();
    UART_1_Start();
    // isr_1_StartEx(handler);

    // // wake from sleep mode
    // I2C_1_MasterSendStart(MPU6050, 0);
    // I2C_1_MasterWriteByte(PWR_MGMT_1);
    // I2C_1_MasterWriteByte(0x00);
    // I2C_1_MasterSendStop();

    // // set gyro scaling
    // I2C_1_MasterSendStart(MPU6050, 0);
    // I2C_1_MasterWriteByte(GYRO_CONFIG);
    // I2C_1_MasterWriteByte(0x08);
    // I2C_1_MasterSendStop();

    // // setup DLPF (digital LPF)
    // I2C_1_MasterSendStart(MPU6050, 0);
    // I2C_1_MasterWriteByte(CONFIG);
    // I2C_1_MasterWriteByte(0x05);
    // I2C_1_MasterSendStop();
    init_dmp();


    // // supply your own gyro offsets here, scaled for min sensitivity
    // mpu.setXGyroOffset(51);
    // mpu.setYGyroOffset(8);
    // mpu.setZGyroOffset(21);
    // mpu.setXAccelOffset(1150);
    // mpu.setYAccelOffset(-50);
    // mpu.setZAccelOffset(1060);

    // Calibration Time: generate offsets and calibrate our MPU6050
    // mpu.CalibrateAccel(6);
    // mpu.CalibrateGyro(6);

    // mpu.setDMPEnabled(true);
    I2C_1_MasterSendStart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterSendRestart(MPU6050, 1);
    uint8 val = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
    val = (val & ~(0b1 << 7)) | (true << 7);
    I2C_1_MasterSendRestart(MPU6050, 0);
    I2C_1_MasterWriteByte(0x6A);
    I2C_1_MasterWriteByte(val);
    I2C_1_MasterSendStop();

    // enable Arduino interrupt detection
    // Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    // Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    // Serial.println(F(")..."));
    // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    isr_2_StartEx(dmpDataReady);
    // mpuIntStatus = mpu.getIntStatus();

    // get expected DMP packet size for later comparison
    // packetSize = mpu.dmpGetFIFOPacketSize();
    uint16 packetSize = dmpPacketSize;

    for(;;)
    {
        /* Place your application code here. */
        // char str[200];
        // // sprintf(str, "yaw: %.4lf\n", yaw);
        // sprintf(str, "yaw: %.4lf (yaw_rate=%.4lf)\n", yaw, yaw_rate);
        // // sprintf(str, "yaw: %.4lf (yaw_rate=%.4lf\t=%d)\n", yaw, yaw_rate, yaw_rate_i16);
        // UART_1_PutString(str);

        uint8_t fifoBuffer[64];
        if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180 / M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180 / M_PI);
            Serial.print("\t");
            Serial.print(ypr[2] * 180 / M_PI);
        }

        CyDelay(200);
    }
}

/* [] END OF FILE */
