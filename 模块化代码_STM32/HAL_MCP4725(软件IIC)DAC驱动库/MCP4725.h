#ifndef _MCP4725_H
#define _MCP4725_H
 
#include "main.h"
 
#define IIC_SCL(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,(GPIO_PinState)(x))  
#define IIC_SDA(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,(GPIO_PinState)(x))  
#define IIC_SDAIN  HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)
 
 
void IIC_Init(void);                     		 
void IIC_Start(void);				             
void IIC_Stop(void);	  			           
void IIC_Send_Byte(uint8_t txd);			   
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void); 				     
void IIC_Ack(void);			                 
void IIC_NAck(void);				           
 
void SDA_IN(void);
void SDA_OUT(void);

void  MCP4725_WriteData_Voltage(uint16_t Vout);
void MCP4725_Init(void); 
 
#endif
