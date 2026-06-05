#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "myspi.h"
/*引脚配置层*/

/**
  * 函    数：SPI写SS引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SS的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SS为低电平，当BitValue为1时，需要置SS为高电平
  */
void MySPI_W_SS(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI写SCK引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCK的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCK为低电平，当BitValue为1时，需要置SCK为高电平
  */
void MySPI_W_SCK(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI写MOSI引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入MOSI的电平，范围0~0xFF
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置MOSI为低电平，当BitValue非0时，需要置MOSI为高电平
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI读MISO引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前MISO的电平，范围0~1
  * 注意事项：此函数需要用户实现内容，当前MISO为低电平时，返回0，当前MISO为高电平时，返回1
  */
uint8_t MySPI_R_MISO(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
}

/**
  * 函    数：SPI初始化
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，实现SS、SCK、MOSI和MISO引脚的初始化
  */
void MySPI_Init(void)
{
    /*设置默认电平*/
    MySPI_W_SS(1);      // SS默认高电平(未选中从设备)
    MySPI_W_SCK(1);     // SCK默认高电平(CPOL=1)
}

/*协议层*/

/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Start(void)
{
    MySPI_W_SS(0);      // 拉低SS，开始时序
}

/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Stop(void)
{
    MySPI_W_SS(1);      // 拉高SS，终止时序
    MySPI_W_SCK(1);     // 恢复SCK到空闲高电平状态
}

/**
  * 函    数：SPI交换传输一个字节，使用SPI模式3
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    uint8_t i, ByteReceive = 0x00;
    
    for (i = 0; i < 8; i++) {
        // 输出当前位到MOSI
        MySPI_W_MOSI(ByteSend & (0x80 >> i));
        
        // 下降沿采样数据(CPHA=1)
        MySPI_W_SCK(0);
        if (MySPI_R_MISO() == 1) {
            ByteReceive |= (0x80 >> i);
        }
        
        // 上升沿准备下一位
        MySPI_W_SCK(1);
    }
    
    return ByteReceive;
}
