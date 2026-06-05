#include "MyFunc.h"
#include "gpio.h"
#include "MySPI.h"
#include "MT6816.h"


void MT6816_Init(void)
{
	MySPI_Init();					
}
 
float MT6816_Get_AngleData(void)
{
    uint8_t Data1 = 0x00;
    uint8_t Data2 = 0x00;

    MySPI_Start();
    MySPI_SwapByte(0x83);            //向MT6816发送读取角度寄存器指令
    Data1 =MySPI_SwapByte(0xFF);    //将角度数据读回来
    MySPI_Stop();

    MySPI_Start();
    MySPI_SwapByte(0x04);
    Data2 =MySPI_SwapByte(0xFF);
    MySPI_Stop();

    float MyAngle = (((float)((Data1*256+Data2)/4)/16384)*360);    //将0X03和0X04数据进行拼接并将其右移两位，保留14位角度数据，根据公式将读取到的数据转换为角度

    return MyAngle;
}


