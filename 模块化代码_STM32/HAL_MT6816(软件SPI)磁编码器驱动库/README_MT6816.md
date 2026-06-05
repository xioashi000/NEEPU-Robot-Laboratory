# MT6816 磁编码位置检测驱动库

## 1. 简介和功能

本库为 MT6816 磁编码位置检测模拟 SPI 驱动库，基于 STM32 进行开发。

## 2. 移植步骤与使用说明

1. 将 MT6816.c MT6816.h MySPI.c MySPI.h 文件添加到工程中
2. 在 MySPI.h 和 cube_MX 中修改引脚定义，其中 MOSI 应上拉输出，MISO 应上拉输入
3. 在 main.c 中包含

   ```c
   #include "MT6816.h"
   ```

4. 调用语句

   ```c
   MT6816_Init(); // MT6816

   float angle = MT6816_Get_AngleData(); //返回读取指令
   ```

## 3. 注意事项

1. 引脚较多，注意正确接线
2. SPI 为模式 3
