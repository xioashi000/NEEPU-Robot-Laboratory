#ifndef __MOHANY_H__
#define __MOHANY_H__

extern int16_t AX, AY, AZ, GX, GY, GZ;
extern float ax, ay, az, gx, gy, gz;
extern float Yaw,Pitch,Roll;
extern int16_t gx_offset;
static float invSqrt(float x); 		//¿ìËÙ¼ÆËã 1/Sqrt(x)
void Prepare_Data(void);
void Imu_Update(float gx,float gy,float gz,float ax,float ay,float az);
void Calibrate_Gyro();
#endif /* MOHANY_H */
