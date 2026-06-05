#include "MCP4725.h"

#define  VREF   3300

 void delay_us(uint32_t udelay)
{
  uint32_t startval,tickn,delays,wait;
 
  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  //sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays =udelay * 72; //sysc / 1000 * udelay;
  if(delays > startval)
    {
      while(HAL_GetTick() == tickn)
        {
 
        }
      wait = 72000 + startval - delays;
      while(wait < SysTick->VAL)
        {
 
        }
    }
  else
    {
      wait = startval - delays;
      while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
 
        }
    }
}
  void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  IIC_SCL(1);
  IIC_SDA(1);
}
 
  void SDA_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}
 
void SDA_OUT(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  GPIO_InitStruct.Pin =  GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
 
 
void IIC_Start(void)
{
	SDA_OUT();     
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(10);
 	IIC_SDA(0);     //START:when CLK is high,DATA change form high to low 
	delay_us(10);
	IIC_SCL(0);     
}	  
 
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(10);
	IIC_SCL(1); 
	IIC_SDA(1);
	delay_us(10);	
}
 
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      
	IIC_SDA(1);delay_us(2);	   
	IIC_SCL(1);delay_us(2);	 
	while(IIC_SDAIN)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);	   
	return 0;  
} 
 
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us(5);
	IIC_SCL(1);
	delay_us(5);
	IIC_SCL(0);
}
  
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(5);
	IIC_SCL(1);
	delay_us(5);
	IIC_SCL(0);
}					 				     
		  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	  SDA_OUT(); 	    
    IIC_SCL(0);
    for(t=0;t<8;t++)    
    {              
			if((txd&0x80)>>7)
				IIC_SDA(1);
			else
				IIC_SDA(0);
			txd<<=1; 	  
			delay_us(5);   
			IIC_SCL(1);
			delay_us(5); 
			IIC_SCL(0);	
			delay_us(5);
    }	 
} 	    
   
  
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(5);
		    IIC_SCL(1);
        receive<<=1;
        if(IIC_SDAIN)receive++;   
		delay_us(4); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack(); //产生ACK   
    return receive;
}
void MCP4725_Init(void)
{
	IIC_Init();
}
 
//使用快速模式写命令写DAC寄存器
void  MCP4725_WriteData_Voltage(uint16_t Vout)   //电压单位mV   (max:3299)
{
  uint8_t temp;
	uint16_t Dn;
	Dn = (4096*Vout)/VREF;  
	temp = (0x0F00 & Dn) >> 8;  //12位数据。0000XXXX XXXXXXXX 
	IIC_Start();
	IIC_Send_Byte(0XC0);        //器件寻址，器件代码：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0000
	IIC_Wait_Ack();	 
	IIC_Send_Byte(temp); 	      //将高8位送到DAC寄存器
	IIC_Wait_Ack();	 
	IIC_Send_Byte(Dn);          //将低8位送到DAC寄存器
	IIC_Wait_Ack();	
	IIC_Stop();                //产生一个停止条件 	
	HAL_Delay(10);	
}
 
  void  MCP4725_WriteData_Digital(uint16_t data)   //12位数字量   (max:4095)
{
  uint8_t data_H=0,data_L=0;
	data_H = ( 0x0F00 & data) >> 8;
	data_L = 0X00FF & data ;
	IIC_Start();               //20us
	IIC_Send_Byte(0XC0);      //器件寻址，器件代码：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0000  15us
	IIC_Wait_Ack();	          //5us
	IIC_Send_Byte(data_H); 	
	IIC_Wait_Ack();	 
	IIC_Send_Byte(data_L);
	IIC_Wait_Ack();	
	IIC_Stop();               //产生一个停止条件  	20us
	delay_us(10);	
}
