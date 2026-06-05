#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f1xx_hal.h"

typedef struct {
    GPIO_TypeDef *SCL_Port;
    uint16_t SCL_Pin;
    GPIO_TypeDef *SDA_Port;
    uint16_t SDA_Pin;
} MyI2C_t;

extern MyI2C_t MPU6050;
// I2C 引脚操作
void MyI2C_W_SCL(MyI2C_t* i2c, GPIO_PinState x);
void MyI2C_W_SDA(MyI2C_t* i2c, GPIO_PinState x);
uint8_t MyI2C_R_SDA(MyI2C_t* i2c);

// I2C 初始化
void MyI2C_Init(MyI2C_t* i2c);

// I2C 基础协议
void MyI2C_Start(MyI2C_t* i2c);
void MyI2C_Stop(MyI2C_t* i2c);
void MyI2C_SendByte(MyI2C_t* i2c, uint8_t Byte);
uint8_t MyI2C_ReceiveByte(MyI2C_t* i2c);
void MyI2C_SendAck(MyI2C_t* i2c, uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(MyI2C_t* i2c);

// 延时函数
void delay_us(uint32_t udelay);

#endif
