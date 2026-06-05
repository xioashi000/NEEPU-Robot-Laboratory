#ifndef __MPU6050_H
#define __MPU6050_H
#include "stdint.h"
#include "stdio.h"
/* º”ÀŸ∂»º∆¡È√Ù∂»≈‰÷√ */
#define ACCEL_FS_SEL_2G 0x00     		// °¿2g
#define ACCEL_FS_SEL_4G 0x08       // °¿4g
#define ACCEL_FS_SEL_8G 0x10       // °¿8g
#define ACCEL_FS_SEL_16G 0x18       // °¿16g 

/* Õ”¬›“«¡È√Ù∂»≈‰÷√ */

#define GYRO_FS_SEL_250DPS 0x00  // °¿250dps
#define GYRO_FS_SEL_500DPS 0x08     // °¿500dps
#define GYRO_FS_SEL_1000DPS 0x10     // °¿1000dps
#define GYRO_FS_SEL_2000DPS 0x18     // °¿2000dps

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
	uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
void MPU6050_ReadAccel(float* ax, float* ay, float* az,uint16_t accel_fs_sel);
void MPU6050_ReadGyro(float* gx, float* gy, float* gz,uint16_t gyro_fs_sel);
#endif
