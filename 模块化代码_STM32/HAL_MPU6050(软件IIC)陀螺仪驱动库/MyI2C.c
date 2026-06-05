#include "gpio.h"
#include "MyI2C.h"


MyI2C_t MPU6050 = {GPIOB, MPU6050SCL_Pin, GPIOB, MPU6050SDA_Pin};

void delay_us(uint32_t udelay) {
    uint32_t startval, tickn, delays, wait;
    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays = udelay * 72;
    if (delays > startval) {
        while (HAL_GetTick() == tickn) {}
        wait = 72000 + startval - delays;
        while (wait < SysTick->VAL) {}
    } else {
        wait = startval - delays;
        while (wait < SysTick->VAL && HAL_GetTick() == tickn) {}
    }
}

/* 引脚控制 */
void MyI2C_W_SCL(MyI2C_t* i2c, GPIO_PinState x) {
    HAL_GPIO_WritePin(i2c->SCL_Port, i2c->SCL_Pin, x);
    delay_us(10);
}

void MyI2C_W_SDA(MyI2C_t* i2c, GPIO_PinState x) {
    HAL_GPIO_WritePin(i2c->SDA_Port, i2c->SDA_Pin, x);
    delay_us(10);
}

uint8_t MyI2C_R_SDA(MyI2C_t* i2c) {
    return HAL_GPIO_ReadPin(i2c->SDA_Port, i2c->SDA_Pin) == GPIO_PIN_RESET ? 0 : 1;
}

/* I2C 初始化 */
void MyI2C_Init(MyI2C_t* i2c) {
    HAL_GPIO_WritePin(i2c->SCL_Port, i2c->SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(i2c->SDA_Port, i2c->SDA_Pin, GPIO_PIN_SET);
}

/* I2C 协议 */
void MyI2C_Start(MyI2C_t* i2c) {
    MyI2C_W_SDA(i2c, 1);
    MyI2C_W_SCL(i2c, 1);
    delay_us(10);
    MyI2C_W_SDA(i2c, 0);
    delay_us(10);
    MyI2C_W_SCL(i2c, 0);
}

void MyI2C_Stop(MyI2C_t* i2c) {
    MyI2C_W_SCL(i2c, 0);
    MyI2C_W_SDA(i2c, 0);
    delay_us(10);
    MyI2C_W_SCL(i2c, 1);
    MyI2C_W_SDA(i2c, 1);
    delay_us(10);
}

void MyI2C_SendByte(MyI2C_t* i2c, uint8_t Byte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        MyI2C_W_SDA(i2c, (Byte & (0x80 >> i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        MyI2C_W_SCL(i2c, 1);
        MyI2C_W_SCL(i2c, 0);
    }
}

uint8_t MyI2C_ReceiveByte(MyI2C_t* i2c) {
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(i2c, 1);
    for (i = 0; i < 8; i++) {
        MyI2C_W_SCL(i2c, 1);
        if (MyI2C_R_SDA(i2c)) {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(i2c, 0);
    }
    return Byte;
}

void MyI2C_SendAck(MyI2C_t* i2c, uint8_t AckBit) {
    MyI2C_W_SDA(i2c, AckBit);
    MyI2C_W_SCL(i2c, 1);
    delay_us(5);
    MyI2C_W_SCL(i2c, 0);
    delay_us(5);
}

uint8_t MyI2C_ReceiveAck(MyI2C_t* i2c) {
    uint8_t AckBit;
    MyI2C_W_SDA(i2c, 1);
    delay_us(5);
    MyI2C_W_SCL(i2c, 1);
    AckBit = MyI2C_R_SDA(i2c);
    MyI2C_W_SCL(i2c, 0);
    delay_us(5);
    return AckBit;
}
