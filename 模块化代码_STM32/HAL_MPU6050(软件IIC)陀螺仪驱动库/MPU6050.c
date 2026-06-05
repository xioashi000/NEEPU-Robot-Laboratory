#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h"
#include "Mohany.h"


#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址


/**
  * 函    数：MPU6050写寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 参    数：Data 要写入寄存器的数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start(&MPU6050);						//I2C起始
	MyI2C_SendByte(&MPU6050,MPU6050_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	MyI2C_SendByte(&MPU6050,RegAddress);			//发送寄存器地址
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	MyI2C_SendByte(&MPU6050,Data);				//发送要写入寄存器的数据
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	MyI2C_Stop(&MPU6050);						//I2C终止
}

/**
  * 函    数：MPU6050读寄存器
  * 参    数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
  * 返 回 值：读取寄存器的数据，范围：0x00~0xFF
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start(&MPU6050);						//I2C起始
	MyI2C_SendByte(&MPU6050,MPU6050_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	MyI2C_SendByte(&MPU6050,RegAddress);			//发送寄存器地址
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	
	MyI2C_Start(&MPU6050);						//I2C重复起始
	MyI2C_SendByte(&MPU6050,MPU6050_ADDRESS | 0x01);	//发送从机地址，读写位为1，表示即将读取
	MyI2C_ReceiveAck(&MPU6050);					//接收应答
	Data = MyI2C_ReceiveByte(&MPU6050);			//接收指定寄存器的数据
	MyI2C_SendAck(&MPU6050,1);					//发送应答，给从机非应答，终止从机的数据输出
	MyI2C_Stop(&MPU6050);						//I2C终止
	delay_us(10);
	
	return Data;
}

/**
  * 函    数：MPU6050初始化
  * 参    数：无
  * 返 回 值：无
  */
void MPU6050_Init(void)
{
	int i,j;
		for (i = 0; i < 1000; i ++)
	{
		for (j = 0; j < 1000; j ++);
	}
	MyI2C_Init(&MPU6050);									//先初始化底层的I2C
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x00);	//加速度计配置寄存器，选择满量程为±2g
}

/**
  * 函    数：MPU6050获取ID号
  * 参    数：无
  * 返 回 值：MPU6050的ID号
  */
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//返回WHO_AM_I寄存器的值
}

/**
  * 函    数：MPU6050获取数据
  * 参    数：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 参    数：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
  * 返 回 值：无
  */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}

/*
@brief  读取加速度计值
@param  accel_fs_sel: 选择的量程
@param  ax: 存储X轴加速度值的指针
@param  ay: 存储Y轴加速度值的指针
@param  az: 存储Z轴加速度值的指针
*/

void MPU6050_ReadAccel(float* ax, float* ay, float* az,uint16_t accel_fs_sel)
{
		int16_t AX, AY, AZ, GX, GY, GZ;
    /* 将原始数据转换为g值 */
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
    switch (accel_fs_sel) 
    {
        case ACCEL_FS_SEL_2G:
        *ax = (float)AX / 16384.0;
        *ay = (float)AY / 16384.0;
        *az = (float)AZ / 16384.0;
        break;

        case ACCEL_FS_SEL_4G:
        *ax = (float)AX / 8192.0;
        *ay = (float)AY / 8192.0;
        *az = (float)AZ / 8192.0;
        break;

        case ACCEL_FS_SEL_8G:
        *ax = (float)AX / 4096.0;
        *ay = (float)AY / 4096.0;
        *az = (float)AZ / 4096.0;
        break;

        case ACCEL_FS_SEL_16G:
            *ax = (float)AX / 2048.0;
            *ay = (float)AY / 2048.0;
            *az = (float)AZ / 2048.0;
            break;

        default:
            break;
    }
}

/*
@brief  读取陀螺仪值
@param  gyro_fs_sel: 选择的量程
@param  gx: 存储X轴角速度值的指针
@param  gy: 存储Y轴角速度值的指针
@param  gz: 存储Z轴角速度值的指针
*/

void MPU6050_ReadGyro(float* gx, float* gy, float* gz,uint16_t gyro_fs_sel)
{
		int16_t AX, AY, AZ, GX, GY, GZ;
    /* 将原始数据转换为度每秒 */
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

    switch (gyro_fs_sel) 
    {
        case GYRO_FS_SEL_250DPS:
        *gx = (float)GX / 131.0;
        *gy = (float)GY / 131.0;
        *gz = (float)GZ / 131.0;
        break;

        case GYRO_FS_SEL_500DPS:
        *gx = (float)GX / 65.5;
        *gy = (float)GY / 65.5;
        *gz = (float)GZ / 65.5;
        break;

        case GYRO_FS_SEL_1000DPS:
        *gx = (float)GX / 32.8;
        *gy = (float)GY / 32.8;
        *gz = (float)GZ / 32.8;
        break;

        case GYRO_FS_SEL_2000DPS:
        *gx = (float)GX / 16.4;
        *gy = (float)GY / 16.4;
        *gz = (float)GZ / 16.4;
        break;

        default:
        break;
    }
}
