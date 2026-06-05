#include "MPU6050.h"
#include "main.h"
#include "stdio.h"
#include "math.h"
#include "Mohany.h"

#define Acc_Gain 0.00061035//a = Range * G * ACC / 32768  选择2g量程
#define Gyro_Gr 0.0010641f			//Anglerate = ADCrate/灵敏度  选择2000g量程
#define G 9.80665f					// m/s^2
#define Kp 11.50f
#define Ki 0.010f
#define halfT 0.05f						//计算周期的一半，单位s

int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

int16_t AX, AY, AZ, GX, GY, GZ;
float ax, ay, az, gx, gy, gz;
float Yaw,Pitch,Roll;
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;		//四元数
float exInt = 0, eyInt = 0, ezInt = 0;		//叉积计算误差的累计积分
float last_Yaw = 0;
float err_Yaw;
void Calibrate_Gyro() 
{
    int32_t gx_sum = 0, gy_sum = 0, gz_sum = 0;
    const uint16_t samples =70;
    for(int i=0; i<samples; i++){
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        gx_sum += GX;
        gy_sum += GY;
        gz_sum += GZ;
    }
    gx_offset = gx_sum / samples;
    gy_offset = gy_sum / samples;
    gz_offset = gz_sum / samples;
}
static float invSqrt(float x) 		//快速计算 1/Sqrt(x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
void Prepare_Data(void)
{
		//获取加速度计原始值
		//获取角速度原始值
	MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);

		GX -= gx_offset;  // 应用X轴校准
    GY -= gy_offset;  // 应用Y轴校准
    GZ -= gz_offset;  // 补偿Z轴零偏
	
	//将加速度原始AD值转换为m/s^2
	ax = (float)AX * Acc_Gain * G;
	ay = (float)AY * Acc_Gain * G;
	az = (float)AZ * Acc_Gain * G;
	
	//将陀螺仪AD值转换为 弧度/s
	gx = (float)GX * Gyro_Gr;
	gy = (float)GY * Gyro_Gr;
	gz = (float)GZ * Gyro_Gr;
	

}
void Imu_Update(float gx,float gy,float gz,float ax,float ay,float az)
{
    static float last_output_Yaw = 0;  // 添加：用于“死区”Yaw处理
	uint8_t i;
	float vx,vy,vz;							//实际重力加速度
	float ex,ey,ez;							//叉积计算的误差
	float norm;
	
 	float q0q0 = q0*q0;
 	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
 	float q1q2 = q1*q2;
 	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if(ax*ay*az == 0)
		return;

	//加速度计测量的重力方向(机体坐标系)
	norm = invSqrt(ax*ax + ay*ay + az*az);		
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	
	//四元数推出的实际重力方向(机体坐标系)
	vx = 2*(q1q3 - q0q2);												
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	//叉积误差
	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	
	//叉积误差积分为角速度
	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	
	//角速度补偿
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	
	//更新四元数
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;	
	
	//单位化四元数
  	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  	q0 = q0 * norm;
  	q1 = q1 * norm;
  	q2 = q2 * norm;  
  	q3 = q3 * norm;
	
	//四元数反解欧拉角
	float raw_Yaw = atan2(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3f;
	Pitch = -asin(2.f * (q1q3 - q0q2))* 57.3f;
	Roll = atan2(2.f * q2q3 + 2.f * q0q1, q0q0 - q1q1 - q2q2 + q3q3)* 57.3f;

	// 添加：死区判断，限制Yaw更新
	if (fabs(raw_Yaw - last_output_Yaw) > 0.05f) {
		Yaw = raw_Yaw;
		last_output_Yaw = Yaw;
	} else {
		Yaw = last_output_Yaw;
	}
}


