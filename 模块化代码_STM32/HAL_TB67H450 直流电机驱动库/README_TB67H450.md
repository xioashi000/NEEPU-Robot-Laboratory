# TB67H450 直流电机驱动库

## 1. 简介和功能

本库为 TB67H450 直流电机驱动库，基于 STM32 进行开发。一片 TB67H450 可用于驱动步进电机的一相。

## 2. 移植步骤与使用说明

1. 将 TB67H450.c 和 TB67H450.h 文件添加到工程中
2. 在 TB67H450.h 中修改引脚定义，本驱动包含双 TB67H450 引脚定义
3. 在 main.c 中包含

   ```c
   #include "TB67H450.h"
   ```

4. 调用语句

   ```c
   TB67H450_Init(); // TB67H450初始化

   //设置TB67H450的两个IN引脚状态
   TB67H450_SetInputA(bool IN1, bool IN2);
   TB67H450_SetInputB(bool IN1, bool IN2);

   //设置TB67H450的两个基准电压大小
   PWM_INA(uint8_t percent);
   PWM_INB(uint8_t percent);

   TB67H450_Brake(); //刹车

   TB67H450_Sleep(); //休眠

   Set_Output_I(int Ia, int Ib); //一次性设置TB67H450的状态和电压

   StepCtrl(uint8_t step); //半步法控制转动
   ```

## 3. 注意事项

1. 引脚较多，注意正确接线
