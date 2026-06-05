# MPU6050_HAL(软件 IIC)陀螺仪驱动库

## 1. 简介和功能

本库为 MPU6050_HAL(软件 IIC)陀螺仪驱动库，基于 STM32 和 MPU6050 陀螺仪，使用 HAL 库进行开发。该库支持多种量程选择，使用结构体封装 iic，更方便的部署多 iic 设备驱动，附加 Mahony 互补滤波算法，更好的数据输出

## 2. 移植步骤与使用说明

1. 将 MPU6050.c MPU6050.h MPU6050_Reg.h MyI2C.c MyI2C.h Mohany.c Mohany.h 文件添加到工程中
2. 在 cube_mx 里将 SCL 和 SDA 引脚配置为 GPIO 推挽输出
3. 在 MyI2C.c 定义 iic 结构体 cube_mx 配置的引脚（程序中已经定义好可自行更改）
4. 初始化 MPU6050 （值得注意的是，在 init 前通常需要添加适当的延时来保证初始化正常）

   ```c
   MPU6050_Init();
   ```

5. 获得原始数据

   ```c
   Calibrate_Gyro();
   ```

6. 处理原始数据

   ```c
   Prepare_Data();
   ```

7. 更新数据

   ```c
   Imu_Update(gx, gy, gz, ax, ay, az);
   ```
