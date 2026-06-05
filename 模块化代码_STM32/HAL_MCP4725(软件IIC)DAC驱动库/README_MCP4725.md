# MCP4725_HAL(软件 IIC)DAC 驱动库

## 1. 简介和功能

本库为 MCP4725_HAL(软件 IIC)DAC 驱动库，基于 STM32 和 MCP4725DAC 进行开发。 MCP4725 是一款 12 位 DAC 芯片，具有 I2C 接口，支持标准、快速和高速模式，供电电压 2.7-5.5V，内置 EEPROM 存储设置。硬件设计中，通常使用 10K 上拉电阻，后级可搭配比较器或同相放大器。

## 2. 移植步骤与使用说明

1. 将 MCP4725.c MCP4725.h 文件添加到工程中
2. 在 cube_mx 里将 SCL 和 SDA 引脚配置为 GPIO 推挽输出
3. 在 MCP4725.c 定义 iic cube_mx 配置的引脚（程序中已经定义好可自行更改）
4. 初始化 MCP4725

   ```c
   MCP4725_Init();
   ```

5. 使用快速模式写命令写 DAC 寄存器

   ```c
   MCP4725_WriteData_Voltage(uint16_t Vout) //使用快速模式写命令写 DAC 寄存器
   ```

6. 12 位数字量 (max:4095)

   ```c
   MCP4725_WriteData_Digital(uint16_t data) //12 位数字量 (max:4095)
   ```
